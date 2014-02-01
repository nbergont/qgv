#-------------------------------------------------
#
# Project created by QtCreator 2013-04-17T09:06:06
#
#-------------------------------------------------

QT       += core

TARGET = QGVCore
TEMPLATE = lib
CONFIG += shared

DESTDIR = ../lib
DLLDESTDIR = ../bin

#GraphViz librairie
!include(GraphViz.pri) {
     error("fail open GraphViz.pri")
 }

SOURCES += QGVScene.cpp \
    QGVNode.cpp \
    QGVEdge.cpp \
    QGVSubGraph.cpp \
    QGVCore.cpp

HEADERS  += QGVScene.h \
    QGVNode.h \
    QGVEdge.h \
    QGVSubGraph.h \
    QGVCore.h
