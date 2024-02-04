#include "paintview.h"
#include <QMouseEvent>
#include <QColor>
#include <QPainter>




PaintView::PaintView(QWidget *parent): QWidget{parent}
{
    setAttribute(Qt::WA_StaticContents);
    modified = false;
    painting = false;
    myPenWidth = 1;
    myPenColor = Qt::blue;
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

void PaintView::setPenColor(const QColor &newColor)
{
    myPenColor = newColor;
}

void PaintView::setPenWidth(int newWidth)
{
    myPenWidth = newWidth;
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
        lastPoint = event->pos();
        painting = true;
    }
}

void PaintView::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton && painting)
    {
        drawLineTo(event->pos());
    }
}

void PaintView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        drawLineTo(event->pos());
        painting = false;
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

void PaintView::drawLineTo(const QPoint &endPoint)
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint,endPoint);

    modified = true;

    int rad = (myPenWidth / 2) + 2;
    QRect updateRect = QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad);
    update(updateRect);

    lastPoint = endPoint;
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

void PaintView::print(){

// #if QT_CONFIG(

// #endif

}

// QPrinter printer(QPrinter::HighResolution);
// QPrintDialog printerDialog(&printer, this);
// if(printDialog.exec() == QDialog::Accepted)
// {
//     QPaintyer painter(&printer);
//     QSize size = image.size();
//     size.scale(rect.size(), Qt::KeepAspectRatio);
//     painter.setViewport(rect.x(), rect.y(), size.width(), size.height());
//     painter.setWindow(image.rect());
//     painter.drawImage(0,0, image);
// }


