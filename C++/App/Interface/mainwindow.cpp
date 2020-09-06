#include "Interface/mainwindow.h"

#include <QtGui>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    handwriter = new Handwriter();
    model = new Model(this);
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
    label = new QLabel("Draw\n and Try!");
    label->setAlignment(Qt::AlignHCenter);
    label->setStyleSheet("*{color:#5A95B3;font-size:30px;font-weight:bold;}");
    rightLayout->addSpacerItem(new QSpacerItem(0, 50, QSizePolicy::Minimum, QSizePolicy::Minimum));
    rightLayout->addWidget(label);
    rightLayout->setAlignment(Qt::AlignHCenter);
    QHBoxLayout* layout = new QHBoxLayout();
    layout->addLayout(leftLayout);
    layout->addLayout(rightLayout);
    setLayout(layout);
    setFixedSize(600, 350);
}

void MainWindow::setLabel(const QString& text) {
    label->setText(text);
}

void MainWindow::imageRecognition() {
    if (handwriter->isModified()) {
        model->query();
    }
}

MainWindow::~MainWindow() {
    delete handwriter;
    delete clearButton;
    delete doneButton;
    delete label;
}

