CONFIG += qt
QT += qml quick
TEMPLATE = app
TARGET = qmlosg

RESOURCES += ./resources.qrc


INCLUDEPATH += /Users/ricky/sourcecode/OpenSceneGraph-3.1.5/include

OSG_LIB_PATH = /Users/ricky/sourcecode/OpenSceneGraph-3.1.5/build/lib
LIBS += -L$$OSG_LIB_PATH -losg -losgGA -losgQt -losgViewer -lOpenThreads -losgText



UI_DIR = ./.ui
OBJECTS_DIR = ./.obj
MOC_DIR = ./.moc


SOURCES += main.cpp \
    osgQuickNode.cpp \
    GlobeOsg.cpp

HEADERS += osgQuickNode.h \
    GlobeOsg.h

OTHER_FILES += ./qmlosgexample.qml
