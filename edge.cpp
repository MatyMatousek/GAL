#include "edge.h"
#include <math.h>

#include <QPen>
#include <QPainter>

Edge::Edge(Node *sourceNode, Node *destinationNode, QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    destination = destinationNode;
    source = sourceNode;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    color = Qt::black;
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
    setLine(line);
}
//! [3]

//! [4]
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
//! [4] //! [5]

    QLineF centerLine(source->pos(), destination->pos());
    QPolygonF endPolygon = destination->polygon();
    QPointF p1 = endPolygon.first() + destination->pos();
    QPointF p2;
    QPointF intersectPoint;
    QLineF polyLine;
    for (int i = 1; i < endPolygon.count(); ++i) {
    p2 = endPolygon.at(i) + destination->pos();
    polyLine = QLineF(p1, p2);
    QLineF::IntersectType intersectType =
        polyLine.intersect(centerLine, &intersectPoint);
    if (intersectType == QLineF::BoundedIntersection)
        break;
        p1 = p2;
    }

    setLine(QLineF(intersectPoint, source->pos()));
//! [5] //! [6]

    double angle = ::acos(line().dx() / line().length());
    if (line().dy() >= 0)
        angle = (3.14 * 2) - angle;

        QPointF arrowP1 = line().p1() + QPointF(sin(angle + 3.14 / 3) * arrowSize,
                                        cos(angle + 3.14 / 3) * arrowSize);
        QPointF arrowP2 = line().p1() + QPointF(sin(angle + 3.14 - 3.14 / 3) * arrowSize,
                                        cos(angle + 3.14 - 3.14 / 3) * arrowSize);

        arrowHead.clear();
        arrowHead << line().p1() << arrowP1 << arrowP2;
//! [6] //! [7]
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
