#-------------------------------------------------
#
# Project created by QtCreator 2013-04-17T09:06:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QGraphViz
TEMPLATE = app

#GraphViz librairie
DEFINES += WITH_CGRAPH

unix {
 CONFIG += link_pkgconfig
 PKGCONFIG += libcdt libgvc libcgraph libgraph
}
win32 {
 #Configure Windows GraphViz path here :
 GRAPHVIZ_PATH = C:/GraphViz
 DEFINES += WIN32_DLL
 DEFINES += GVDLL
 INCLUDEPATH += $$GRAPHVIZ_PATH/include
 LIBS += -L$$GRAPHVIZ_PATH/lib/release/lib -lgvc -lcgraph -lgraph -lcdt
}

SOURCES += main.cpp\
        MainWindow.cpp \
    QGVScene.cpp \
    QGraphicsViewEc.cpp \
    QGVNode.cpp \
    QGVEdge.cpp \
    QGVSubGraph.cpp \
    QGVCore.cpp

HEADERS  += MainWindow.h \
    QGVScene.h \
    QGraphicsViewEc.h \
    QGVNode.h \
    QGVEdge.h \
    QGVSubGraph.h \
    QGVCore.h

FORMS    += MainWindow.ui

RESOURCES += \
    ress.qrc
