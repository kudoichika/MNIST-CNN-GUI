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
	//parseTestData();
	//parseTrainData();
	//parseNumberTrainLabel();
	Net net(atoi(argv[1]), atoi(argv[2]));
	net.loadTrainData();
	net.train(make_pair(0, 10000),
				atof(argv[3]),
				atoll(argv[4]),
				atof(argv[5])
			);
	net.exportParams();
	return 0;
}
