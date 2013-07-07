#ifndef QGVEDGE_H
#define QGVEDGE_H

#include <QGraphicsItem>
#include <QPen>
#include <gvc.h>
#include <cgraph.h>

class QGVNode;
class QGVScene;

class QGVEdge : public QGraphicsItem
{
public:
    QGVEdge(QGVNode *source, QGVNode *target, const QString &label, QGVScene *scene);
    QGVEdge(Agedge_t *edge, QGVScene *scene);
    ~QGVEdge();

    QString label() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;

    void setLabel(const QString &label);

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    void setAttribute(const QString &name, const QString &value);
    void updateLayout();

    enum { Type = UserType + 3 };
    int type() const
    {
        return Type;
    }

private:
    friend class QGVSubGraph;

    QGVScene *_scene;
    Agedge_t* _edge;

    QGVNode *_source;
    QGVNode *_target;

    QPainterPath _path;
    QPen _pen;
    QPolygonF _arrow;
};

#endif // QGVEDGE_H
