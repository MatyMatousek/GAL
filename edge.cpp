#include "edge.h"
#include <math.h>
#include <QDebug>

#include <QPen>
#include <QPainter>

Edge::Edge(Node *sourceNode, Node *destinationNode, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    destination = destinationNode;
    source = sourceNode;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    color = Qt::black;
    setZValue(-1000.0);
    setPen(QPen(color, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

QRectF Edge::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

QPainterPath Edge::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}

void Edge::updatePosition()
{
    QLineF line(mapFromItem(source, 0, 0), mapFromItem(destination, 0, 0));
    this->setLine(line);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *)
{
    if (source->collidesWithItem(destination))
        return;

    QPen myPen = pen();
    myPen.setColor(color);
    qreal arrowSize = 20;
    painter->setPen(myPen);
    painter->setBrush(color);

    QLineF centerLine(source->pos(), destination->pos());
    centerLine.setLength(centerLine.length()-20.0);

    setLine(QLineF(centerLine.p2(), source->pos()));


    double angle = ::acos(line().dx() / line().length());
    if (line().dy() >= 0)
        angle = (3.14 * 2) - angle;

        QPointF arrowP1 = line().p1() + QPointF(sin(angle + 3.14 / 3) * arrowSize,
                                        cos(angle + 3.14 / 3) * arrowSize);
        QPointF arrowP2 = line().p1() + QPointF(sin(angle + 3.14 - 3.14 / 3) * arrowSize,
                                        cos(angle + 3.14 - 3.14 / 3) * arrowSize);

        arrowHead.clear();
        arrowHead << line().p1() << arrowP1 << arrowP2;
        painter->drawLine(line());
        painter->drawPolygon(arrowHead);
        if (isSelected()) {
            painter->setPen(QPen(color, 1, Qt::DashLine));
        QLineF myLine = line();
        myLine.translate(0, 4.0);
        painter->drawLine(myLine);
        myLine.translate(0,-8.0);
        painter->drawLine(myLine);
    }
}
