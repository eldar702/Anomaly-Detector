
#include "SimpleAnomalyDetector.h"

#include <iostream>


float checkMaxCorrelate(vector<string>& features,const TimeSeries& ts, float big_correlate, float big_threshold, int i, int& higherNum) {
    // variables zeroing:
    float temp = 0;
    higherNum = 0;

    /* this loop check which variable is the most correlate to the checked variable
       j = i+1 so it will not check the same variables (when i=j) and no repeat checked which happened. */
    for (int j = i + 1; j < ts.getColumnNum(); ++j) {
        temp = pearson(ts.getMatrix().at(features[i]).data(), ts.getMatrix().at(features[j]).data(),
                       ts.getLineNum());
        // check if the checked variable is the one which most correlate to
        if (abs(temp) > abs(big_correlate)) {
            big_correlate = temp;
            higherNum = j;
        }
    }
    return abs(big_correlate);
}
/** the function check the max threshold for 2 correlated features */
float getCorrelate(vector<float> &feature1, vector<float> &feature2, int numOfLines, Line lin_reg, float max_pearson)
{
    float big_threshold = 0;
    for (int x = 0; x < numOfLines; ++x) {
        float temp = dev(Point(feature1[x],feature2[x]), lin_reg);
        if (temp > big_threshold) big_threshold = temp;
    }
    return big_threshold * 1.1f;
}

/** getter to max correlate */
float SimpleAnomalyDetector::getThreshold() {
    return this->max_correlate;
}
/** this function complete the correlatedFeatures - adding the lin_reg and thresold details. */
void SimpleAnomalyDetector::completeStruct(const TimeSeries &ts, correlatedFeatures &c){
    c.lin_reg = linear_reg(ts.getMatrix().at(c.feature1).data(), ts.getMatrix().at(c.feature2).data(), ts.getLineNum());
    c.threshold = getCorrelate(ts.getMatrix().at(c.feature1), ts.getMatrix().at(c.feature2), ts.getLineNum(), c.lin_reg, c.corrlation);
}

/** part of the offline learning stage - Suit for any variable the most correlated variable.
 *  the function uses "pearson" function to find the correlation between every 2 variables */

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    //variables declaration:
    vector<string> features = ts.getFeatures();
    float max_correlate_between = 0;
    for (int i = 0; i < features.size(); ++i)
    {
        int higherNum = 0;
        max_correlate_between = checkMaxCorrelate(features, ts, 0.0f, 0.0f, i, higherNum);
        float min_correlate = getThreshold();
        // check if the correlate for the 2 checked variables is more than the min/max (min if it hybrid and max if its
        if (max_correlate_between <= min_correlate) continue;
        else { // if max_correlate_betweem > 0.5

            string feature1 = features[i], feature2 = features[higherNum];
            correlatedFeatures c{feature1, feature2, max_correlate_between, {0, 0}, 0 };
            completeStruct(ts, c);
            cf.push_back(c);
        }
    }
}
// detect and report if there is an error in the flight, when it happened and who's involve.
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {
    //variables declaration:
    const vector<string> features = ts.getFeatures();
    vector<AnomalyReport> reportVec;
    for(auto & j : cf) {
    for(int i = 0; i < ts.getLineNum(); ++i) {
            // check if (and if yes when) there is an error. if yes - make a report of this.
            // the report include the time and the involve variables
            if(isDetect(ts, i, j)) {
                AnomalyReport report = {j.feature1 + "-" + j.feature2, (i+1)};
                reportVec.push_back(report);
            }
        }
    }
    return reportVec;
}
/** this function checks if a point  exceeds the range defined as normal, in our case, checks if
 *  a given point (represent a specific detail of a report) is anomaly  */
bool SimpleAnomalyDetector::isDetect(const TimeSeries &ts, int numOfLine, const correlatedFeatures &cfCheck)
{
    float x = ts.getMatrix().at(cfCheck.feature1)[numOfLine]; // creating the x value of the point
    float y = ts.getMatrix().at(cfCheck.feature2)[numOfLine]; // creating the y value of the point
    Point p(x, y);
    float temp = dev(p, cfCheck.lin_reg);
    if(temp > cfCheck.threshold) {
        return true;
    }
    return false;
}
/** setter to max_correlate **/
void SimpleAnomalyDetector::setMaxCorrelate(float maxCorrelate) {
    max_correlate = maxCorrelate;
}
