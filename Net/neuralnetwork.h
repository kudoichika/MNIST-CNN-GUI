#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

class MainWindow;
#include <vector>

#include <QtGui>
#include <QtWidgets>
#include <armadillo>

class NeuralNetwork {
public:
    NeuralNetwork(MainWindow* parent);
    ~NeuralNetwork();
    void trainModel();
    void query();
private:
    MainWindow* parent;
    QString imageFilePath;
    QString labelFilePath;
    QString queryFilePath;

    double numT;

    bool readData();
    int rev(int val);

    void train();
    void forwardBackPropogation(arma::mat& input, double& cost, int lambda, double alpha);

    arma::mat* sigmoid(arma::mat z);
    arma::mat* dsigmoid(arma::mat z);

    arma::mat features;
    arma::mat values;
    arma::mat hiddenLayerMap;
    arma::mat outputLayerMap;
};

#endif // NEURALNETWORK_H
