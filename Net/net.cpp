#include <utility>

//Nerual Network uses 28x28 Image File (pixel values)
//and outputs multiclass classification into different digits
//ranged from 1 - 10

//Path for each data file
const std::string loc = "../Data/";
const std::string trainX = "train-img.csv";
const std::string trainY = "train-lbl.csv";
const std::string testX = "test-img.csv";
const std::string testY = "test-lbl.csv";
const std::string paramFile = "theta";

//Neural Network class to make private/public functions/variables
//easier to handle
class Net {
private:
	arma::mat X; //training/validation features
	arma::mat Y; //training/validation labels
	arma::mat X_test; //test features
	arma::mat Y_test; //test labels
	double alpha; //alpha learning rate
	double lambda; //lambda regularization rate
	std::vector<arma::mat> Theta; //array of each layers weight parameters
	void resetParams();
	void update(std::vector<arma::mat>& change);
	arma::mat sigmoid(arma::mat& z);
	arma::mat dsigmoid(arma::mat& z);
	std::vector<arma::mat> propogation(std::pair<int, int> data, double& cost);
	arma::mat compute(arma::mat& query);
	arma::mat fastCSVReader(std::string file);
public:
	Net();
	Net(int hidden, int neurons);
	int hidden; //corresponds to the # of hidden layers
	int neurons; //corrresponds to # of units in each layer (uniform)
	void loadTrainData();
	void loadTestData();
	void train(std::pair<int, int> data, double alpha, long long iter, double lambda);
	void validate(std::pair<int, int> data);
	void test();
	void exportParams();
};

Net::Net() {
	this->hidden = 1;
	this->neurons = 30;
	std::cout << "Initialized Neural Network with 1 Hidden Layer (30 Neurons)" << std::endl;
}

Net::Net(int hidden, int neurons) {
	this->hidden = hidden;
	this->neurons = neurons;
	std::cout << "Initialized Neural Network with " << hidden;
	std::cout << " Hidden Layer(s) (" << neurons << " Neurons)" << std::endl;
}

//Load training data to memory
void Net::loadTrainData() {
	//X.load(loc + trainX);
	//Y.load(loc + trainY);
	X = fastCSVReader(loc + trainX);
	Y = fastCSVReader(loc + trainY);
	std::cout << "Completed Loading Training Data to Memory" << std::endl;
}

//Load test data to memory
void Net::loadTestData() {
	//X_test.load(loc + testX);
	//Y_test.load(loc + testY);
	X_test = fastCSVReader(loc + testX);
	Y_test = fastCSVReader(loc + testY);
	std::cout << "Completed Loading Test Data to Memory" << std::endl;
}

//Export for the use of the GUI Interface
//each layer parameter has its own file
void Net::exportParams() {
  	for (int i = 0; i < hidden; i++) {
    	Theta[i].save(loc + paramFile + std::to_string(i));
		std::cout << "Exporting Theta " + std::to_string(i) << std::endl;
  	}
	std::cout << "Competed Exporting All Parameters" << std::endl;
}

//Reset params to match the number of hidden layers before
//each time training is done using random initialization
void Net::resetParams() {
	Theta.clear();
	arma::mat T;
	T.randu(neurons, 785);
	T = (T - 0.5) / 4.0;
	Theta.push_back(T);
	for (int i = 0; i < hidden - 1; i++) {
		T.randu(neurons, neurons+1);
		T = (T - 0.5) / 4.0;
		Theta.push_back(T);
	}
	T.randu(10, neurons+1);
	T = (T - 0.5) / 4.0;
	Theta.push_back(T);
	std::cout << "Completed Random Initialization of Parameters" << std::endl;
}

//Sigmoid function
arma::mat Net::sigmoid(arma::mat& z) {
	arma::mat sig = 1 / (1 + exp(-z));
	return sig;
}

//Derivitive of the sigmoid function
arma::mat Net::dsigmoid(arma::mat& z) {
	arma::mat sig = sigmoid(z);
	arma::mat dsig = sig % (1 - sig);
	return dsig;
}

//Forward Propogation, Cost Calculation and Back Propogation
//done simulatenously
std::vector<arma::mat> Net::propogation(std::pair<int, int> data, double& cost) {
	double m = data.second - data.first;
	//Forward Prop
	std::vector<arma::mat> Transforms;
	arma::mat Activated;
	Activated = X.rows(data.first, data.second);
	Transforms.push_back(arma::join_horiz(arma::ones(Activated.n_rows, 1), Activated));
	for (int i = 0; i < hidden; i++) {
		Transforms.push_back(arma::join_horiz(arma::ones(Activated.n_rows, 1), Activated)
					* Theta[i].t());
		Activated = sigmoid(Transforms[i+1]);
	}
	arma::mat Out = Activated;
	//Cost Calculation
	cost = (-1/m) * accu(Y.rows(data.first, data.second) * log(Out)
			+ (1 - Y.rows(data.first, data.second)) * log(1 - Out));
	//Regularized Cost
	for (int i = 0; i < hidden + 1; i++) {
		cost += (lambda/(2*m))
				* accu(square(Theta[i].cols(1, Theta[i].n_cols-1)));
	}
	//Back Prop
	std::vector<arma::mat> E; //Error Deltas
	E.push_back(Out - Y.rows(data.first, data.second));
	for (int i = 0; i < hidden; i++) {
		arma::mat T = Transforms[hidden - i].cols(1, E[i].n_cols-1);
		E.push_back((E[i].t() * Theta[hidden - i]) % arma::join_horiz(arma::ones(m, 1),
					dsigmoid(T)));
	}
	//Error Accumulation var E -> partial change of parameters
	E[0] = E[0].t() % Transforms[hidden];
	for (int i = 1; i < hidden; i++) {
		E[i] = (1/m) * E[i].cols(1, E[i].n_cols-1).t() * Transforms[hidden - i];
	}
	//Regularized Change
	for (int i = 0; i < hidden + 1; i++) {
		E[i].cols(1, E[i].n_cols-1) += (lambda/m)
			* Theta[hidden - i].cols(1, Theta[hidden - i].n_cols-1);
	}
	return E;
}

//Forward Propogation
arma::mat Net::compute(arma::mat& query) {
	arma::mat result = query;
	for (int i = 0; i < hidden + 1; i++) {
		result = arma::join_horiz(arma::ones(result.n_rows, 1), result) * Theta[i].t();
		result = sigmoid(result);
	}
	return result;
}

//Simultaenous Update for parameters
void Net::update(std::vector<arma::mat>& change) {
	for (int i = 0; i < hidden + 1; i++) {
		Theta[i] -= alpha * change[i];
	}
}

//Main training example with variable parameters alpha, lambda,
//iterations, and size of the training set. Simultaenous update
//after each gradient descent iteration
void Net::train(std::pair<int, int> data, double alpha, long long iter, double lambda) {
	this->alpha = alpha;
	this->lambda = lambda;
	resetParams();
	double cost = 0;
	//Batch Gradient Descent
	std::cout << "Starting Batch Gradient Descent with " << iter << " iterations" << std::endl;
	for (long long i = 0; i < iter; i++) {
		std::vector<arma::mat> change = propogation(data, cost);
		update(change);
		std::cout << "Iteration " << i << " Cost J = " << cost << std::endl;
	}
	std::cout << "Completed Batch Gradient Descent with Cost " << cost << std::endl;
}

//Cross-validation function with parameter as the
//range of data to evaluate upon
void Net::validate(std::pair<int, int> data) {
	arma::mat newX = X.rows(data.first, data.second);
    arma::umat Check = (compute(newX) == Y.rows(data.first, data.second)); //range of data
    double m = data.second - data.first;
    std::cout << "Accuracy of Cross-Validation Set: " << accu(Check)/m << std::endl;
}

//Test function to calculate and dispay accuracy of the model
//with the test data
void Net::test() {
	arma::umat Check = (compute(X_test) == Y_test);
	double m = X_test.n_rows;
	std::cout << "Accuracy of Test Set: " << accu(Check)/m << std::endl;
}

//Fast CSV Reading copied from somewhere online
arma::mat Net::fastCSVReader(std::string file) {
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
