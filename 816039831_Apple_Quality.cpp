//Name :Vishwesh Pattanaik
//ID: 816039831
//Course: ECNG 1009
//Individual Mini Project
//Dataset: Apple Quality

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>

using namespace std;

//Create a struct for storing apple quality
struct AppleQuality {
    int appleid;
    double Size;
    double Weight;
    double Sweetness;
    double Crunchiness;
    double Juiciness;
    double Ripeness;
    double Acidity;
    string Quality;
};

//read data
vector<AppleQuality> readAppleData(string filename) {
    vector<AppleQuality> apples;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error in opening the file: " << filename << endl;
        return apples;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        AppleQuality apple;

        char comma;
        ss >> apple.appleid >> comma;
        ss >> apple.Size >> comma;
        ss >> apple.Weight >> comma;
        ss >> apple.Sweetness >> comma;
        ss >> apple.Crunchiness >> comma;
        ss >> apple.Juiciness >> comma;
        ss >> apple.Ripeness >> comma;
        ss >> apple.Acidity >> comma;
        ss >> apple.Quality;
        apples.push_back(apple);
    }

    file.close();
    return apples;
}
// clean data
vector<AppleQuality> cleanData(const vector<AppleQuality>& apples) {
    vector<AppleQuality> cleanedApples;
    for (const AppleQuality& apple : apples) {
        if (apple.appleid >= 0 && apple.Size != 0 && apple.Weight != 0 && apple.Sweetness != 0 &&
            apple.Crunchiness != 0 && apple.Juiciness != 0 && apple.Ripeness != 0 &&
            apple.Acidity != 0 && !apple.Quality.empty()) {
            cleanedApples.push_back(apple);
        }
    }
    return cleanedApples;
}

// export to excel
void exportToExcel(const vector<AppleQuality>& cleanedApples) {
    ofstream outfile("Apples.csv");
    if (outfile.is_open()) {
        outfile << "Apple ID,Size,Weight,Sweetness,Crunchiness,Juiciness,Ripeness,Acidity,Quality" << endl;
        for (const AppleQuality& apple : cleanedApples) {
            outfile << apple.appleid << "," << apple.Size << "," << apple.Weight << "," << apple.Sweetness << ","
                << apple.Crunchiness << "," << apple.Juiciness << "," << apple.Ripeness << ","
                << apple.Acidity << "," << apple.Quality << endl;
        }
        outfile.close();
        cout << "Cleaned data was exported to Apples.csv" << endl;
    }
    else {
        cout << "Error: Cannot open file" << endl;
    }
}
// count function
void countQualityApples(const vector<AppleQuality>& cleanedApples, int& goodCount, int& badCount) {
    goodCount = 0;
    badCount = 0;

    for (const AppleQuality& apple : cleanedApples) {
        if (apple.Quality == "good") {
            goodCount++;
        }
        else if (apple.Quality == "bad") {
            badCount++;
        }
    }
}

// Function to display the first twenty entries of the data in a table
void displayData(const vector<AppleQuality>& cleanedApples) {
    cout << "First twenty apple records:" << endl;
    // Display headers
    cout << left << setw(10) << "AppleID" << setw(8) << "Size" << setw(8) << "Weight" << setw(12) << "Sweetness"
        << setw(12) << "Crunchiness" << setw(12) << "Juiciness" << setw(12) << "Ripeness" << setw(12) << "Acidity"
        << setw(10) << "Quality" << endl;

    cout << string(110, '-') << endl;  // Print a separator line

    // Display each record formatted as a table row
    for (int i = 0; i < 20 && i < cleanedApples.size(); ++i) {
        const auto& a = cleanedApples[i];
        cout << setw(10) << a.appleid
            << setw(8) << fixed << setprecision(1) << a.Size
            << setw(8) << a.Weight
            << setw(12) << a.Sweetness
            << setw(12) << a.Crunchiness
            << setw(12) << a.Juiciness
            << setw(12) << a.Ripeness
            << setw(12) << a.Acidity
            << setw(10) << a.Quality << endl;
    }
}

// Function to train Gaussian Naive Bayes classifier
void trainGaussianNB(const vector<AppleQuality>& cleanedApples) {
    // Separate features and target variable
    vector<vector<double>> features;
    vector<string> targets;

    for (const auto& apple : cleanedApples) {
        features.push_back({ apple.Size, apple.Weight, apple.Sweetness, apple.Crunchiness,
                            apple.Juiciness, apple.Ripeness, apple.Acidity });
        targets.push_back(apple.Quality);
    }

    // Separate data for good and bad quality apples
    vector<vector<double>> goodQualityData, badQualityData;
    for (size_t i = 0; i < features.size(); ++i) {
        if (targets[i] == "good") goodQualityData.push_back(features[i]);
        else if (targets[i] == "bad") badQualityData.push_back(features[i]);
    }

    // Calculate mean and variance for each feature
    vector<double> goodQualityMean, badQualityMean, goodQualityVariance, badQualityVariance;
    for (size_t i = 0; i < features[0].size(); ++i) {
        double goodSum = 0, badSum = 0;
        for (const auto& data : goodQualityData) goodSum += data[i];
        for (const auto& data : badQualityData) badSum += data[i];

        goodQualityMean.push_back(goodSum / goodQualityData.size());
        badQualityMean.push_back(badSum / badQualityData.size());

        double goodVarSum = 0, badVarSum = 0;
        for (const auto& data : goodQualityData) goodVarSum += pow(data[i] - goodQualityMean[i], 2);
        for (const auto& data : badQualityData) badVarSum += pow(data[i] - badQualityMean[i], 2);

        goodQualityVariance.push_back(goodVarSum / (goodQualityData.size() - 1));
        badQualityVariance.push_back(badVarSum / (badQualityData.size() - 1));
    }

    // Display the trained model parameters
    cout << "Trained Gaussian Naive Bayes Classifier:" << endl;
    cout << "Good Quality Mean: ";
    for (auto mean : goodQualityMean) cout << mean << " ";
    cout << endl;
    cout << "Good Quality Variance: ";
    for (auto var : goodQualityVariance) cout << var << " ";
    cout << endl;
    cout << "Bad Quality Mean: ";
    for (auto mean : badQualityMean) cout << mean << " ";
    cout << endl;
    cout << "Bad Quality Variance: ";
    for (auto var : badQualityVariance) cout << var << " ";
    cout << endl;
}

// Function to predict apple quality using Gaussian Naive Bayes classifier
string predictQuality(const vector<AppleQuality>& cleanedApples, const vector<double>& goodQualityMean,
    const vector<double>& goodQualityVariance, const vector<double>& badQualityMean,
    const vector<double>& badQualityVariance, const vector<double>& classProbabilities,
    const AppleQuality& apple) {
    // Calculate likelihoods for good quality
    double goodLikelihood = 1.0;
    for (size_t i = 0; i < apple.Size; ++i) {
        goodLikelihood *= exp(-(apple.Size - goodQualityMean[i]) * (apple.Size - goodQualityMean[i]) /
            (2 * goodQualityVariance[i])) /
            sqrt(2 * 3.14 * goodQualityVariance[i]);
    }

    // Calculate likelihoods for bad quality
    double badLikelihood = 1.0;
    for (size_t i = 0; i < apple.Size; ++i) {
        badLikelihood *= exp(-(apple.Size - badQualityMean[i]) * (apple.Size - badQualityMean[i]) /
            (2 * badQualityVariance[i])) /
            sqrt(2 * 3.14 * badQualityVariance[i]);
    }

    // Calculate posterior probabilities
    double goodPosterior = goodLikelihood * classProbabilities[0];
    double badPosterior = badLikelihood * classProbabilities[1];

    // Predict the quality
    if (goodPosterior > badPosterior) {
        return "good";
    }
    else {
        return "bad";
    }
}

//void predictAppleQuality(GaussianNB& model) {
//    ModelingData apple;
//    cout << "Enter apple features for prediction:\n";
//    cout << "Size: "; cin >> apple.size;
//    cout << "Weight: "; cin >> apple.weight;
//    cout << "Sweetness: "; cin >> apple.sweetness;
//    cout << "Crunchiness: "; cin >> apple.crunchiness;
//    cout << "Juiciness: "; cin >> apple.juiciness;
//    cout << "Ripeness: "; cin >> apple.ripeness;
//    cout << "Acidity: "; cin >> apple.acidity;
//
//    string prediction = predict(model, apple);
//    cout << "Predicted quality of the apple is: " << prediction << endl;

int main() {

    cout << "Name: Vishwesh Pattanaik" << endl;
    cout << "ID: 816039831" << endl;
    cout << "Apple Quality Analysis" << endl;

    cout << "Hi, Welcome to my Apple Quality Analysis Program. The chosen dataset is Apple Quality Analysis from Kaggle's website uploaded by NIDULA ELGIRIYEWITHANA" << endl;
    cout << "Lets begin: " << endl;

    string filename = "apple_quality.csv";
    vector<AppleQuality> apples = readAppleData(filename);
    vector<AppleQuality> cleanedApples = cleanData(apples);

    if (cleanedApples.empty()) {
        cout << "No data to process" << endl;
        return 1;
    }

    exportToExcel(cleanedApples);

    int goodCount = 0, badCount = 0;

    countQualityApples(cleanedApples, goodCount, badCount);

    cout << "Number of good apples: " << goodCount << endl;
    cout << "Number of bad apples: " << badCount << endl;

    displayData(cleanedApples);

    cout << endl;
    // Train Gaussian Naive Bayes classifier
    trainGaussianNB(cleanedApples);

    //predictQuality(model);

    return 0;
}