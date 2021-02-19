

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "anomaly_detection_util.h"
using namespace std;
class TimeSeries{
    map<string, vector<float> > matrix;
    vector<string> features;
    int line_num = 0;
    int column_num = 0;
public:
    // constructor
    explicit TimeSeries(const char *CSVfileName)
    {
        createMatrix(CSVfileName); // make an unsorted map with the csv details.
    } // destructor
    virtual ~TimeSeries()= default; // virtual destructor

    // the function reads the csv file and make a matrix of the data
    void createMatrix(const char* CSVfileName);
    void createMatrixHelper(ifstream& file,const string& line);

    //getters:
    map<string, vector<float>> getMatrix() const {
        return matrix;
    }
    const vector<string> getFeatures() const {
        return features;
    }
    int getLineNum() const {
        return line_num;
    }
    int getColumnNum() const {
        return column_num;
    }
};

#endif /* TIMESERIES_H_ */
