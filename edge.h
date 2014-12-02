#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsLineItem>

#include "node.h"

class Edge : public QGraphicsLineItem
{
public:
    Edge(Node *sourceNode, Node *destinationNode, QGraphicsItem *parent = 0);

    enum { Type = UserType +2 };
    int type() const { return Type; }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setColor(const QColor &Color) { color = Color; }
    Node *sourceNode() const { return source; }
    Node *destinationNode() const { return destination; }

    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
    Node *source;
    Node *destination;
    QColor color;
    QPolygonF arrowHead;
};

#endif // EDGE_H
