#ifndef NODE_H
#define NODE_H

#include <QGraphicsPixmapItem>
#include <QList>

class Edge;

class Node : public QGraphicsEllipseItem
{
public:
    enum { Type = UserType + 15 };
    Node(QGraphicsItem *parent = 0);

    void removeEdge(Edge *edge);
    void removeEdges();
    QRectF polygon() const { return rectangle; }
    void addEdge(Edge *edge);
    QPixmap image() const;
    int type() const { return Type;}

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    QRectF rectangle;
    QList<Edge *> edges;
};

#endif // NODE_H
