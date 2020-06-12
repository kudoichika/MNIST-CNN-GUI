#ifndef MODEL_H
#define MODEL_H

class MainWindow;
#include <vector>

#include <QtGui>
#include <QtWidgets>
#include <armadillo>

class Model {
public:
    Model(MainWindow* parent);
    void query();
    ~Model();
private:
    MainWindow* parent;
    QString queryFilePath;
};

#endif // MODEL_H
