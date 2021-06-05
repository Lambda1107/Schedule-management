#include <string>
#include <vector>
#include <ctime>
#include "schedule.h"
#ifndef ARRANGE_H
#define ARRANGE_H
class arrange : public schedule
{
private:
    string name;
    string site;
    string remark;
    timeDate DDLDate;
    timeScale rankTime;
    vector<timeDate> rankDate;

public:
    arrange();
    arrange(string _name, string _site, timeDate _DDLDate, timeScale _rankTime, vector<timeDate> &_rankDate, string _remark);
    ~arrange();
    timeDate getRecentDate(timeDate theDate);
    timeMonent getStartTime();
    string printOut(timeDate theDate = 0);
    void eraseRankDate(timeDate theRankDate);
    vector<timeDate> getRankDate();
    static arrange *addArrange();
    string store();
    void load(istream &fin);
    schedule *reset(schedule *sp = NULL, timeDate theData = 0);
};
#endif