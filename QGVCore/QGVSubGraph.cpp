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
#include "QGVSubGraph.h"
#include <QGVScene.h>
#include <QDebug>
#include <QPainter>

QGVSubGraph::QGVSubGraph(Agraph_t* subGraph, QGVScene *scene): _sgraph(subGraph), _scene(scene)
{
    //setFlag(QGraphicsItem::ItemIsSelectable, true);
}

QGVSubGraph::~QGVSubGraph()
{
    _scene->removeItem(this);
}

QString QGVSubGraph::name() const
{
    return QString::fromLocal8Bit(GD_label(_sgraph)->text);
}

QGVNode *QGVSubGraph::addNode(const QString &label)
{
    Agnode_t *node = agnode(_sgraph, NULL, TRUE);
    if(node == NULL)
    {
        qWarning()<<"Invalid sub node :"<<label;
        return 0;
    }
    agsubnode(_sgraph, node, TRUE);

    QGVNode *item = new QGVNode(node, _scene);
    item->setLabel(label);
    _scene->addItem(item);
    _scene->_nodes.append(item);
    _nodes.append(item);
    return item;
}

QGVSubGraph *QGVSubGraph::addSubGraph(const QString &name, bool cluster)
{
    Agraph_t* sgraph;
    if(cluster)
        sgraph = agsubg(_sgraph, ("cluster_" + name).toLocal8Bit().data(), TRUE);
    else
        sgraph = agsubg(_sgraph, name.toLocal8Bit().data(), TRUE);

    if(sgraph == NULL)
    {
        qWarning()<<"Invalid subGraph :"<<name;
        return 0;
    }

    QGVSubGraph *item = new QGVSubGraph(sgraph, _scene);
    _scene->_subGraphs.append(item);
    _scene->addItem(item);
    return item;
}

QRectF QGVSubGraph::boundingRect() const
{
    return QRectF(0,0, _width, _height);
}

void QGVSubGraph::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();

    painter->setPen(_pen);
    painter->setBrush(_brush);

    painter->drawRect(boundingRect());
    painter->drawText(_label_rect, Qt::AlignCenter, _label);
    painter->restore();
}

void QGVSubGraph::setAttribute(const QString &name, const QString &value)
{
    agsafeset(_sgraph, name.toLocal8Bit().data(), value.toLocal8Bit().data(), "");
}

QString QGVSubGraph::getAttribute(const QString &name) const
{
    char* value = agget(_sgraph, name.toLocal8Bit().data());
    if(value)
        return value;
    return QString();
}

void QGVSubGraph::updateLayout()
{
    prepareGeometryChange();

    //SubGraph box
    boxf box = GD_bb(_sgraph);
    pointf p1 = box.UR;
    pointf p2 = box.LL;
    _width = p1.x - p2.x;
    _height = p1.y - p2.y;

    qreal gheight = QGVCore::graphHeight(_scene->_graph);
    setPos(p2.x, gheight - p1.y);

    _pen.setWidth(1);
    _brush.setStyle(QGVCore::toBrushStyle(getAttribute("style")));
    _brush.setColor(QGVCore::toColor(getAttribute("fillcolor")));
    _pen.setColor(QGVCore::toColor(getAttribute("color")));

    //SubGraph label
    textlabel_t *xlabel = GD_label(_sgraph);
    if(xlabel)
    {
        _label = xlabel->text;
        _label_rect.setSize(QSize(xlabel->dimen.x, xlabel->dimen.y));
        _label_rect.moveCenter(QGVCore::toPoint(xlabel->pos, QGVCore::graphHeight(_scene->_graph)) - pos());
    }
}
