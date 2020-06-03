#ifndef HANDWRITER_H
#define HANDWRITER_H

#include <QtGui>
#include <QtWidgets>

class Handwriter : public QWidget
{
    Q_OBJECT
public:
   Handwriter(QWidget *parent = nullptr);
   ~Handwriter();
   bool isModified();
public slots:
   void clearImage();
   void saveImage();
protected:
   void mousePressEvent(QMouseEvent *event) override;
   void mouseMoveEvent(QMouseEvent *event) override;
   void mouseReleaseEvent(QMouseEvent *event) override;
   void paintEvent(QPaintEvent *event) override;
private:
    void drawLineTo(const QPoint& endPoint);
    bool modified;
    bool activate;
    QImage* image;
    QPoint lastPoint;
};

#endif // HANDWRITER_H
