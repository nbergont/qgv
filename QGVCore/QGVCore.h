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

#ifndef QGVCORE_H
#define QGVCORE_H

#include <QPointF>
#include <QPolygonF>
#include <QPainterPath>
#include <QColor>

//GraphViz headers
#include <gvc.h>
#include <cgraph.h>

const qreal DotDefaultDPI = 72.0;

/**
 * @brief GraphViz to GraphicsScene conversions
 *
 */
class QGVCore
{
public:
    static qreal graphHeight(Agraph_t *graph);
    static QPointF toPoint(pointf p, qreal gheight);
    static QPointF toPoint(point p, qreal gheight);
    static QPointF centerToOrigin(const QPointF &p, qreal width, qreal height);
    static QPolygonF toPolygon(const polygon_t* poly, qreal width, qreal height);

    static QPainterPath toPath(const char *type, const polygon_t *poly, qreal width, qreal height);
    static QPainterPath toPath(const splines* spl, qreal gheight);

    static Qt::BrushStyle toBrushStyle(const QString &style);
    static Qt::PenStyle toPenStyle(const QString &style);
    static QColor toColor(const QString &color);
};

#endif // QGVCORE_H
