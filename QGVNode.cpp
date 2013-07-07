#include <QGVNode.h>
#include <QGVScene.h>
#include <QDebug>
#include <QPainter>


QGVNode::QGVNode(const QString &label, QGVScene *scene): _scene(scene)
{
    _node = agnode(_scene->_graph, NULL, TRUE);
    Q_ASSERT(_node);
    if(_node == NULL)
        qWarning()<<"Invalid node :"<<label;

    _scene->_nodes.append(this);
    _scene->addItem(this);

    setAttribute("label", label);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    //setFlag(QGraphicsItem::ItemIsMovable, true);
    //setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
}

QGVNode::QGVNode(Agnode_t* node, QGVScene *scene): _node(node), _scene(scene)
{
    updateLayout();
}

QGVNode::~QGVNode()
{
    _scene->removeItem(this);
    _scene->_nodes.removeOne(this);
    //agdelnode(_scene->_graph, _node);
}

QString QGVNode::name() const
{
    return QString::fromLocal8Bit(ND_label(_node)->text);
}

QRectF QGVNode::boundingRect() const
{
    return QRectF(0,0, _width, _height);
}

void QGVNode::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();

    painter->setPen(_pen);

    if(isSelected())
    {
        QBrush tbrush(_brush);
        tbrush.setColor(tbrush.color().darker(120));
        painter->setBrush(tbrush);
    }
    else
        painter->setBrush(_brush);

    painter->drawPath(_path);
    const QRectF rect = boundingRect();
    //painter->drawRect(rect);
    painter->drawText(rect.adjusted(2,2,-2,-2), Qt::AlignCenter , QGVNode::name());
    painter->restore();
}

void QGVNode::setAttribute(const QString &name, const QString &value)
{
    agsafeset(_node, name.toLocal8Bit().data(), value.toLocal8Bit().data(), "");
}

void QGVNode::updateLayout()
{
    prepareGeometryChange();
    _width = ND_width(_node)*DotDefaultDPI;
    _height = ND_height(_node)*DotDefaultDPI;

    //Node Position (center)
    boxf bb = GD_bb(_scene->_graph);
    qreal x = ND_coord(_node).x - _width/2;
    qreal y = bb.UR.y - ND_coord(_node).y - _height/2;
    setPos(x, y);

    //Node on top
    setZValue(1);

    //Node path
    _path = makeShape(_node);
    _pen.setWidth(1);

    const char* style = agget(_node, "style");
    if(style && strcmp(style, "filled") == 0)
    {
        _brush.setStyle(Qt::SolidPattern);
        _brush.setColor(QString(agget(_node, "fillcolor")));
    }

    setToolTip(agget(_node, "tooltip"));
    //qDebug()<<QGVNode::name()<<_width<<_height<<pos();
}

QPainterPath QGVNode::makeShape(Agnode_t* node) const
{
    QPainterPath path;

    const char* name = ND_shape(node)->name;

    if ((strcmp(name, "rectangle") == 0) ||
        (strcmp(name, "box") == 0) ||
        (strcmp(name, "hexagon") == 0) ||
        (strcmp(name, "polygon") == 0) ||
        (strcmp(name, "diamond") == 0))
    {
        QPolygonF polygon = makeShapeHelper(node);
        polygon.append(polygon[0]);
        path.addPolygon(polygon);
    }
    else if ((strcmp(name, "ellipse") == 0) ||
            (strcmp(name, "circle") == 0))
    {
        QPolygonF polygon = makeShapeHelper(node);
        path.addEllipse(QRectF(polygon[0], polygon[1]));
    }
    else
    {
        qWarning("unsupported shape %s", name);
    }

    return path;
}

QPolygonF QGVNode::makeShapeHelper(Agnode_t* node) const
{
    const polygon_t* poly = (polygon_t*) ND_shape_info(node);

    if (poly->peripheries != 1)
        qWarning("unsupported number of peripheries %d", poly->peripheries);

    const int sides = poly->sides;
    const pointf* vertices = poly->vertices;

    QPolygonF polygon;
    for (int side = 0; side < sides; side++)
        polygon.append(QPointF(vertices[side].x + _width/2, vertices[side].y + _height/2));
    return polygon;
}

