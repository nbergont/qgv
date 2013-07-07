#-------------------------------------------------
#
# Project created by QtCreator 2013-04-17T09:06:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QGraphViz
TEMPLATE = app


#Configure GraphViz path here :
GRAPHVIZ_PATH = C:/GraphViz

#GraphViz librairie
DEFINES += WIN32_DLL
DEFINES += GVDLL
DEFINES += WITH_CGRAPH
INCLUDEPATH += $$GRAPHVIZ_PATH/include
LIBS += -L$$GRAPHVIZ_PATH/lib/release/lib -lgvc -lcgraph -lgraph -lcdt


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
