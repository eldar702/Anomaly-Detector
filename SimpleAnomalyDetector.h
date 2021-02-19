

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <cstring>
#include <cmath>

struct correlatedFeatures{
    string feature1,feature2;  // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
};

/** this function complete the correlatedFeatures . */
class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
    vector<correlatedFeatures> cf;

protected:
    float max_correlate;
public:
    SimpleAnomalyDetector():max_correlate(0.9f){};
    ~SimpleAnomalyDetector() override = default;

    void learnNormal(const TimeSeries& ts) override;
    vector<AnomalyReport> detect(const TimeSeries& ts) override;
    virtual float getThreshold();
    vector<correlatedFeatures> getNormalModel(){
        return cf;
    }
    void setMaxCorrelate(float maxCorrelate);
    virtual void completeStruct(const TimeSeries &ts, correlatedFeatures &c);
    virtual bool isDetect(const TimeSeries &ts, int numOfLine, const correlatedFeatures &cfCheck);

};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
