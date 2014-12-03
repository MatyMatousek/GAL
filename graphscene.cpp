#include "graphscene.h"
#include "edge.h"
#include "node.h"
#include <QDebug>

GraphScene::GraphScene(QObject *parent) :
    QGraphicsScene(parent)
{
    line = 0;
    cMode = MoveItem;
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
        return;
    QList<QGraphicsItem *> deleteItems;
    switch (cMode)
    {
        case InsertEdge:
            line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                        mouseEvent->scenePos()));
            line->setPen(QPen(myLineColor, 2));
            addItem(line);
            break;

        case InsertNode:
            Node *item;
            item = new Node();

            addItem(item);
            item->setPos(mouseEvent->scenePos());
            break;

        case DeleteItem:
            deleteItems = items(mouseEvent->scenePos());
            if (deleteItems.count())
            {
                if (deleteItems.first()->type() == Edge::Type)
                {
                    Edge *edge = qgraphicsitem_cast<Edge *>(deleteItems.first());
                    edge->sourceNode()->removeEdge(edge);
                    edge->destinationNode()->removeEdge(edge);
                    removeItem(edge);
                    delete edge;
                }
                else
                {
                    Node *node = qgraphicsitem_cast<Node *>(deleteItems.first());
                    node->removeEdges();
                    removeItem(node);
                    delete node;
                }

            }
            break;

        default:;
    }
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphScene::setCursorMode(CursorMode mode)
{
    cMode = mode;
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != 0 && cMode == InsertEdge) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    }
    else if (cMode == MoveItem)
    {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != 0) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();
        removeItem(line);
        delete line;

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first() != endItems.first()) {
            Node *startItem = qgraphicsitem_cast<Node *>(startItems.first());
            Node *endItem = qgraphicsitem_cast<Node *>(endItems.first());
            Edge *edge = new Edge(startItem, endItem);
            startItem->addEdge(edge);
            endItem->addEdge(edge);
            edge->setZValue(-1000.0);
            addItem(edge);
            edge->updatePosition();
        }
    }
    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

bool GraphScene::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}
