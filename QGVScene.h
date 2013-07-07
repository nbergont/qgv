#ifndef QGVSCENE_H
#define QGVSCENE_H

#include <QGVCore.h>
#include <QGraphicsScene>
#include <QGVNode.h>
#include <QGVEdge.h>
#include <QGVSubGraph.h>

/**
 * @brief GraphViz interactive scene
 *
 */
class QGVScene : public QGraphicsScene
{
    Q_OBJECT
public:

    explicit QGVScene(const QString &name, QObject *parent = 0);
    ~QGVScene();

    void setGraphAttribute(const QString &name, const QString &value);
    void setNodeAttribute(const QString &name, const QString &value);
    void setEdgeAttribute(const QString &name, const QString &value);

    QGVNode* addNode(const QString& label);
    QGVEdge* addEdge(QGVNode* source, QGVNode* target, const QString& label=QString());
    QGVSubGraph* addSubGraph(const QString& name, bool cluster=true);

    void setRootNode(QGVNode *node);

    void loadLayout(const QString &text);
    void applyLayout();
    void clear();


signals:
    void nodeContextMenu(QGVNode* node);
    void nodeDoubleClick(QGVNode* node);

    void edgeContextMenu(QGVEdge* edge);
    void edgeDoubleClick(QGVEdge* edge);

    void subGraphContextMenu(QGVSubGraph* graph);
    void subGraphDoubleClick(QGVSubGraph* graph);

    void graphContextMenuEvent();
    
public slots:

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent * contextMenuEvent);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void drawBackground(QPainter * painter, const QRectF & rect);
private:
    friend class QGVNode;
    friend class QGVEdge;
    friend class QGVSubGraph;

    GVC_t *_context;
    Agraph_t *_graph;
    //QFont _font;

    QList<QGVNode*> _nodes;
    QList<QGVEdge*> _edges;
    QList<QGVSubGraph*> _subGraphs;
};

#endif // QGVSCENE_H
