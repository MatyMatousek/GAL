#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>

class Edge;

class Node : public QGraphicsItem
{
public:
    enum { Type = UserType + 15 };
    Node(QGraphicsItem *parent = 0);

    void removeEdge(Edge *edge);
    void removeEdges();
    QPolygonF polygon() const { return myPolygon; }
    void addEdge(Edge *edge);
    QPixmap image() const;
    int type() const { return Type;}

    QRectF boundingRect() const;
    QPainterPath shape() const;

protected:
    //void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QPolygonF myPolygon;
    QMenu *myContextMenu;
    QList<Edge *> edges;
};

#endif // NODE_H
