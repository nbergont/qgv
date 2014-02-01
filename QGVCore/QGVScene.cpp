/***************************************************************
QGVCore
Copyright (c) 2014, Bergont Nicolas, All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 3.0 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library.
***************************************************************/
#include "QGVScene.h"
#include <QDebug>

QGVScene::QGVScene(const QString &name, QObject *parent) : QGraphicsScene(parent)
{
    _context = gvContext();
    _graph = agopen(name.toLocal8Bit().data(), Agdirected, NULL);
    //setGraphAttribute("fontname", QFont().family());
}

QGVScene::~QGVScene()
{
    gvFreeLayout(_context, _graph);
    agclose(_graph);
    gvFreeContext(_context);
}

void QGVScene::setGraphAttribute(const QString &name, const QString &value)
{
    agattr(_graph, AGRAPH, name.toLocal8Bit().data(), value.toLocal8Bit().data());
}

void QGVScene::setNodeAttribute(const QString &name, const QString &value)
{
    agattr(_graph, AGNODE, name.toLocal8Bit().data(), value.toLocal8Bit().data());
}

void QGVScene::setEdgeAttribute(const QString &name, const QString &value)
{
    agattr(_graph, AGEDGE, name.toLocal8Bit().data(), value.toLocal8Bit().data());
}

QGVNode *QGVScene::addNode(const QString &label)
{
    Agnode_t *node = agnode(_graph, NULL, TRUE);
    if(node == NULL)
    {
        qWarning()<<"Invalid node :"<<label;
        return 0;
    }
    QGVNode *item = new QGVNode(node, this);
    item->setLabel(label);
    addItem(item);
    _nodes.append(item);
    return item;
}

QGVEdge *QGVScene::addEdge(QGVNode *source, QGVNode *target, const QString &label)
{
    Agedge_t* edge = agedge(_graph, source->_node, target->_node, NULL, TRUE);
    if(edge == NULL)
    {
        qWarning()<<"Invalid egde :"<<label;
        return 0;
    }

    QGVEdge *item = new QGVEdge(edge, this);
    item->setLabel(label);
    addItem(item);
    _edges.append(item);
    return item;
}

QGVSubGraph *QGVScene::addSubGraph(const QString &name, bool cluster)
{
    Agraph_t* sgraph;
    if(cluster)
        sgraph = agsubg(_graph, ("cluster_" + name).toLocal8Bit().data(), TRUE);
    else
        sgraph = agsubg(_graph, name.toLocal8Bit().data(), TRUE);

    if(sgraph == NULL)
    {
        qWarning()<<"Invalid subGraph :"<<name;
        return 0;
    }

    QGVSubGraph *item = new QGVSubGraph(sgraph, this);
    addItem(item);
    _subGraphs.append(item);
    return item;
}

void QGVScene::setRootNode(QGVNode *node)
{
    Q_ASSERT(_nodes.contains(node));
    agset(_graph, "root", node->label().toLocal8Bit().data());
}

//Hack ...

typedef struct {
    const char *data;
    int len;
    int cur;
} rdr_t;

static int memiofread(void *chan, char *buf, int bufsize)
{
    const char *ptr;
    char *optr;
    char c;
    int l;
    rdr_t *s;

    if (bufsize == 0) return 0;
    s = (rdr_t *) chan;
    if (s->cur >= s->len)
        return 0;
    l = 0;
    ptr = s->data + s->cur;
    optr = buf;
    do {
        *optr++ = c = *ptr++;
        l++;
    } while (c && (c != '\n') && (l < bufsize));
    s->cur += l;
    return l;
}

Agraph_t *agmemread2(const char *cp)
{
    Agraph_t* g;
    rdr_t rdr;
    Agdisc_t disc;
    Agiodisc_t memIoDisc;

    memIoDisc.afread = memiofread;
    memIoDisc.putstr = AgIoDisc.putstr;
    memIoDisc.flush = AgIoDisc.flush;
    rdr.data = cp;
    rdr.len = strlen(cp);
    rdr.cur = 0;

    disc.mem = &AgMemDisc;
    disc.id = &AgIdDisc;
    disc.io = &memIoDisc;
    g = agread (&rdr, &disc);
    return g;
}

void QGVScene::loadLayout(const QString &text)
{
    _graph = agmemread2(text.toLocal8Bit().constData());

    if(gvLayout(_context, _graph, "dot") != 0)
    {
        qCritical()<<"Layout render error"<<agerrors()<<QString::fromLocal8Bit(aglasterr());
        return;
    }

    //Debug output
    //gvRenderFilename(_context, _graph, "png", "debug.png");

    //Read nodes and edges
    for (Agnode_t* node = agfstnode(_graph); node != NULL; node = agnxtnode(_graph, node))
    {
        QGVNode *inode = new QGVNode(node, this);
        inode->updateLayout();
        addItem(inode);
        for (Agedge_t* edge = agfstout(_graph, node); edge != NULL; edge = agnxtout(_graph, edge))
        {
            QGVEdge *iedge = new QGVEdge(edge, this);
            iedge->updateLayout();
            addItem(iedge);
        }

    }
    update();
}

void QGVScene::applyLayout()
{
    if(gvLayout(_context, _graph, "dot") != 0)
    {
        /*
         * Si plantage ici :
         *  - Verifier que les dll sont dans le repertoire d'execution
         *  - Verifie que le fichier "configN" est dans le repertoire d'execution !
         */
        qCritical()<<"Layout render error"<<agerrors()<<QString::fromLocal8Bit(aglasterr());
        return;
    }

    //Debug output
    //gvRenderFilename(_context, _graph, "canon", "debug.dot");
    //gvRenderFilename(_context, _graph, "png", "debug.png");

    //Update items layout
    foreach(QGVNode* node, _nodes)
        node->updateLayout();

    foreach(QGVEdge* edge, _edges)
        edge->updateLayout();

    foreach(QGVSubGraph* sgraph, _subGraphs)
        sgraph->updateLayout();

    //Graph label
    textlabel_t *xlabel = GD_label(_graph);
    if(xlabel)
    {
        QGraphicsTextItem *item = addText(xlabel->text);
        item->setPos(QGVCore::centerToOrigin(QGVCore::toPoint(xlabel->pos, QGVCore::graphHeight(_graph)), xlabel->dimen.x, -4));
    }

    update();
}

void QGVScene::clear()
{
    gvFreeLayout(_context, _graph);
    _nodes.clear();
    _edges.clear();
    _subGraphs.clear();
    QGraphicsScene::clear();
}

#include <QGraphicsSceneContextMenuEvent>
void QGVScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent)
{
    QGraphicsItem *item = itemAt(contextMenuEvent->scenePos());
    if(item)
    {
        item->setSelected(true);
        if(item->type() == QGVNode::Type)
            emit nodeContextMenu(qgraphicsitem_cast<QGVNode*>(item));
        else if(item->type() == QGVEdge::Type)
            emit edgeContextMenu(qgraphicsitem_cast<QGVEdge*>(item));
        else if(item->type() == QGVSubGraph::Type)
            emit subGraphContextMenu(qgraphicsitem_cast<QGVSubGraph*>(item));
        else
            emit graphContextMenuEvent();
    }
    QGraphicsScene::contextMenuEvent(contextMenuEvent);
}

void QGVScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsItem *item = itemAt(mouseEvent->scenePos());
    if(item)
    {
        if(item->type() == QGVNode::Type)
            emit nodeDoubleClick(qgraphicsitem_cast<QGVNode*>(item));
        else if(item->type() == QGVEdge::Type)
            emit edgeDoubleClick(qgraphicsitem_cast<QGVEdge*>(item));
        else if(item->type() == QGVSubGraph::Type)
            emit subGraphDoubleClick(qgraphicsitem_cast<QGVSubGraph*>(item));
    }
    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}

#include <QVarLengthArray>
#include <QPainter>
void QGVScene::drawBackground(QPainter * painter, const QRectF & rect)
{
    const int gridSize = 25;

    const qreal left = int(rect.left()) - (int(rect.left()) % gridSize);
    const qreal top = int(rect.top()) - (int(rect.top()) % gridSize);

    QVarLengthArray<QLineF, 100> lines;

    for (qreal x = left; x < rect.right(); x += gridSize)
        lines.append(QLineF(x, rect.top(), x, rect.bottom()));
    for (qreal y = top; y < rect.bottom(); y += gridSize)
        lines.append(QLineF(rect.left(), y, rect.right(), y));

    painter->setRenderHint(QPainter::Antialiasing, false);

    painter->setPen(QColor(Qt::lightGray).lighter(110));
    painter->drawLines(lines.data(), lines.size());
    painter->setPen(Qt::black);
    //painter->drawRect(sceneRect());
}
