#ifndef FLOATINGIMAGEWIDGET_H
#define FLOATINGIMAGEWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QImage>
#include <QSharedPointer>
class FloatingImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FloatingImageWidget(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void fitObject();

    void setImage(QImage image);
    void updateImage(QImage image);
private:
    float centerX;
    float centerY;
    int mouseDownAtX;
    int mouseDownAtY;
    bool mouseDown = false;

    float objWidth = 320;
    float objHeight = 240;
    QImage image;

    float objX1;
    float objY1;
    float objX2;
    float objY2;
    float aspectRatio;
    float objAspectRatio;

    void applyScale(int x, int y, float delta);

    void applyMove(int x, int y);

    void drawImage();


    void updateDimensions();
signals:

};

#endif // FLOATINGIMAGEWIDGET_H
