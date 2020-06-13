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

    const int hiddenLayers = 1;

    void loadParameters();
    void compute();
    vector<arma::mat> params;
    arma::mat Model::fastCSVReader(std::string file)
};

#endif // MODEL_H
