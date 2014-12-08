#ifndef TEXTVALUE_H
#define TEXTVALUE_H

#include <QGraphicsTextItem>
#include <QPen>

class Edge;

class TextValue : public QGraphicsTextItem
{
public:
    enum { Type = UserType + 3 };
    TextValue(QGraphicsItem *parent = 0);

    int type() const { return Type; }
    int getFlowValue() const { return flow; }
    int getCapacityValue() const { return capacity; }
    void setFlowValue(const int Flow) { flow = Flow; }
    void setCapacityValue(const int Capacity) { capacity = Capacity; }
    void updateText();

protected:
    void focusOutEvent(QFocusEvent *event);
    void focusInEvent(QFocusEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    int flow, capacity;
};

#endif // TEXTVALUE_H
