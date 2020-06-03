#include "mainwindow.h"

#include <QtGui>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    handwriter = new Handwriter();
    net = new NeuralNetwork(this);
    clearButton = new QPushButton("Clear");
    doneButton = new QPushButton("Interpret");
    QWidget::connect(clearButton, SIGNAL(clicked()), handwriter, SLOT(clearImage()));
    QWidget::connect(doneButton, SIGNAL(clicked()), handwriter, SLOT(saveImage()));
    QWidget::connect(doneButton, SIGNAL(clicked()), this, SLOT(imageRecognition()));
    QHBoxLayout* actionLayout = new QHBoxLayout();
    actionLayout->addWidget(clearButton);
    actionLayout->addWidget(doneButton);
    QVBoxLayout* leftLayout = new QVBoxLayout();
    leftLayout->addWidget(handwriter);
    leftLayout->addLayout(actionLayout);
    QVBoxLayout* rightLayout = new QVBoxLayout();
    label = new QLabel("Train Model");
    label->setAlignment(Qt::AlignHCenter);
    label->setStyleSheet("*{color:#7AFDFF;font-size:30px;font-weight:bold;}");
    trainButton = new QPushButton("Train Model");
    QWidget::connect(trainButton, SIGNAL(clicked()), this, SLOT(trainModel()));
    errorLabel = new QLabel("Status");
    errorLabel->setStyleSheet("*{color:red;}");
    errorLabel->setAlignment(Qt::AlignHCenter);
    rightLayout->addSpacerItem(new QSpacerItem(0, 15, QSizePolicy::Minimum, QSizePolicy::Minimum));
    rightLayout->addWidget(trainButton);
    rightLayout->addSpacerItem(new QSpacerItem(0, 70, QSizePolicy::Minimum, QSizePolicy::Minimum));
    rightLayout->addWidget(label);
    rightLayout->addSpacerItem(new QSpacerItem(0, 50, QSizePolicy::Minimum, QSizePolicy::Minimum));
    rightLayout->addWidget(errorLabel);
    rightLayout->setAlignment(Qt::AlignHCenter);
    QHBoxLayout* layout = new QHBoxLayout();
    layout->addLayout(leftLayout);
    layout->addLayout(rightLayout);
    setLayout(layout);
    setFixedSize(600, 350);
}

void MainWindow::setErrorLabel(const QString& text) {
    errorLabel->setText(text);
}

void MainWindow::setLabel(const QString& text) {
    label->setText(text);
}

void MainWindow::trainModel() {
    errorLabel->setText("Training in Progress...");
    label->setText("Computing");
    net->trainModel();
}

void MainWindow::imageRecognition() {
    if (handwriter->isModified()) {
        net->query();
    } else {
        errorLabel->setText("Please Draw Something First");
    }
}

MainWindow::~MainWindow() {
    delete handwriter;
    delete clearButton;
    delete doneButton;
    delete label;
    delete trainButton;
    delete errorLabel;
}

