#include "graphscene.h"
#include "edge.h"
#include "node.h"

GraphScene::GraphScene(QObject *parent) :
    QGraphicsScene(parent)
{
    line = 0;
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton)
    {
        line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(),
                                    mouseEvent->scenePos()));
        line->setPen(QPen(myLineColor, 2));
        addItem(line);
        //return;
    }
    else
    {
        Node *item;
        item = new Node();

        addItem(item);
        item->setPos(mouseEvent->scenePos());
        emit nodeInserted(item);
    }


    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != 0) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
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
//! [11] //! [12]

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first() != endItems.first()) {
            Node *startItem = qgraphicsitem_cast<Node *>(startItems.first());
            Node *endItem = qgraphicsitem_cast<Node *>(endItems.first());
            Edge *edge = new Edge(startItem, endItem);
            //edge->setColor(myLineColor);
            startItem->addEdge(edge);
            endItem->addEdge(edge);
            edge->setZValue(-1000.0);
            addItem(edge);
            edge->updatePosition();
        }
    }
//! [12] //! [13]
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
