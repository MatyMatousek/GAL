#include "node.h"
#include "edge.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>
#include <QDebug>

Node::Node(QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent)
{
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

void Node::removeEdges()
{
    foreach (Edge *edge, edges) {
        edge->sourceNode()->removeEdge(edge);
        edge->destinationNode()->removeEdge(edge);
        scene()->removeItem(edge);
        delete edge;
    }
}

void Node::addEdge(Edge *edge)
{
    edges.append(edge);
}

QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -20 - adjust, -20 - adjust, 40 + adjust, 40 + adjust);
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (Edge *edge, edges) {
            edge->updatePosition();
        }
    }

    return value;
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-20, -20, 40, 40);
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    QBrush fillNode(Qt::white, Qt::SolidPattern);
    if (isSelected()) {
        QRect rec(-20, -20, 40, 40);
        painter->setPen(QPen(Qt::black, 1, Qt::DashLine));
        painter->drawRect(rec);
    }
    painter->setBrush(fillNode);

    painter->setPen(QPen(Qt::black, 1));
    painter->drawEllipse(-20, -20, 40, 40);
}
