#include "graphscene.h"
#include "edge.h"
#include "node.h"
#include <QDebug>
#include <QTextCursor>
#include <iostream>
#include <ogdf/basic/Graph.h>
#include <ogdf/energybased/SpringEmbedderKK.h>

using namespace ogdf;

GraphScene::GraphScene(QObject *parent) :
    QGraphicsScene(parent)
{
    line = 0;
    cMode = MoveItem;
    nodeValue = 0;
    start = NULL;
    end = NULL;

    maxnodes = 100;
    dist = new int[maxnodes];
    queue = new int[maxnodes];
    work = new int[maxnodes];
    adj = new std::vector<myEdge>[maxnodes];
    src = dest = -1;

    //promenne pro krokovani
    state = init;
    bottleneck = NULL;
    maxflow = 0;
    index = 0;
}

void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton)
    {
        return;
    }
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

            item->setName(QString("%1").arg(nodeValue));
            item->setValue( nodeValue );
            addItem(item);
            nodeValue++;
            //qDebug() << item;
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
                    removeItem(edge->getTextObject());
                    removeItem(edge);

                    deleteEdge(edge->sourceNode()->getValue(),edge->destinationNode()->getValue(),edge);

                    delete edge->getTextObject();
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
            startItems.first() != endItems.first() &&
            startItems.first()->type() == Node::Type &&
            endItems.first()->type() == Node::Type) {
            Node *startItem = qgraphicsitem_cast<Node *>(startItems.first());
            Node *endItem = qgraphicsitem_cast<Node *>(endItems.first());
            Edge *edge = new Edge(startItem, endItem);
            startItem->addEdge(edge);
            endItem->addEdge(edge);
            edge->setZValue(-1000.0);
            addItem(edge);

            TextValue *textItem = new TextValue();
            //textItem->setFont(QFont("Times", 10));
            textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
            textItem->setZValue(-1000.0);
            addItem(textItem);
            edge->setTextObject(textItem);
            //textItem->setPos(mouseEvent->scenePos());
            edge->updatePosition();
            //edge->setFlow(8);
            addEdge(startItem->getValue(),endItem->getValue(),edge->getCapacite(),edge);
        }
    }


    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GraphScene::setStart()
{
    if (!selectedItems().isEmpty() && selectedItems().first()->type() == Node::Type)
    {
        Node *node = qgraphicsitem_cast<Node *>(selectedItems().first());
        if (start != NULL)
        {
            start->setColor(Qt::white);
            start = NULL;
        }
        node->setColor(Qt::green);
        src = node->getValue();
        start = node;
        if (start == end)
            end = NULL;
    }
    update();
}

void GraphScene::setEnd()
{
    if (!selectedItems().isEmpty() && selectedItems().first()->type() == Node::Type)
    {
        Node *node = qgraphicsitem_cast<Node *>(selectedItems().first());
        if (end != NULL)
        {
            end->setColor(Qt::white);
            end = NULL;
        }
        node->setColor(Qt::red);
        dest = node->getValue();
        end = node;
        if (end == start)
            start = NULL;
    }
    update();
}

// prasarna jak hrom
void GraphScene::updateStructures()
{
    for ( int i = 0 ; i < maxnodes; i++ )
    {
        for ( int j = 0 ; j < (int)adj[i].size() ; j++)
        {
            adj[i][j].f = adj[i][j].edge->getFlow();
            adj[i][j].cap = adj[i][j].edge->getCapacite();
        }
    }
}


bool GraphScene::dinic_bfs()
{
  std::fill(dist, dist + maxnodes, -1);
  dist[src] = 0;
  int qt = 0;
  queue[qt++] = src;
  for (int qh = 0; qh < qt; qh++) {
    int u = queue[qh];
    for (int j = 0; j < (int) adj[u].size(); j++) {
      myEdge &e = adj[u][j];
      int v = e.to;
      if (dist[v] < 0 && e.f < e.cap) {
        dist[v] = dist[u] + 1;
        queue[qt++] = v;
      }
    }
  }
  return dist[dest] >= 0;
}

int GraphScene::dinic_dfs(int u, int f)
{
  if (u == dest)
  {
    return f;
  }
  for (int i = 0; i < (int) adj[u].size(); i++)
  {
    myEdge &e = adj[u][i];
    if (e.cap <= e.f) continue;
    int v = e.to;
    if (dist[v] == dist[u] + 1)
    {
      int df = dinic_dfs(v, std::min(f, e.cap - e.f));
      if (df > 0)
      {
        coloring_edge.push_back(adj[u][i]);
        e.f += df;
        e.edge->setFlow(e.f);
        //adj[v][e.ref ].f -= df ;
        //adj[v][e.ref ].edge->setFlow( adj[v][e.ref ].f );


        return df;
      }
    }

  }
  return 0;
}


int GraphScene::dinic_dfs2(int u, int f,int index)
{

  if (u == dest)
  {
    return f;
  }
  int temp = adj[u].size() - index;
  if ( index >= temp ) return -1;
  for (int i = index; i < temp; i++)
  {
    myEdge &e = adj[u][i];
    if (e.cap <= e.f) continue;
    int v = e.to;
    if (dist[v] == dist[u] + 1)
    {
      int df = dinic_dfs2(v, std::min(f, e.cap - e.f),index);
      if (df > 0)
      {
        if ( df == e.cap - e.f )
               bottleneck = e.edge;
        coloring_edge.push_back(adj[u][i]);
        e.f += df;
        e.edge->setFlow(e.f);
        return df;
      }
    }

  }
  return 0;
}

int GraphScene::maxFlow()
{
  resetSteps();
  maxflow = 0;
  if ( src == -1 || dest == -1 ) return -1;
  int result = 0;
  while (dinic_bfs())
  {
    coloring_edge.clear();
    while (int delta = dinic_dfs(src, INT_MAX))
    {
      result += delta;
      if ( delta != 0 )
      {
        for( std::vector<myEdge>::iterator it = coloring_edge.begin(); it != coloring_edge.end(); it++)
        {
                (*it).edge->setColor(Qt::red);
        }
      }
    }  
  }
  update();
  maxflow = result;
  return maxflow;
}

// Adds bidirectional edge
void GraphScene::addEdge(int s, int t, int cap, Edge *e){
  myEdge a = {t, (int)adj[t].size(), 0, cap,e};
  //myEdge b = {s, (int)adj[s].size(), 0, cap,e};
  adj[s].push_back(a);
  //adj[t].push_back(b);
  a.edge = e;
  //b.edge = e;
}

void GraphScene::deleteEdge(int s, int t, Edge *e)
{
    for ( std::vector<myEdge>::iterator it = adj[s].begin(); it != adj[s].end(); ++it)
    {
        if ( (*it).edge == e)
        {
            adj[s].erase(it);
            break;
        }
    }
    for ( std::vector<myEdge>::iterator it = adj[t].begin() ; it != adj[t].end() ; ++it)
    {
        if ( (*it).edge == e)
        {
            adj[t].erase(it);
            break;
        }

    }
}

void GraphScene::resetSteps()
{
    for ( int i = 0; i < maxnodes;i++)
    {
        for( std::vector<myEdge>::iterator it = adj[i].begin(); it < adj[i].end(); it++)
        {
            (*it).edge->setFlow(0);
            (*it).edge->setColor( Qt::black);
            (*it).f = 0;
        }
    }
    // nulovani krokovaciho algoritmu
    state = init;
    index = 0;
    update();
}

bool GraphScene::isInQueue(int *que, int max, int prvek)
{
    for ( int i = 0 ; i <= max; i++)
    {
        if ( que[i] == prvek )
            return true;
    }
    return false;
}

void GraphScene::makeStep()
{
    int qt = 0;
    int temp = 0;
    bottleneck = NULL;

    int delta;
    switch( state )
    {
        case init:
            coloring_edge.clear();
            std::fill(dist, dist + maxnodes, -1);
            dist[src] = 0;
            queue[qt++] = src;
            for ( int i = 0; i < maxnodes; i++)
            {
                for( std::vector<myEdge>::iterator it = adj[i].begin(); it != adj[i].end(); it++)
                {
                    if ( (*it).edge->getColor() != Qt::gray  )
                    {
                        (*it).edge->setColor(Qt::black);
                    }
                    if ( (*it).cap - (*it).f == 0 && (*it).edge->getColor() != Qt::gray )
                    {
                        (*it).edge->setColor ( Qt::gray);
                        temp++;
                    }
                }

            }
            if ( temp ) {update();return;}

            //BFS
            for (int qh = 0; qh < qt; qh++)
            {
              int u = queue[qh];
              for (int j = 0; j < (int) adj[u].size(); j++)
              {
                myEdge &e = adj[u][j];
                int v = e.to;
                if ( dist[v] < 0 && e.f < e.cap) // pridani do seznamu
                {
                  dist[v] = dist[u] + 1;
                  queue[qt++] = v;
                }
                if ( (dist[v] < 0 || isInQueue(queue,qt,v))  && e.f < e.cap )
                {
                    e.edge->setColor(Qt::green);
                }
              }
            }
            if (dist[dest] >= 0) // nalezen koncovy uzel
                state = DFSstep;
            break;

       case DFSstep:
          coloring_edge.clear();
          while ( (delta = dinic_dfs2(src, INT_MAX,index)) == 0);
          if ( delta == -1 ) { state = theend; break;}
          maxflow += delta;
          if ( !my_stack.empty())
          {
            //stackData &temp = my_stack.top();
            //temp.maxflow = maxflow;
          }
          std::cout << maxflow << std::endl;
          for ( int i = 0; i < maxnodes; i++)
          {
              for( std::vector<myEdge>::iterator it = adj[i].begin(); it != adj[i].end(); it++)
              {
                  if ( (*it).edge->getColor() != Qt::gray  )
                  {
                      (*it).edge->setColor(Qt::black);
                  }
                  if ( (*it).cap - (*it).f == 0 )
                      (*it).edge->setColor ( Qt::gray);
              }

          }
          if ( delta != 0 )
          {
            for( std::vector<myEdge>::iterator it = coloring_edge.begin(); it != coloring_edge.end(); it++)
            {
                (*it).edge->setColor(Qt::red);
            }
          }
          else
          {
              index++;
          }
          if ( bottleneck != NULL )
          {
              bottleneck->setColor(Qt::blue);
              bottleneck = NULL;
          }
          state = init;

        break;

    case theend:
        return;
    default:break;
    }
      update();

}


void GraphScene::pushStepOnStack()
{
    int pocitadylko = getNodeValue();
    stackData temp;
    /*for ( int i = 0; i < getNodeValue();i++)
    {
        if ( adj[i].size() != 0)
            pocitadylko++;
    }
*/
    temp.adj = new std::vector<myEdge>[pocitadylko];
    for ( int i = 0 ; i < pocitadylko;i++ )
    {
        for ( std::vector<myEdge>::iterator it = adj[i].begin() ; it != adj[i].end();it++ )
            temp.adj[i].push_back(  *it );
    }

    QColor** barvicky = new QColor*[pocitadylko];
    for ( int i = 0; i < pocitadylko ; i++)
    {
        barvicky[i] = new QColor[temp.adj[i].size()];
    }

    for ( int i = 0; i < pocitadylko; i++)
    {
        for ( int j = 0; j < (int) temp.adj[i].size(); j++)
        {
            barvicky[i][j] = temp.adj[i][j].edge->getColor();
        }
    }

    temp.edge_colors = barvicky;
    temp.bottleneck = bottleneck;
    temp.state = state;
    temp.maxflow = maxflow;
    temp.poc_prvku = pocitadylko;
    temp.index = index;
    my_stack.push(temp);

}


void GraphScene::popStepFromStack()
{
    if ( my_stack.empty() ) return;
    stackData &temp = my_stack.top();
    maxflow = temp.maxflow;
    state = temp.state;
    bottleneck = temp.bottleneck;
    index = temp.index;

    for ( int i= 0;i < temp.poc_prvku ;i++)
    {
        adj[i].clear();
        for( std::vector<myEdge>::iterator it = temp.adj[i].begin(); it != temp.adj[i].end(); it++)
        {
            adj[i].push_back( *it);
        }
        for ( int j = 0; j < (int) adj[i].size();j++)
        {
            adj[i][j].edge->setColor( temp.edge_colors[i][j]);
            adj[i][j].edge->setFlow( adj[i][j].f);
            adj[i][j].edge->setCapacite( adj[i][j].cap);
        }
        temp.adj[i].clear();
    }

    delete [] temp.adj;
    for ( int i =0; i < temp.poc_prvku ;i++)
        delete [] temp.edge_colors[i];
    delete [] temp.edge_colors;

    my_stack.pop();

}

void GraphScene::createGraph(QString fileName)
{
    Graph G;
    GraphAttributes GA(G, GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics | GraphAttributes::edgeDoubleWeight);
    if( !GA.readGML(G, const_cast<char *>(qPrintable(fileName))) )
    {
        qDebug() << "Could not load " << fileName;
        return;
    }
    SpringEmbedderKK KK;
    //KK.setDesLength(150.0);
    KK.call(GA);

    QList<Node *> nodeList;
    node v;
    Node *N;
    forall_nodes(v,G)
    {
        N = new Node();
        nodeList.append(N);
        N->setName(QString("%1").arg(nodeValue));
        N->setValue( nodeValue );
        addItem(N);
        nodeValue++;
        //qDebug() << GA.x(v) << GA.y(v);
        N->setPos(GA.x(v),GA.y(v));
    }
    edge e;
    Edge *E;
    forall_edges(e,G)
    {
        Node *startItem = nodeList.at(e->source()->index());
        Node *endItem = nodeList.at(e->target()->index());
        E = new Edge(startItem,endItem);

        startItem->addEdge(E);
        endItem->addEdge(E);
        E->setZValue(-1000.0);
        addItem(E);

        TextValue *textItem = new TextValue();
        textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
        textItem->setZValue(-1000.0);
        addItem(textItem);
        E->setTextObject(textItem);
        E->updatePosition();
        E->setCapacite(int(GA.doubleWeight(e)));
        addEdge(startItem->getValue(),endItem->getValue(),E->getCapacite(),E);
    }
}

void GraphScene::saveGraph(QString fileName)
{
    Graph G;
    GraphAttributes GA(G, GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics | GraphAttributes::edgeDoubleWeight);

    QHash<QString, node> Nodes;
    QList<Edge *> Edges;
    Node *nodeItem;
    foreach (QGraphicsItem *item, items())
    {
        if(item->type() == Node::Type)
        {
            nodeItem = qgraphicsitem_cast<Node *>(item);
            node v = G.newNode();
            GA.x(v) = nodeItem->pos().x();
            GA.y(v) = nodeItem->pos().y();
            Nodes.insert(nodeItem->getName(), v);
        }
        else if(item->type() == Edge::Type)
        {
            Edges.append(qgraphicsitem_cast<Edge *>(item));
        }
    }
    edge e;
    foreach (Edge *E, Edges)
    {
        e = G.newEdge(Nodes.value(E->sourceNode()->getName()), Nodes.value(E->destinationNode()->getName()));
        GA.doubleWeight(e) = E->getCapacite();
    }

    SpringEmbedderKK KK;
    KK.call(GA);
    GA.writeGML(const_cast<char *>(qPrintable(fileName)));
}
