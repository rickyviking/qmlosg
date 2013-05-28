
// qmlosg
#include <GlobeOsg.h>
#include <osgQuickNode.h>

// qt quick stuff
#include<QQuickWindow>
#include<QOpenGLFunctions>

#include <QSGSimpleTextureNode>


// osg
#include <osg/ShapeDrawable>
#include <osg/PolygonMode>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Shape>



// stl
#include <math.h>
#include <iostream>




class RotateCallback : public osg::NodeCallback
{
public:
    RotateCallback(osg::PositionAttitudeTransform* pat) :
        _pat(pat),
        _angle(0.f)
    {}

    /** Callback method called by the NodeVisitor when visiting a node.*/
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        osg::Quat rotationX;
        float delta = osg::PI / 6.0 / 60.f; // 30 degrees in a second
        _angle += delta;
        rotationX.makeRotate(_angle, osg::Z_AXIS);
        _pat->setAttitude(rotationX);

        //std::cout << "current rotation is: " << osg::RadiansToDegrees(_angle) << std::endl;

        // note, callback is responsible for scenegraph traversal so
        // they must call traverse(node,nv) to ensure that the
        // scene graph subtree (and associated callbacks) are traversed.
        traverse(node,nv);
    }



protected:
    osg::ref_ptr<osg::PositionAttitudeTransform> _pat;
    float _angle;
};


class PostDrawDebug : public osg::Camera::DrawCallback
{
public:
    /** Functor method called by rendering thread. Users will typically override this method to carry tasks such as screen capture.*/
    virtual void operator () (osg::RenderInfo& renderInfo) const
    {
        std::cout << "YES, I've rendered also frame: " << renderInfo.getState()->getFrameStamp()->getFrameNumber() << std::endl;
    }
};


GlobeOsg::GlobeOsg() :
    _osgQuickNode(NULL),
    _zoomProp(1.0)
{
    m_texture = QImage(":/images/earth.png");

    setFlag(ItemHasContents);

    startTimer(16);
}


GlobeOsg::~GlobeOsg()
{
}

void GlobeOsg::setZoom(qreal zoom)
{
    _orbit->setDistance(zoom * _defaultDistance);
    _zoomProp = zoom;
}

qreal GlobeOsg::getZoom()
{
    return _zoomProp;
}

void GlobeOsg::setAnimate(bool animate)
{
    if(animate)
    {
        if(_pat->getUpdateCallback() == NULL)
            _pat->setUpdateCallback(new RotateCallback(_pat) );
    }
    else
        _pat->setUpdateCallback(NULL);
}

bool GlobeOsg::getAnimate()
{
    return (_pat->getUpdateCallback() != NULL);
}

QSGNode* GlobeOsg::updatePaintNode(QSGNode* qNode, UpdatePaintNodeData* qNodeData)
{
    //std::cout << "updatePaintNode() " << std::endl;

    if(!_osgQuickNode)
    {
        _osgQuickNode = new OsgQuickNode;
        _osgQuickNode->setQuickWindow(window());

        setupScene();
    }

    // pass events here if any occured
    if(!_mousePressVec.empty())
    {
        int x = _mousePressVec[0].x();
        int y = _mousePressVec[0].y();
        _osgQuickNode->getViewer()->getEventQueue()->mouseButtonPress(x, y, osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON);

        _mousePressVec.clear();
    }

    // pass events here if any occured
    if(!_mouseDragVec.empty())
    {
        int x = _mouseDragVec[0].x();
        int y = _mouseDragVec[0].y();
        _osgQuickNode->getViewer()->getEventQueue()->mouseMotion(x, y);
        std::cout << "Mouse Motion at " << x << ", "  << y << std::endl;
        _mouseDragVec.clear();
    }

    return _osgQuickNode;

}

void GlobeOsg::home()
{
    // reset to home
    _osgQuickNode->getViewer()->home();
}

void GlobeOsg::setupScene()
{
    // setup a sphere to render
    osg::ShapeDrawable* pSD = new osg::ShapeDrawable(new osg::Box(osg::Vec3(0, 0, 0), 10));
    pSD->setColor(osg::Vec4(1, 0, 1, 0.5));
    osg::Geode* geode = new osg::Geode;
    geode->addDrawable(pSD);

    osg::StateSet* ss = geode->getOrCreateStateSet();
    ss->setMode(GL_CULL_FACE, osg::StateAttribute::ON);
    //geode->getOrCreateStateSet()->setAttributeAndModes(new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE));
    //geode->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);

    osg::Shader* pvert = new osg::Shader(osg::Shader::VERTEX);
    // read vertex shader from resources
    {
        QFile file(":/shaders/globe.vert");
        file.open(QIODevice::ReadOnly | QIODevice::Text);

        QTextStream in(&file);
        QString line = in.readAll();
        pvert->setShaderSource(line.toStdString());
    }

    osg::Shader* pFrag = new osg::Shader(osg::Shader::FRAGMENT);
    // read frag shader from resources
    {
        QFile file(":/shaders/globe.frag");
        file.open(QIODevice::ReadOnly | QIODevice::Text);

        QTextStream in(&file);
        QString line = in.readAll();
        pFrag->setShaderSource(line.toStdString());
    }

    osg::Program* pProg = new osg::Program;
    pProg->addShader(pvert);
    pProg->addShader(pFrag);
    geode->getOrCreateStateSet()->setAttributeAndModes(pProg);


    _pat = new osg::PositionAttitudeTransform;
    _pat->addChild(geode);

    osg::Group* sceneRoot = dynamic_cast<osg::Group*>(_osgQuickNode->getViewer()->getSceneData());
    if(sceneRoot == NULL)
    {
        std::cout << "Creating NEW scene home" << std::endl;
        sceneRoot = new osg::Group;
        _osgQuickNode->getViewer()->setSceneData(sceneRoot);
    }

    sceneRoot->addChild(_pat.get());

    // setup a nice default FOV
    osg::Camera* pCamera = _osgQuickNode->getViewer()->getCamera();
    double fov, ar, zMin, zMax;
    pCamera->getProjectionMatrixAsPerspective(fov, ar, zMin, zMax);
    fov = 65.0f / ar;
    pCamera->setProjectionMatrixAsPerspective(fov, ar, zMin, zMax);

    //pCamera->setPostDrawCallback(new PostDrawDebug() );

    // also add an orbit manipulator
    _orbit = new osgGA::OrbitManipulator;
    _osgQuickNode->getViewer()->setCameraManipulator(_orbit.get());
    _osgQuickNode->getViewer()->home();
    _defaultDistance = _orbit->getDistance();
}

void GlobeOsg::timerEvent(QTimerEvent *)
{
    update();
}


void GlobeOsg::_mousePressEvent(int x, int y)
{
    // fwd mouse event to osg
    _mousePressVec.push_back(osg::Vec2(x, y));
    //_osgViewer->getEventQueue()->mouseButtonPress(x, y, osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON);
}

void GlobeOsg::_mouseDragEvent(int x, int y)
{
    // fwd mouse event to osg
    //std::cout << "MouseDragged at " << x << ", "  << y << std::endl;
    _mouseDragVec.push_back(osg::Vec2(x, y));
    //_osgViewer->getEventQueue()->mouseMotion(x, y);
}
