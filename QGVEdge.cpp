#include <QGVEdge.h>
#include <QGVScene.h>
#include <QDebug>
#include <QPainter>


QGVEdge::QGVEdge(QGVNode *source, QGVNode *target, const QString &label, QGVScene *scene) : _scene(scene), _source(source), _target(target)
{
    //_edge = agidedge(_scene->_graph, _source->_node, _target->_node, _scene->_edges.count()+1, TRUE);
    _edge = agedge(_scene->_graph, _source->_node, _target->_node, NULL, TRUE);
    Q_ASSERT(_edge);
    if(_edge == NULL)
    {
        qWarning()<<"Invalid egde :"<<label;
        //_edge = agedge(_scene->_graph, _source->_node, _target->_node, NULL, FALSE);
    }

    //_source->_edges.append(this);
    //_target->_edges.append(this);

    _scene->_edges.append(this);
    _scene->addItem(this);

    setLabel(label);

    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

QGVEdge::QGVEdge(Agedge_t *edge, QGVScene *scene) :  _edge(edge), _scene(scene)
{
    updateLayout();
}

QGVEdge::~QGVEdge()
{
    _source->_edges.removeOne(this);
    _target->_edges.removeOne(this);

    _scene->removeItem(this);
    _scene->_edges.removeOne(this);
    //agdeledget(_scene->_graph, _edge);
}

QString QGVEdge::label() const
{
    return QString::fromLocal8Bit(ED_label(_edge)->text);
}

QRectF QGVEdge::boundingRect() const
{
    return _path.boundingRect();
}

QPainterPath QGVEdge::shape() const
{
    QPainterPathStroker ps;
    ps.setCapStyle(_pen.capStyle());
    ps.setWidth(_pen.widthF() + 10);
    ps.setJoinStyle(_pen.joinStyle());
    ps.setMiterLimit(_pen.miterLimit());
    return ps.createStroke(_path);
}

void QGVEdge::setLabel(const QString &label)
{
    setAttribute("xlabel", label);
}

void QGVEdge::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();

    if(isSelected())
        painter->setPen(QPen(Qt::SolidPattern, 1, Qt::DashLine));
    else
        painter->setPen(_pen);


    painter->drawPath(_path);


    textlabel_t *xlabel = ED_xlabel(_edge);
    if(xlabel)
        painter->drawText(xlabel->pos.x - xlabel->dimen.x/2, GD_bb(_scene->_graph).UR.y - xlabel->pos.y + 4, xlabel->text);

    painter->setBrush(Qt::SolidPattern);
    painter->drawPolygon(_arrow);
    painter->restore();
}

void QGVEdge::setAttribute(const QString &name, const QString &value)
{
    agsafeset(_edge, name.toLocal8Bit().data(), value.toLocal8Bit().data(), "");
}

void QGVEdge::updateLayout()
{
    prepareGeometryChange();

    _path = QPainterPath();

    const splines* spl = ED_spl(_edge);


    if((spl->list != 0) && (spl->list->size%3 == 1))
    {
        qreal ymax = GD_bb(_scene->_graph).UR.y;
        //If there is a starting point, draw a line from it to the first curve point
        if(spl->list->sflag)
        {
            _path.moveTo(spl->list->sp.x, (ymax - spl->list->sp.y));
            _path.lineTo(spl->list->list[0].x, (ymax - spl->list->list[0].y));
        }
        else
            _path.moveTo(spl->list->list[0].x, (ymax - spl->list->list[0].y));

        //Loop over the curve points
        for(int i=1; i<spl->list->size; i+=3)
            _path.cubicTo(spl->list->list[i].x,(ymax - spl->list->list[i].y), spl->list->list[i+1].x,
                    (ymax - spl->list->list[i+1].y),
                    spl->list->list[i+2].x,
                    (ymax - spl->list->list[i+2].y));

        //If there is an ending point, draw a line to it
        if(spl->list->eflag)
            _path.lineTo(spl->list->ep.x, (ymax - spl->list->ep.y));


        //Calcul de la fleche
        QLineF line(spl->list->list[spl->list->size-1].x, (ymax - spl->list->list[spl->list->size-1].y), spl->list->ep.x, (ymax - spl->list->ep.y));

        QLineF n(line.normalVector());
        QPointF o(n.dx() / 3.0, n.dy() / 3.0);

        QPolygonF polygon;
        polygon.append(line.p1() + o);
        polygon.append(line.p2());
        polygon.append(line.p1() - o);
        _arrow = polygon;
    }

    //ED_edge_type(_edge)
    _pen.setWidth(1);

    //qDebug()<<_name<<_path;
}
