#ifndef PAINTVIEW_H
#define PAINTVIEW_H

#include <QColor>
#include <QWidget>
#include <QImage>
#include <QPoint>


class PaintView : public QWidget
{
    Q_OBJECT
public:
    explicit PaintView(QWidget *parent = nullptr);
    bool openImage(const QString &fileName);
    bool saveImage(const QString &fileName, const char *fileFormat);
    void setPenColor(const QColor &newColor);
    void setPenWidth(int newWidth);


    bool isModified() const {return modified; }
    QColor penColor() const { return myPenColor; }
    int penWidth() const {return myPenWidth;}
public slots:
    void clearImage();
    void print();


protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawLineTo(const QPoint &endPoint);
    void resizeImage (QImage *image, const QSize &newSize);
    bool modified;
    bool painting;
    QColor myPenColor;
    int myPenWidth;
    QImage image;
    QPoint lastPoint;


signals:
};

#endif // PAINTVIEW_H
