#ifndef PAINTVIEW_H
#define PAINTVIEW_H

#include <QColor>
#include <QWidget>
#include <QImage>
#include <QPoint>

#include "coolpen.h"


class PaintView : public QWidget
{
    Q_OBJECT
public:
    CoolPen *pen;

    explicit PaintView(QWidget *parent = nullptr);
    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    bool isModified() const {return modified; }
    QColor penColor() const { return pen->color; }
    int penWidth() const {return pen->width;}
public slots:
    void clearImage();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:

    void resizeImage (QImage *image, const QSize &newSize);
    bool modified;
    QImage image;

signals:
};

#endif // PAINTVIEW_H
