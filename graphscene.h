#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>
#include "node.h"
#include "edge.h"
#include "textvalue.h"
#include <QGraphicsSceneMouseEvent>

struct myEdge
{
    int to,ref;
    int f,cap;
    Edge* edge;
};

class GraphScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraphScene(QObject *parent = 0);
    enum CursorMode { InsertNode, InsertEdge, MoveItem, DeleteItem};
    void setStart();
    void setEnd();
    void makeStep();
    bool dinic_bfs();
    int dinic_dfs(int u,int f);
    int maxFlow();
    void addEdge(int s, int t, int cap, Edge *e);
    void deleteEdge(int s, int t, Edge *e);
    void resetSteps();
    enum {init,BFSstep,DFSstep};

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

    //algoritmus
    int src,dest;
    int maxnodes;
    int* dist,*queue,*work;
    std::vector<myEdge>* adj;
    std::vector<myEdge> coloring_edge;

    // promenne pro krokovani..
    int state;

};

#endif // GRAPHSCENE_H
