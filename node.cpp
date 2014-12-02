#include "node.h"
#include "edge.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>

Node::Node(QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent)
{
    setZValue(10);
    rectangle.setRect(-25.0, -25.0, 50.0, 50.0);

    setRect(rectangle);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void Node::removeEdge(Edge *edge)
{
    int index = edges.indexOf(edge);

    if (index != -1)
        edges.removeAt(index);
}
//! [1]

//! [2]
void Node::removeEdges()
{
    foreach (Edge *edge, edges) {
        edge->sourceNode()->removeEdge(edge);
        edge->destinationNode()->removeEdge(edge);
        scene()->removeItem(edge);
        delete edge;
    }
}
//! [2]

//! [3]
void Node::addEdge(Edge *edge)
{
    edges.append(edge);
}
//! [3]

//! [4]
/*QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}*/
//! [4]

/*
void Node::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}
*/

//! [6]
QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (Edge *edge, edges) {
            edge->updatePosition();
        }
    }

    return value;
}

//! [9]
/*QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}
//! [9]

//! [10]
void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-7, -7, 20, 20);

    QRadialGradient gradient(-3, -3, 10);

        gradient.setColorAt(0, Qt::yellow);
        gradient.setColorAt(1, Qt::darkYellow);

    painter->setBrush(gradient);

    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);
}*/

QPixmap Node::image() const
{
    QPixmap pixmap(50, 50);
    pixmap.fill( Qt::white );
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    QPainterPath path;
    QBrush fillbrush;
    fillbrush.setColor(Qt::blue);
    fillbrush.setStyle(Qt::SolidPattern);
    painter.setBrush(Qt::red);
    path.addEllipse(rectangle);
    painter.fillPath(path,fillbrush);
    painter.drawEllipse(rectangle);

    return pixmap;
}
