#include <iostream>
#include <fstream>
#include <armadillo>

#include "parse.cpp"
#include "net.cpp"

using namespace std;

void parseTrainData() {
	readTrainImages();
	readTrainLabels();
	clearMemory();
}

void parseTestData() {
	readTestImages();
	readTestLabels();
	clearMemory();
}

int main() {
	Net net;
	net.loadTrainData();
	net.train(make_pair(0, 10), 0.1, 10, 0);
	return 0;
}
