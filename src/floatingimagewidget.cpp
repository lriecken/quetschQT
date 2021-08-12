#include "floatingimagewidget.h"

FloatingImageWidget::FloatingImageWidget(QWidget *parent) : QWidget(parent)
{
    fitObject();
}

void FloatingImageWidget::paintEvent(QPaintEvent *) {
    size();
    updateDimensions();
    drawImage();
}

void FloatingImageWidget::mouseDoubleClickEvent(QMouseEvent *event) {

}

void FloatingImageWidget::mousePressEvent(QMouseEvent *event) {
    mouseDownAtX = event->pos().x();
    mouseDownAtY = event->pos().y();
    mouseDown = true;
}

void FloatingImageWidget::mouseReleaseEvent(QMouseEvent *event) {
    mouseDown = false;
}

void FloatingImageWidget::mouseMoveEvent(QMouseEvent *event) {
    if (mouseDown) {
        applyMove(mouseDownAtX - event->pos().x(), mouseDownAtY - event->pos().y());
        mouseDownAtX = event->pos().x();
        mouseDownAtY = event->pos().y();
        repaint();
    }
}

void FloatingImageWidget::wheelEvent(QWheelEvent *event) {
    auto angle = event->angleDelta();
    applyScale(event->position().x(), event->position().y(), 1 - (float)angle.y() / 100.0);
    repaint();
}

void FloatingImageWidget::fitObject() {
    objAspectRatio = objWidth / objHeight;
    if (objAspectRatio < aspectRatio) {
        objX1 = centerX - ((float)height() * objAspectRatio) / 2;;
        objX2 = centerX + ((float)height() * objAspectRatio) / 2;;
        objY1 = 0;
        objY2 = height();
    } else {
        objX1 = 0;
        objX2 = width();
        objY1 = centerY - (float)width() / objAspectRatio / 2;
        objY2 = centerY + (float)width() / objAspectRatio / 2;;
    }
}

void FloatingImageWidget::setImage(QImage image) {
    this->image = image;
    this->objWidth = image.width();
    this->objHeight = image.height();
    fitObject();
    repaint();
}

void FloatingImageWidget::updateImage(QImage image) {
    this->image = image;
    this->objWidth = image.width();
    this->objHeight = image.height();
    repaint();
}

void FloatingImageWidget::applyScale(int x, int y, float delta) {
    qDebug() << delta;
    objX1 = (objX1 - x) * delta + x;
    objX2 = (objX2 - x) * delta + x;
    objY1 = (objY1 - y) * delta + y;
    objY2 = (objY2 - y) * delta + y;

}

void FloatingImageWidget::applyMove(int x, int y) {
    objX1 -= x;
    objX2 -= x;
    objY1 -= y;
    objY2 -= y;
}

void FloatingImageWidget::drawImage() {
    QPainter *qp = new QPainter();
    qp->begin(this);
    qp->drawImage(QRect(QPoint(objX1, objY1), QPoint(objX2, objY2)), image);

    qp->end();
}

void FloatingImageWidget::updateDimensions() {
    this->centerX = width() / 2;
    this->centerY = height() / 2;
    this->aspectRatio = (float)width() / (float)height();

}
