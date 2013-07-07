#ifndef QGVSUBGRAPH_H
#define QGVSUBGRAPH_H

#include <QGraphicsItem>
#include <gvc.h>
#include <cgraph.h>

class QGVNode;
class QGVEdge;
class QGVScene;

class QGVSubGraph : public QGraphicsItem
{
public:
    explicit QGVSubGraph(const QString &name, QGVScene *scene);
    QGVSubGraph(Agraph_t* subGraph, QGVScene *scene);
    ~QGVSubGraph();

    QString name() const;

    QGVNode* addNode(const QString& name);
    QGVEdge* addEdge(QGVNode* source, QGVNode* target, const QString& label=QString());

    QRectF boundingRect() const;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    void setAttribute(const QString &name, const QString &value);
    void updateLayout();

    enum { Type = UserType + 3 };
    int type() const
    {
        return Type;
    }

private:
    double _height, _width;

    QGVScene *_scene;
    Agraph_t *_sgraph;
};

#endif // QGVSUBGRAPH_H
