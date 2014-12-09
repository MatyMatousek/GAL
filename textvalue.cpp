#include "textvalue.h"
#include <QDebug>
#include <QKeyEvent>


TextValue::TextValue(QGraphicsItem *parent) :
    QGraphicsTextItem(parent)
{
    setPlainText("0/0");
    setFlowValue(0);
    setCapacityValue(0);
}

void TextValue::focusInEvent(QFocusEvent *event)
{
    setPlainText(QString("%1").arg(capacity));
    QGraphicsTextItem::focusInEvent(event);
}

void TextValue::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    QString text = toPlainText();
    bool ok;
    int tmp = text.toInt(&ok, 10);
    if (tmp)
    {
        //setFlowValue(tmp);
        setCapacityValue(tmp);
    }
    else
    {
        setCapacityValue(0);
        setFlowValue(0);
        setPlainText("0/0");
    }
    setFlowValue(0);
    updateText();
    QGraphicsTextItem::focusOutEvent(event);
    //qDebug() << getFlowValue();
}

void TextValue::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

void TextValue::keyPressEvent(QKeyEvent *event)
{

    QGraphicsTextItem::keyPressEvent(event);
}

void TextValue::updateText()
{
    setPlainText(QString("%1/%2").arg(flow).arg(capacity));
}
