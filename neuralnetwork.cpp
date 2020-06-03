#include "neuralnetwork.h"
#include "mainwindow.h"

#include <iostream>
#include <fstream>

NeuralNetwork::NeuralNetwork(MainWindow* parent) {
    this->parent = parent;
    //load data if already exists
    imageFilePath = "/train-images-idx3-ubyte";
    labelFilePath = "/train-labels-idx1-ubyte";
    queryFilePath = QApplication::applicationDirPath() + "/query.pgm";
}

arma::mat* NeuralNetwork::sigmoid(arma::mat z) {
    z = 1 / (1 + exp(-z));
    return &z;
}

arma::mat* NeuralNetwork::dsigmoid(arma::mat z) {
    arma::mat dz = *sigmoid(z);
    z = dz % (1 - dz);
    return &z;
}

void NeuralNetwork::trainModel() {
    parent->setErrorLabel(QString("Reading Data..."));
    if (readData()) {
        parent->setErrorLabel(QString("Training Data..."));
        train();
        parent->label->setText("Ready");
        parent->errorLabel->setText("Completed Training");
    }
}

void NeuralNetwork::forwardBackPropogation(arma::mat& input, double& cost, int lambda, double alpha) {
    //Forward Propogation
    arma::mat input_mod = arma::join_horiz(
                arma::ones<arma::mat>(numT,1),
                input) * hiddenLayerMap.t();
    arma::mat hidden = *sigmoid(input_mod);
    arma::mat hidden_mod = arma::join_horiz(
                arma::ones<arma::mat>(numT, 1),
                hidden) * outputLayerMap.t();
    std::cout << hidden_mod << std::endl;
    arma::mat output = *sigmoid(hidden_mod);
    std::cout << output << std::endl;
    cost = (-1/numT) * accu(values % log(output) + (1-values) % log(1-output));
    std::cout << "Cost W/O Reg: " << cost << std::endl;
    cost += (lambda/(2 * numT)) * (accu(square(hiddenLayerMap.cols(1, hiddenLayerMap.n_cols-1))) +
                                        accu(square(outputLayerMap.cols(1, outputLayerMap.n_cols-1))));

    std::cout << "Cost: " << cost << std::endl;

    //Backward Propogation
    arma::mat delOut = output - values;
    arma::mat delHidden = (delOut * outputLayerMap) %
            arma::join_horiz(arma::ones(numT, 1), * dsigmoid(input_mod));

    arma::mat D2 = delOut.t() * arma::join_horiz(arma::ones<arma::mat>(numT, 1), hidden);
    arma::mat D1 = delHidden.cols(1, delHidden.n_cols-1).t() *
            arma::join_horiz(arma::ones<arma::mat>(numT,1),input);

    arma::mat T2(outputLayerMap.n_rows, outputLayerMap.n_cols);
    arma::mat T1(hiddenLayerMap.n_rows, hiddenLayerMap.n_cols);
    T2 += (1/numT) * D2;
    T1 += (1/numT) * D1;

    T2.cols(1, T2.n_cols-1) += (lambda/numT) * outputLayerMap.cols(1, T2.n_cols-1);
    T1.cols(1, T1.n_cols-1) += (lambda/numT) * hiddenLayerMap.cols(1, T1.n_cols-1);

    outputLayerMap = outputLayerMap - alpha * T2;
    hiddenLayerMap = hiddenLayerMap - alpha * T1;
}

void NeuralNetwork::train() {
    hiddenLayerMap.randu(25, 785);
    outputLayerMap.randu(10, 26);
    hiddenLayerMap -= 0.5;
    hiddenLayerMap /= 4.0;
    outputLayerMap -= 0.5;
    outputLayerMap /= 4.0;
    const int epoch = 10;
    const int lambda = 1;
    const double alpha = 0.01;
    double cost = 0;
    for (int i = 0; i < epoch; i++) {
        forwardBackPropogation(features, cost, lambda, alpha);
    }
    parent->errorLabel->setText("Cost = " + QString::number(cost));
}

void NeuralNetwork::query() {
    arma::mat que; que.load(queryFilePath.toUtf8().constData(), arma::pgm_binary);
    que.resize(1, 784);
    que /= 255.0;
    que -= 0.5;
    que /= 4.0;
    //std::cout << que << std::endl; //read?
    arma::mat hyp1 = *sigmoid(arma::join_horiz(arma::ones(1, 1), que) * hiddenLayerMap.t());
    arma::mat hyp2 = *sigmoid(arma::join_horiz(arma::ones(1, 1), hyp1) * outputLayerMap.t());
    std::cout << hyp2 << std::endl;
    for (int i = 0; i < hyp2.n_rows; i++) {
        if (hyp2[i] == 1) {
            parent->setLabel(QString::number(i));
            break;
        }
    }
}

int NeuralNetwork::rev(int val) {
    return ((int)(val & 255) << 24) + ((int) ((val >> 8) &255) << 16)
    + ((int) ((val >> 16)&255) << 8) + ((int) ((val >> 24) &255));
}

bool NeuralNetwork::readData() {
    QTemporaryDir tempDir;
    QFile::copy(":/Data" + imageFilePath, tempDir.path() + imageFilePath);
    std::ifstream imageFile(QString(tempDir.path() + imageFilePath).toUtf8().constData(), std::ios::binary);
    if (!imageFile.fail()) {
        int m, n, r, c; m = n = r = c = 0;
        imageFile.read((char*)&m,sizeof(m)); m = rev(m);
        imageFile.read((char*)&n,sizeof(n)); n = rev(n);
        imageFile.read((char*)&r,sizeof(r)); r = rev(r);
        imageFile.read((char*)&c,sizeof(c)); c = rev(c);
        n = 10;
        numT = n;
        features.set_size(n, r * c);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < r; j++) {
                std::vector<float> matrixEntry;
                for (int k = 0; k < c; k++) {
                    unsigned char t = 0;
                    imageFile.read((char*) &t, sizeof(t));
                    features(i, r * j + k) = ((int)t)/255.0;
                }
            }
        }
        features -= 0.5;
        features /= 4.0;
        imageFile.close();
    } else {
        parent->setErrorLabel(QString("Could Not Find Training Images File"));
        return false;
    }
    QFile::copy(":/Data" + labelFilePath, tempDir.path() + labelFilePath);
    std::ifstream labelFile(QString(tempDir.path() + labelFilePath).toUtf8().constData(), std::ios::binary);
    if (!labelFile.fail()) {
        int m, n; m = n = 0;
        labelFile.read((char*)&m,sizeof(m)); m = rev(m);
        labelFile.read((char*)&n,sizeof(n)); n = rev(n);
        n = 10;
        values.set_size(n, 10);
        for (int i = 0; i < n; i++) {
            unsigned char t = 0;
            labelFile.read((char*) &t, sizeof(t));
            values(i, ((int)t)) = 1;
        }
        labelFile.close();
        return true;
    } else {
        parent->setErrorLabel(QString("Could Not Find Training Label File"));
    }
    return false;
}

NeuralNetwork::~NeuralNetwork() {

}
