#include "QGVSubGraph.h"
#include <QGVScene.h>
#include <QDebug>
#include <QPainter>

QGVSubGraph::QGVSubGraph(const QString &name, QGVScene *scene): _scene(scene)
{
    _sgraph = agsubg(_scene->_graph, name.toLocal8Bit().data(), TRUE);
    Q_ASSERT(_sgraph);
    _scene->_subGraphs.append(this);
    _scene->addItem(this);

    setAttribute("label", name);
    setAttribute("color", "blue");
}

QGVSubGraph::QGVSubGraph(Agraph_t* subGraph, QGVScene *scene): _sgraph(subGraph), _scene(scene)
{
    updateLayout();
}

QGVSubGraph::~QGVSubGraph()
{
    _scene->removeItem(this);
    _scene->_subGraphs.removeOne(this);
    //agdelnode(_scene->_graph, _sgraph);
}

QString QGVSubGraph::name() const
{
    return QString::fromLocal8Bit(GD_label(_sgraph)->text);
}

QGVNode *QGVSubGraph::addNode(const QString &name)
{
    QGVNode *node = new QGVNode(name, _scene);
    agsubnode(_sgraph, node->_node, TRUE);
    return node;
}

QGVEdge *QGVSubGraph::addEdge(QGVNode *source, QGVNode *target, const QString &label)
{
    QGVEdge *edge = new QGVEdge(source, target, label, _scene);
    agsubedge(_sgraph, edge->_edge, TRUE);
    return edge;
}

QRectF QGVSubGraph::boundingRect() const
{
    return QRectF(0,0, _width, _height);
}

void QGVSubGraph::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();
    painter->drawRect(boundingRect());
    painter->restore();
}

void QGVSubGraph::setAttribute(const QString &name, const QString &value)
{
    agsafeset(_sgraph, name.toLocal8Bit().data(), value.toLocal8Bit().data(), "");
    //agattr(_sgraph, AGRAPH, name.toLocal8Bit().data(), value.toLocal8Bit().data());
}

void QGVSubGraph::updateLayout()
{
    prepareGeometryChange();
    _height = 50;
    _width = 50;

    pointf size = GD_bb(_sgraph).UR;
    _height = size.x*DotDefaultDPI;
    _width =size.y*DotDefaultDPI;

    setPos(size.x, size.y);

}
