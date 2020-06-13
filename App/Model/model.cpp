#include "Model/model.h"
#include "Interface/mainwindow.h"

#include <iostream>
#include <fstream>

Model::Model(MainWindow* parent) {
    this->parent = parent;
    queryFilePath = QApplication::applicationDirPath() + "/query.pgm";
}

arma::mat sigmoid(arma::mat& z) {
    return (1 / (1 + exp(-z)));
}

void Model::loadParameters() {
    params.clear();
    for (int i = 0; i < hiddenLayers; i++) {
        params.push_back(fastCSVReader(""));
    }
}

void Model::query() {
    loadParameters();
    compute();
}

void Model::compute() {
    arma::mat que;
    que.load(queryFilePath.toUtf8().constData(), arma::pgm_binary);
    que.resize(1, 784);
    que /= 255.0;
    for (int i = 0; i < hiddenLayers; i++) {
        que = sigmoid(arma::join_horiz(arma::ones(1, 1), que) * params[i]);
    }
    int res = accu(index_max(que));
    parent->setLabel(QString(res));
}

arma::mat Model::fastCSVReader(std::string file) {
    std::ifstream csv(file);
    std::vector<std::vector<double> > datas;
    for(std::string line; std::getline(csv, line);) {
        std::vector<double> data;
        auto start = 0U;
        auto end = line.find(",");
        while (end != std::string::npos) {
            data.push_back(std::stod(line.substr(start, end - start)));
            start = end + 1;
            end = line.find(",", start);
        }
        data.push_back(std::stod(line.substr(start, end)));
        datas.push_back(data);
    }
    arma::mat data_mat = arma::zeros<arma::mat>(datas.size(), datas[0].size());
    for (int i = 0; i < datas.size(); i++) {
        arma::mat r(datas[i]);
        data_mat.row(i) = r.t();
    }
    return data_mat;
}

Model::~Model() {

}
