

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
protected:
    float min_correlate = 0.5f;
public:
	HybridAnomalyDetector() = default;
	~HybridAnomalyDetector() override = default;
//    virtual Line getLine(const TimeSeries &ts,const string& feature1, const string& feature2, float maxPearson);
    float getThreshold() override;
//    float getThreshold(vector<float> &feature1, vector<float> &feature2, int numOfLines, Line lin_reg, float max_pearson ) override;
    bool isDetect(const TimeSeries &ts, int numOfLine, const correlatedFeatures &cfCheck) override;
    virtual void completeStruct(const TimeSeries &ts, correlatedFeatures &c);
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
