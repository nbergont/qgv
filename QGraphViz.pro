#-------------------------------------------------
#
# Project created by QtCreator 2013-04-17T09:06:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QGraphViz
TEMPLATE = app


#Configure GraphViz
GRAPHVIZ_DIR = $$PWD/GraphViz

DEFINES += WIN32_DLL
DEFINES += GVDLL
DEFINES += WITH_CGRAPH
INCLUDEPATH += $$GRAPHVIZ_DIR/include
LIBS += -L$$GRAPHVIZ_DIR/lib/release/lib -lgvc -lcgraph -lgraph -lcdt


SOURCES += main.cpp\
        MainWindow.cpp \
    QGVScene.cpp \
    QGraphicsViewEc.cpp \
    QGVNode.cpp \
    QGVEdge.cpp \
    QGVSubGraph.cpp

HEADERS  += MainWindow.h \
    QGVScene.h \
    QGraphicsViewEc.h \
    QGVNode.h \
    QGVEdge.h \
    QGVSubGraph.h

FORMS    += MainWindow.ui
