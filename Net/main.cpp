#include <iostream>
#include <fstream>
#include <cstdlib>
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

int main(int argc, char *argv[]) {
	//parseTrainData();
	Net net;
	net.loadTrainData();
	net.train(make_pair(0, 10000),
				atof(argv[1]),
				atoll(argv[2]),
				atof(argv[3])
			);
	net.exportParams();
	return 0;
}
