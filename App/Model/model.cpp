#include "Model/model.h"
#include "Interface/mainwindow.h"

#include <iostream>
#include <fstream>

Model::Model(MainWindow* parent) {
    this->parent = parent;
    queryFilePath = QApplication::applicationDirPath() + "/query.pgm";
}

void Model::query() {
    //Function to Predict
}

Model::~Model() {

}
