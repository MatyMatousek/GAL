#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>
#include "node.h"
#include "edge.h"
#include "textvalue.h"
#include <QGraphicsSceneMouseEvent>

class GraphScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphScene(QObject *parent = 0);
    enum CursorMode { InsertNode, InsertEdge, MoveItem, DeleteItem};
    void setStart();
    void setEnd();

public slots:
    void setCursorMode(CursorMode mode);

signals:
    void change(int cap);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    bool isItemChange(int type);
    int nodeValue;

    Node *start;
    Node *end;
    QGraphicsLineItem *line;
    QColor myLineColor;
    CursorMode cMode;
};

#endif // GRAPHSCENE_H
