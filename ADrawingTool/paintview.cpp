#include "paintview.h"
#include <QMouseEvent>
#include <QColor>
#include <QPainter>
#include <iostream>
using namespace std;




PaintView::PaintView(QWidget *parent): QWidget{parent}
{
    setAttribute(Qt::WA_StaticContents);
    modified = false;

    pen = new CoolPen();
    pen->color = Qt::blue;
    pen->width = 10;
    pen->painting = false;
    pen->shape = CoolPen::Shape::Circle;
}

bool PaintView::openImage(const QString &fileName)
{
    QImage loadedImage;
    if(loadedImage.load(fileName) == false)
    {
        return false;
    }

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}

bool PaintView::saveImage(const QString &fileName, const char *fileFormat)
{
    QImage visibleImage = image;

    resizeImage(&visibleImage, size());
    if(visibleImage.save(fileName, fileFormat))
    {
        modified = false;
        return true;
    } else
    {
        return false;
    }
}



void PaintView::clearImage()
{
    image.fill(qRgb(255,255,255));
    modified = true;
}

void PaintView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        pen->lastPoint = event->pos();
        pen->painting = true;
    }
}

void PaintView::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton && pen && pen->painting) {

        cout << "Mouse move event " << event->pos().x() << " " << event->pos().y() << endl;
        QRect* rect = pen->drawLineTo(event->pos(), image);
        modified = true;
        update(*rect);
    }
}


void PaintView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        QRect rect = *pen->drawLineTo(event->pos(), image);
        modified = true;
        pen->painting = false;
    }
}

void PaintView::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect dirtyRect = event->rect();
    painter.drawImage(dirtyRect, image, dirtyRect);
}

void PaintView::resizeEvent(QResizeEvent *event)
{
    if(width() > image.width() || height() > image.height())
    {
        int newWidth = qMax(width() + 128, image.width());
        int newHeight = qMax(height() + 128, image.height());

        resizeImage(&image, QSize(newWidth, newHeight));
        update();
    }

    QWidget::resizeEvent(event);
}



void PaintView::resizeImage(QImage *image, const QSize &newSize)
{
    if(image->size() == newSize) return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255,255,255));
    QPainter painter(&newImage);

    painter.drawImage(QPoint(0,0), *image);
    *image = newImage;
}



