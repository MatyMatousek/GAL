#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsLineItem>
#include <QGraphicsTextItem>

#include "node.h"
#include "textvalue.h"

class Edge : public QGraphicsLineItem
{
public:
    Edge(Node *sourceNode, Node *destinationNode, QGraphicsItem *parent = 0);

    enum { Type = UserType + 2 };
    int type() const { return Type; }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setColor(const QColor &Color) { color = Color; }
    Node *sourceNode() const { return source; }
    Node *destinationNode() const { return destination; }
    void updatePosition();
    void setTextObject(TextValue *object);
    TextValue *getTextObject() const { return textV; }
    void setFlow(int flow);
    void setCapacite(int capacite);
    int getFlow();
    int getCapacite();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
    TextValue *textV;
    Node *source;
    Node *destination;
    QColor color;
    QPolygonF arrowHead;
};

#endif // EDGE_H
