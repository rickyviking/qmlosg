
#include <GlobeOsg.h>

#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlContext>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<GlobeOsg>("qmlosg.globe", 1, 0, "GlobeOsg");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);

    view.setSource(QUrl("../../../../qmlosgexample.qml"));


    view.rootContext()->setContextProperty("Window", &view);
    view.show();
    view.raise();

    return app.exec();
}
