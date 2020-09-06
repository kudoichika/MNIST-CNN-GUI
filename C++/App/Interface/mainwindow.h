#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Handwriter/handwriter.h"
#include "Model/model.h"
#include <QtGui>
#include <QtWidgets>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QLabel* label;
public slots:
    void setLabel(const QString& text);
private slots:
    void imageRecognition();
private:
    Handwriter* handwriter;
    Model* model;
    QPushButton* clearButton;
    QPushButton* doneButton;
};
#endif // MAINWINDOW_H
