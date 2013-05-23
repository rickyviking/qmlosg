
#ifndef GLOBE_OSG_H
#define GLOBE_OSG_H

#include <QQuickItem>
#include <QtOpenGL/QGLShaderProgram>
#include <QOpenGLShaderProgram>

// osg
#include <osgViewer/Viewer>
#include <osg/PositionAttitudeTransform>
#include <osgGA/OrbitManipulator>

#include <osgQuickNode.h>


class GlobeOsg : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(qreal zoom READ getZoom WRITE setZoom)
    Q_PROPERTY(bool animate READ getAnimate WRITE setAnimate)

public:
    // default ctor and dtor
    GlobeOsg();
    ~GlobeOsg();

    void setZoom(qreal zoom);
    qreal getZoom();

    void setAnimate(bool animate);
    bool getAnimate();

    // reimplement QuickItem update method
    virtual QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    // sample qml invokable method
    Q_INVOKABLE void home();


protected:

    // NOTE by Riccardo Corsi - this is a workaround to receive mouse events from the qml window.
    // I could not find a way to use QuickItem native methods like mousePressEvent(QMouseEvent*) and so on...
    Q_INVOKABLE void _mousePressEvent(int x, int y);
    Q_INVOKABLE void _mouseDragEvent(int x, int y);


    void setupScene();

    // implement timerEvent method to force update on every frame
    void timerEvent(QTimerEvent *);


    qreal _zoomProp;
    QImage m_texture;

    // internal render node
    OsgQuickNode* _osgQuickNode;

    // local scene root
    osg::ref_ptr<osg::PositionAttitudeTransform> _pat;
    osg::ref_ptr<osgGA::OrbitManipulator> _orbit;
    float _defaultDistance;


    std::vector<osg::Vec2> _mousePressVec;
    std::vector<osg::Vec2> _mouseDragVec;

};
#endif // GLOBE_OSG_H

