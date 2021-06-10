#include <string>
#include <vector>
#include <ctime>
#include "schedule.h"
#ifndef WORK_H
#define WORK_H
class work : public schedule
{
public:
    string name;
    timeDate DDLDate;
    timeDateMonent rankTime;
    timeMonent timeLong;
    string remark;
    work();
    work(string _name, string _remark, timeDate _DDLDate, timeDateMonent _rankTime, timeMonent _timeLong);
    ~work();
    timeMonent getStartTime();
    timeDate getRecentDate(timeDate theDate);
    string printOut(timeDate theDate = 0);
    vector<timeDate> getRankDate();
    void eraseRankDate(timeDate theRankDate);
    schedule *reset(schedule *sp = NULL, timeDate theData = 0);
    string store();
    void load(istream &fin);
    static work *addWork();
};
#endif