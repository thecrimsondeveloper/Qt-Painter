#include "CoolPen.h"
#include <QPainter>


CoolPen::CoolPen()
{
    width = 10;
    color = Qt::blue;
}

void CoolPen::setColor(const QColor &newColor)
{
    color = newColor;
}

void CoolPen::setWidth(int newWidth)
{
    width = newWidth;
}

QRect* CoolPen::drawLineTo(const QPoint &endPoint, QImage &image)
{
    QPainter painter(&image);
    refreshPen(painter);


    painter.drawLine(lastPoint,endPoint);
    int rad = (width / 2) + 2;
    QRect* updateRect = new QRect(lastPoint, endPoint);
    updateRect->adjust(-rad, -rad, +rad, +rad);

    lastPoint = endPoint;

    return updateRect;
}

void CoolPen::refreshPen(QPainter &painter)
{
    QPen *pen = new QPen();

    //set shape to square
    if(shape == Shape::Rectangle)
    {
        pen->setCapStyle(Qt::SquareCap);
    } else {
        pen->setCapStyle(Qt::RoundCap);
    }
    pen->setWidth(width);
    pen->setColor(color);

    painter.setPen(*pen);
}
