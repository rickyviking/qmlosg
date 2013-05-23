
#ifndef OSG_QUICK_NODE_H
#define OSG_QUICK_NODE_H

#include <QtQuick/QSGSimpleTextureNode>
#include <QtQuick/QSGTextureMaterial>
#include <QtQuick/QSGOpaqueTextureMaterial>
#include <QtQuick/QQuickWindow>

#include <QtQuick/QSGFlatColorMaterial>

// osg
#include <osgViewer/Viewer>
#include <osg/PositionAttitudeTransform>



class OsgQuickNode : public QSGGeometryNode
{
public:
    OsgQuickNode();
    ~OsgQuickNode();

    void setSize(const QSize &size);
    QSize size() const { return m_size; }

    void setQuickWindow(QQuickWindow *window);

    void setAAEnabled(bool enable);

    void renderOsgFrame();

    void init();

    void preprocess();
    void updateFBO();

    // return osg viewer, mainly for setting scene, camera manipulator, etc...
    osgViewer::Viewer* getViewer(){ return _osgViewer.get();}

protected:

    // internal utility for osg rendering/binding
    void saveOsgState();
    void restoreOsgState();
    GLuint getGLtexId();

    // TEST scene
    osg::Node* createScene();

    // Quick geometry stuff
    QSGTextureMaterial _qTexMaterial;
    QSGOpaqueTextureMaterial _qOpaqueMaterial;
    QSGGeometry _qGeometry;
    QSGTexture *_qTexture;

    // QtQuick container window
    QQuickWindow *_quickWindow;


    int _samples;
    bool _AAEnabled;
    QSize m_size;


    bool _initialized;
    bool _dirtyFBO;

    // Pointer to QOpenGLContext to be used by OSG.
    QOpenGLContext* _osgContext;
    // Pointer to QOpenGLContext to be restored after OSG context
    QOpenGLContext* _qtContext;


    // osg stuff
    osg::ref_ptr<osgViewer::Viewer> _osgViewer;
    osg::ref_ptr<osg::PositionAttitudeTransform> _pat;
    osg::ref_ptr<osg::Texture2D> _fboTex;
};

#endif // OSG_QUICK_NODE_H
