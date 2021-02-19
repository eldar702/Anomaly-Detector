#include "HybridAnomalyDetector.h"

float HybridAnomalyDetector:: getThreshold() {
    return min(this->min_correlate, this->max_correlate);
}
void HybridAnomalyDetector:: completeStruct(const TimeSeries &ts, correlatedFeatures &c){
    if(fabs(c.corrlation) > this->max_correlate)
        return SimpleAnomalyDetector::completeStruct(ts, c);
    Circle circle = findMinCircle(ts.getMatrix().at(c.feature1), ts.getMatrix().at(c.feature2), ts.getLineNum());
    c.lin_reg = Line(circle.center.x, circle.center.y);
    c.threshold = circle.radius * 1.1f;
}
bool HybridAnomalyDetector::isDetect(const TimeSeries &ts, int numOfLine, const correlatedFeatures &cfCheck){
    if(fabs(cfCheck.corrlation) > this->max_correlate)
        return SimpleAnomalyDetector::isDetect(ts, numOfLine, cfCheck);
    Point point(ts.getMatrix().at(cfCheck.feature1)[numOfLine], ts.getMatrix().at(cfCheck.feature2)[numOfLine]);
    Circle circle(Point(cfCheck.lin_reg.a, cfCheck.lin_reg.b), cfCheck.threshold);
    return !insideCircle(point, circle);
}