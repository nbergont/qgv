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
    return new QGVNode(label, this);
}

QGVEdge *QGVScene::addEdge(QGVNode *source, QGVNode *target, const QString &label)
{
    return new QGVEdge(source, target, label, this);
}

QGVSubGraph *QGVScene::addSubGraph(const QString &name)
{
    return new QGVSubGraph(name, this);
}

void QGVScene::setRootNode(QGVNode *node)
{
    Q_ASSERT(_nodes.contains(node));
    agset(_graph, "root", node->name().toLocal8Bit().data());
}

void QGVScene::loadLayout(const QString &text)
{

    /*
    agsetfile("simple.gv");

    FILE* fp = fopen("simple.gv", "r");
    if(fp)
        _graph = agread(fp, NULL);

    qDebug("ok");
    */

    //_graph = agmemread(text.toLocal8Bit().data());

    if(gvLayout(_context, _graph, "dot") != 0)
    {
        qCritical()<<"Layout render error"<<agerrors()<<QString::fromLocal8Bit(aglasterr());
        return;
    }

    //Read nodes and edges
    for (Agnode_t* node = agfstnode(_graph); node != NULL; node = agnxtnode(_graph, node))
    {
        _nodes.append(new QGVNode(node, this));
        for (Agedge_t* edge = agfstout(_graph, node); edge != NULL; edge = agnxtout(_graph, edge))
            _edges.append(new QGVEdge(edge, this));
    }

}

void QGVScene::applyLayout()
{
    //gvFreeLayout(_context, _graph);
    if(gvLayout(_context, _graph, "dot") != 0)
    {
        qCritical()<<"Layout render error"<<agerrors()<<QString::fromLocal8Bit(aglasterr());
        return;
    }

    //Debug output
    gvRenderFilename(_context, _graph, "canon", "debug.dot");
    gvRenderFilename(_context, _graph, "png", "debug.png");

    foreach(QGVNode* node, _nodes)
        node->updateLayout();

    foreach(QGVEdge* edge, _edges)
        edge->updateLayout();

    foreach(QGVSubGraph* sgraph, _subGraphs)
        sgraph->updateLayout();

    update();
}

#include <QGraphicsSceneContextMenuEvent>
void QGVScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent)
{
    QGraphicsItem *item = itemAt(contextMenuEvent->scenePos());
    if(item)
    {
        item->setSelected(true);
        QGVNode *node = qgraphicsitem_cast<QGVNode*>(item);
        if(node)
            emit nodeContextMenu(node);

        QGVEdge *edge = qgraphicsitem_cast<QGVEdge*>(item);
        if(edge)
            emit edgeContextMenu(edge);
    }
    QGraphicsScene::contextMenuEvent(contextMenuEvent);
}

void QGVScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsItem *item = itemAt(mouseEvent->scenePos());
    if(item)
    {
        QGVNode *node = qgraphicsitem_cast<QGVNode*>(item);
        if(node)
            emit nodeDoubleClick(node);

        QGVEdge *edge = qgraphicsitem_cast<QGVEdge*>(item);
        if(edge)
            emit edgeDoubleClick(edge);
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
