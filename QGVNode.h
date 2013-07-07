#ifndef QGVNODE_H
#define QGVNODE_H

#include <QGraphicsItem>
#include <QPen>
#include <gvc.h>
#include <cgraph.h>

class QGVEdge;
class QGVScene;

class QGVNode : public QGraphicsItem
{
public:
    QGVNode(const QString &label, QGVScene *scene);
    QGVNode(Agnode_t* node, QGVScene *scene);
    ~QGVNode();

    QString name() const;

    QRectF boundingRect() const;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    void setAttribute(const QString &name, const QString &value);
    void updateLayout();

    enum { Type = UserType + 2 };
    int type() const
    {
        return Type;
    }

private:

    QPainterPath makeShape(Agnode_t* node) const;
    QPolygonF makeShapeHelper(Agnode_t* node) const;

    friend class QGVEdge;
    friend class QGVSubGraph;

    double _height, _width;
    QPainterPath _path;
    QPen _pen;
    QBrush _brush;
    QGVScene *_scene;

    Agnode_t* _node;
    QList<QGVEdge*> _edges;
};


#endif // QGVNODE_H
