#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "handwriter.h"
#include "neuralnetwork.h"
#include <QtGui>
#include <QtWidgets>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QLabel* label;
    QLabel* errorLabel;
public slots:
    void setErrorLabel(const QString& text);
    void setLabel(const QString& text);
private slots:
    void trainModel();
    void imageRecognition();
private:
    Handwriter* handwriter;
    NeuralNetwork* net;
    QPushButton* clearButton;
    QPushButton* doneButton;
    QPushButton* trainButton;
};
#endif // MAINWINDOW_H
