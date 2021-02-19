

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;
/*******************************         CommandsHelper  class             ************************************/
/**  helper class for the commands - contains methods and variables relevant to all the different commands,
 * so each command have reference to this class **/
class CommandsHelper {
    HybridAnomalyDetector *ad;
    vector<AnomalyReport> ar;
    long lines_num;
public:
    /** constructor and destructor: **/
    CommandsHelper() {this->ad = new HybridAnomalyDetector(); }
    virtual ~CommandsHelper() {};

    /** setters and getters: **/
    HybridAnomalyDetector *getHybridAD() const {return ad; }
    void setHybridAD(HybridAnomalyDetector *had) {CommandsHelper::ad = had; }

    vector<AnomalyReport> getAr()  {return this->ar;}
    void setAr(vector<AnomalyReport> arr) { ar = std::move(arr); }

    long getLinesNum() const {return lines_num; }
    void setLinesNum(long linesNum) { lines_num = linesNum; }
};
/*******************************            DefaultIO  class              ********************************/
/***      abstract class for read and write from the user      **/
class DefaultIO {
public:

    virtual string read() =0;
    virtual void write(string text) =0;
    virtual void write(float f) =0;
    virtual void read(float *f) =0;
    virtual ~DefaultIO() {};
    void uploadFile(string file_name) {
        string line = read();
        ofstream file(file_name);
        while (line != "done") {
            file << line << endl;
            line = read();
        }
    }
};
/*******************************            StandardIO  class              **************************************/
/** derived class for read and write from the user in the standart way **/
class StandardIO : public DefaultIO {
public:
    StandardIO(){};
    string read() override {
        string s;
        cin >> s;
        return s;
    }
    void read(float *f) override {
        cin >> *f;
    }
    void write(string text) override {
        cout << text;
    }
    void write(float f) override {
        cout << f;
    }

    virtual ~StandardIO(){};
};
/********************************         command   class           ***********************************/
/** an abstract class for all the command **/
class Command {
protected:
    DefaultIO* dio;
    string description;
    CommandsHelper *ch;
public:
    Command(DefaultIO* dio, string description, CommandsHelper* ch):dio(dio), description(description), ch(ch){}
    Command()= default;
    virtual void execute()=0;
    ~Command() {}
    // get the description of the command
    string getDescription() {return this->description; }

};
/************************************        command 1 - upload csv          ************************************/
/** class for uploading a csv file, which given by the user */
class Upload_csv : public Command {
public:
    /** constructor and destructor: **/
    Upload_csv(DefaultIO *dio, CommandsHelper *ch) : Command(dio, "1.upload a time series csv file", ch){}
    ~Upload_csv() = default;
    /** function for uploading file */
    void uploadFile(string file_name) {
        string line = dio->read();
        ofstream file(file_name);
        while (line != "done") {
            file << line << endl;
            line = dio->read();
        }
    }
    /** the execute function of upload csv file */
    void execute() override {
        // uploading the first file:
        this->dio->write("Please upload your local train CSV file.\n");
        uploadFile("anomalyTrain.csv");
        this->dio->write("Upload complete.\n");

        // uploading the second file:
        this->dio->write("Please upload your local test CSV file.\n");
        uploadFile("anomalyTest.csv");
        this->dio->write("Upload complete.\n");
    }
};
/********************************        comand 2 - change the threshold          ************************************/
/** this command change the correlate threshold and set a new one that the user chose. */
class changeThreshold : public Command {
public:
    //constructor:
    changeThreshold(DefaultIO *dio, CommandsHelper *ch) : Command(dio, "2.algorithm settings", ch) {}

    /** the execute function of upload csv file */
    float getNewThreshold() {
        float temp;
        dio->read(&temp) ;
        if((temp < 1) && (temp > 0))
            return temp;
        else {
            cout << "please choose a value between 0 and 1.\n" << endl;
            return getNewThreshold();
        }
    }
    /** execute function **/
    void execute() {
        // get the new threshold from the user
        dio->write("The current correlation threshold is ") ;
        dio->write(this->ch->getHybridAD()->SimpleAnomalyDetector::getThreshold());
        dio->write("\n");
        float new_threshold = getNewThreshold();
        dio->write("Type a new threshold\n");
        this->ch->getHybridAD()->setMaxCorrelate(new_threshold);
    }
};

/********************************        comand 3 - detect the anomalies          ************************************/
/** this command detect the anomalies with the new thresold value. */
class detectAnomalies : public Command {
public:
    //constructor:
    detectAnomalies(DefaultIO *dio, CommandsHelper *ch) : Command(dio, "3.detect anomalies", ch) {}
    void execute() override {
        TimeSeries ts_train("anomalyTrain.csv");
        TimeSeries ts_test("anomalyTest.csv");
        ch->getHybridAD()->learnNormal(ts_train);
        ch->setAr(ch->getHybridAD()->detect(ts_test));
        ch->setLinesNum(ts_test.getLineNum());
        dio->write("anomaly detection complete.\n");
    }
};

/********************************        command 4 - display results          ************************************/
/** this command display to the user the result - when there is an exception and who's involved. */
class displayResults : public Command {
public:
    //constructor:
    displayResults(DefaultIO *dio, CommandsHelper *ch) : Command(dio, "4.display results", ch) {}

    void execute() {
        vector<AnomalyReport>::iterator iterator;
        vector<AnomalyReport> ar = ch->getAr();
        for (iterator = ar.begin(); iterator != ar.end(); ++iterator) {
            dio->write(to_string(iterator->timeStep) + "\t " + iterator->description + "\n");
        }
        dio->write("Done.\n");
    }
};

/****************************        command 5 - upload and analyze results          **********************************/
/** In this command, The client will upload an anomaly file to the server, and the server
 * will analyze the results of the algorithm and present them to the client.  */
class analyzeResults : public Command {
public:
    /**  constructor  **/
    analyzeResults(DefaultIO *dio, CommandsHelper *ch) : Command(dio, "5.upload anomalies and analyze results", ch) {}
    /**  method to check if a string contain only digits  **/
    bool is_digits(const std::string &str)
    {
         if(str.length() == 0 ||  !(all_of(str.begin(), str.end(), ::isdigit))) return false;
        return true;
    }
    /**  method to check if a string contain only digits  **/
    void threeAfterDot(float &number) {
        number = floorf(1000 * number) / 1000;
        string s = to_string(number);
        s.erase(s.find_last_not_of('0') + 1, string::npos);
        size_t sSize = s.size() - 1;
        if(s[sSize] =='.') s.substr(0, sSize);
    }
    /**  method to create data struct of kind map contain the data from a anomaly txt file   **/
    map<long, long> createMapFromFile(float &linesWithout) {
        string line;
        map<long,long> tempMap;
        while (line != "done") {
            line = dio->read();
            if (line == "done" || line == "") break;
            string start, end;
            int comma = line.find(',');
            start = line.substr(0, comma);
            end = line.substr(comma + 1, line.length() - comma);
            if(!(is_digits(start)) || !(is_digits(end))) {
                continue;
            }
            tempMap[stol(start)] = stol(end);

            linesWithout  -= (stol(end) - stol(start) + 1);
        }
        return tempMap;
    }
    /**  method to create data struct of kind map contain all the union data of the anomalies the detector founds   **/
       map<long, long > unionAnomalies() {
        long start, end, counter;
        map<long, long> reports;
        vector<AnomalyReport> ar = ch->getAr();
        vector<AnomalyReport>::iterator it_start = ar.begin();
        vector<AnomalyReport>::iterator it_end = ar.begin();
        for (; it_start != ar.end(); it_start = it_end) {
            start = it_start->timeStep;
            for (++it_end, counter = 1; it_end != ar.end(); ++it_end, ++counter) {
                if (it_end->timeStep == it_start->timeStep + counter) continue;
                else break;
            }
            if (counter == 1) ++counter;
            end = start + counter - 1;
            reports[start] = end;
        }
        return reports;
    }

    void execute() {
        // variables declaration:
        float P = 0; // number of anomalies reported in the file
        float TP = 0; // true positive
        float FP = 0; // false positive
        // number of anomalies occurrences (represent by the total number of lines minus the anomalies occurance lines)
        long n = float(ch->getLinesNum());  //number of all reports (represend by the number of lines in the time series).
        float N = n; // number of negative anomalies
        float intersect_counter = 0;
        map<long, long> unionReport, txtAnomalies;
        dio->write("Please upload your local anomalies file.\n");
        dio->write("Upload complete.\n");
        // create a map with the union anomalies data from the past report
        txtAnomalies = createMapFromFile(N);
        unionReport = unionAnomalies();
        P = txtAnomalies.size();
        map<long, long>::iterator it1, it2;
        // check if there is intersection between the anomaly reports to the detector reports
        for (it1 = txtAnomalies.begin(); it1 != txtAnomalies.end(); ++it1) {
            long true_start = it1->first, true_end = it1->second;
            for (it2 = unionReport.begin(); it2 != unionReport.end(); ++it2) {
                long maybe_start = it2->first, maybe_end = it2->second;
                // check for intersection, the 2 first terms is for partial intersection and the other is for full
                if ((true_start >= maybe_start && true_start <= maybe_end) ||
                    (true_start <= maybe_start && true_end >= maybe_end) ||
                    (true_end >= maybe_start && true_end <= maybe_end) ||
                    (maybe_start <= true_start && maybe_end >= true_end)) {
                    ++TP;
                    intersect_counter += 1 ;
                }
            }
        }

        FP = unionReport.size() - intersect_counter;
        float true_positive_rate = TP / P;
        float false_alarm_rate = FP / N;
        dio->write("True Positive Rate: ");
        threeAfterDot(true_positive_rate);
        dio->write(true_positive_rate);
        dio->write("\n");
        dio->write("False Positive Rate: ");
        threeAfterDot(false_alarm_rate);


        dio->write(false_alarm_rate);
        dio->write("\n");
    }
};
#endif /* COMMANDS_H_ */
