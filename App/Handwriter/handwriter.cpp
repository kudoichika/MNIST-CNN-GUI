#include "handwriter.h"

#include <QtGui>
#include <QtWidgets>
#include <iostream>

Handwriter::Handwriter(QWidget *parent) : QWidget(parent) {
    image = new QImage(QSize(280, 280), QImage::Format_RGB32);
    clearImage();
    activate = false;
}

bool Handwriter::isModified() {
    return modified;
}

void Handwriter::clearImage() {
    image->fill(Qt::black);
    update();
    modified = false;
}

void Handwriter::saveImage() {
    if (modified) {
        QImage resizedImage = image->scaled(28, 28, Qt::KeepAspectRatio);
        QTransform transform;
        transform.rotate(90);
        resizedImage = resizedImage.transformed(transform);
        resizedImage.mirrored(true, false).save(QString("query.pgm"), "PGM");
    }
}

void Handwriter::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        activate = true;
    }
}

void Handwriter::mouseMoveEvent(QMouseEvent *event) {
    if (activate) {
        drawLineTo(event->pos());
    }
}

void Handwriter::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && activate) {
        drawLineTo(event->pos());
        activate = false;
    }
}

void Handwriter::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QRect rect = this->rect();
    painter.drawImage(rect, *image, rect);
}

void Handwriter::drawLineTo(const QPoint& endPoint) {
    QPainter painter(image);
    painter.setPen(QPen(Qt::white, 30, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(lastPoint, endPoint);
    modified = true;
    update();
    lastPoint = endPoint;
}

Handwriter::~Handwriter() {
    delete image;
}
