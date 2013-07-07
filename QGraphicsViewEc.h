#ifndef QGRAPHICSVIEWEC_H
#define QGRAPHICSVIEWEC_H

#include <QGraphicsView>

/**
 * @brief Graphics view with wheel zoom
 *
 */
class QGraphicsViewEc : public QGraphicsView
{
    Q_OBJECT
public:
    QGraphicsViewEc(QWidget *parent = 0);

protected:
    virtual void wheelEvent(QWheelEvent* event);
};

#endif // QGRAPHICSVIEWEC_H
