#include "timeseries.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
// the function reads the csv file and make a matrix of the data
void TimeSeries::createMatrix(const char *CSVfileName) {
    string temp_line;
    ifstream file(CSVfileName); // open file
    if (!file)  cout << "could not open the file." << endl, exit;  // check if the file opened
    while (getline(file, temp_line)) // read current line
    {
        if (this->line_num)   // making the details of the features (from line 2 and after)
        {
           // vector<string> result;
            stringstream s_stream(temp_line); //create string stream from the string
            for(int i = 0; i < column_num; ++i) {
                string substr;
                getline(s_stream, substr, ','); //get first string delimited by comma
                matrix.at(features[i]).push_back(stof(substr));
            }
            ++line_num;
        } else { createMatrixHelper(file, temp_line); }  // make the vector of the features,
    }
    --line_num;
}

// helper function to "createMatrix", this function make the features vector.
void TimeSeries::createMatrixHelper(ifstream &file, const string &line) {
    stringstream check(line);
    while (check.good()) {
        string substr = "";
        vector<float> temp;
        getline(check, substr, ',');
        features.push_back(substr);
        matrix.insert(pair<string, vector<float>>(substr, temp));
        ++column_num;
    }
    ++this->line_num;
}






