#ifndef COOLPEN_H
#define COOLPEN_H

#include <QColor>
#include <QPoint>
#include <QRect>
#include <QImage>

class CoolPen
{
public:
    enum class Shape {
        Circle,
        Rectangle,
        Ellipse,
        Triangle
    };

    Shape shape;
    QColor color;
    int width;
    QPoint lastPoint;
    bool painting = false;

    CoolPen();
    ~CoolPen();


    void setColor(const QColor &newColor);
    void setWidth(int newWidth);
    QRect* drawLineTo(const QPoint &endPoint, QImage &image);
private:
    void refreshPen(QPainter &painter);
};

#endif // COOLPEN_H
