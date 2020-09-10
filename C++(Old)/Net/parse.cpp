
//Path to all the files that store data/matrices
const std::string dir = "../Data/";
const std::string trainImgs = "train-images-idx3-ubyte";
const std::string trainLbls = "train-labels-idx1-ubyte";
const std::string parsedTrainImgs = "train-img.csv";
const std::string parsedTrainLbls = "train-lbl.csv";
const std::string testImgs = "t10k-images-idx3-ubyte";
const std::string testLbls = "t10k-labels-idx1-ubyte";
const std::string parsedTestImgs = "test-img.csv";
const std::string parsedTestLbls = "test-lbl.csv";

//temporary armadillo matrix variables to store data
arma::mat Features;
arma::mat Values;
arma::mat TestFeatures;
arma::mat TestValues;
int limit = 10100;

//Reverse an Integer
int rev(int val) {
    return ((int)(val & 255) << 24) + ((int) ((val >> 8) &255) << 16)
    + ((int) ((val >> 16)&255) << 8) + ((int) ((val >> 24) &255));
}

//Stolen from somwhere online
//Efficient way to parse binary => int matrix

//Procedure for training image files
bool readTrainImages() {
    std::ifstream imageFile(dir + trainImgs, std::ios::binary);
    if (!imageFile.fail()) {
        //Get the top header variables
        int m, n, r, c; m = n = r = c = 0;
        imageFile.read((char*)&m,sizeof(m)); m = rev(m);
        imageFile.read((char*)&n,sizeof(n)); n = rev(n);
        imageFile.read((char*)&r,sizeof(r)); r = rev(r);
        imageFile.read((char*)&c,sizeof(c)); c = rev(c);
		if (limit) n = limit;
        Features.set_size(n, r * c);
        //Loop through and store each entry into a n by r*c matrix
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < r; j++) {
                std::vector<float> matrixEntry;
                for (int k = 0; k < c; k++) {
                    unsigned char t = 0;
                    imageFile.read((char*) &t, sizeof(t));
                    //Divide by 255 to Decrease range from 0 to 1
                    Features(i, r * j + k) = ((int)t)/255.0;
                }
            }
        }
        //Mean Normalization
        Features -= 0.5;
        imageFile.close();
        std::cerr << "Completed Reading & Parsing Training Image File" << std::endl;
        //Store matrix as a csv file for the neural network class
        std::ofstream parsedFile(dir + parsedTrainImgs);
        Features.save(parsedFile, arma::csv_ascii);
        parsedFile.close();
        std::cerr << "Completed Saving Parsed Training Image File" << std::endl;
        return true;
    } else {
        std::cerr << "Error Loading Training Image File" << std::endl;
    }
    return false;
}

//Basically the same procedure but for the training labels
//A dataSize by 10 matrix (10 hot states that correspond to label values 0-9)
bool readTrainLabels() {
    std::ifstream labelFile(dir + trainLbls, std::ios::binary);
    if (!labelFile.fail()) {
        int m, n; m = n = 0;
        labelFile.read((char*)&m,sizeof(m)); m = rev(m);
        labelFile.read((char*)&n,sizeof(n)); n = rev(n);
		if (limit) n = limit;
        Values.set_size(n, 10);
        for (int i = 0; i < n; i++) {
            unsigned char t = 0;
            labelFile.read((char*) &t, sizeof(t));
            Values(i, ((int)t)) = 1;
        }
        labelFile.close();
        std::cerr << "Completed Reading & Parsing Training Label File" << std::endl;
        std::ofstream parsedFile(dir + parsedTrainLbls);
        Values.save(parsedFile, arma::csv_ascii);
        parsedFile.close();
        std::cerr << "Completed Saving Parsed Training Label File" << std::endl;
        return true;
    } else {
        std::cerr << "Error Loading Training Label File" << std::endl;
    }
    return false;
}

//Same proedure for test images
bool readTestImages() {
    std::ifstream imageFile(dir + testImgs, std::ios::binary);
    if (!imageFile.fail()) {
        int m, n, r, c; m = n = r = c = 0;
        imageFile.read((char*)&m,sizeof(m)); m = rev(m);
        imageFile.read((char*)&n,sizeof(n)); n = rev(n);
        imageFile.read((char*)&r,sizeof(r)); r = rev(r);
        imageFile.read((char*)&c,sizeof(c)); c = rev(c);
        TestFeatures.set_size(n, r * c);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < r; j++) {
                std::vector<float> matrixEntry;
                for (int k = 0; k < c; k++) {
                    unsigned char t = 0;
                    imageFile.read((char*) &t, sizeof(t));
                    TestFeatures(i, r * j + k) = ((int)t)/255.0;
                }
            }
        }
        TestFeatures -= 0.5;
        imageFile.close();
        std::cout << Features << std::endl;
        std::cerr << "Completed Reading & Parsing Test Image File" << std::endl;
        std::ofstream parsedFile(dir + parsedTestImgs);
        TestFeatures.save(parsedFile, arma::csv_ascii);
        parsedFile.close();
        std::cerr << "Completed Saving Parsed Test Image File" << std::endl;
        return true;
    } else {
        std::cerr << "Error Loading Test Image File" << std::endl;
    }
    return false;
}

//Same procedure for test labels
bool readTestLabels() {
    std::ifstream labelFile(dir + testLbls, std::ios::binary);
    if (!labelFile.fail()) {
        int m, n; m = n = 0;
        labelFile.read((char*)&m,sizeof(m)); m = rev(m);
        labelFile.read((char*)&n,sizeof(n)); n = rev(n);
        TestValues.set_size(n, 10);
        for (int i = 0; i < n; i++) {
            unsigned char t = 0;
            labelFile.read((char*) &t, sizeof(t));
            TestValues(i, ((int)t)) = 1;
        }
        labelFile.close();
        std::cerr << "Completed Reading & Parsing Test Label File" << std::endl;
        std::ofstream parsedFile(dir + parsedTestLbls);
        TestValues.save(parsedFile, arma::csv_ascii);
        parsedFile.close();
        std::cerr << "Completed Saving Parsed Test Label File" << std::endl;
        return true;
    } else {
        std::cerr << "Error Loading Label Test File" << std::endl;
    }
    return false;
}

//Helper function to clear the matrices in case the neural network
//is running while these are stil loaded in this file
void clearMemory() {
    Features.clear();
    Values.clear();
    TestFeatures.clear();
    TestValues.clear();
}
