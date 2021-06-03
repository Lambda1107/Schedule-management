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
    timeDate DDLDate;
    timeScale rankTime;
    string remark;
    vector<int> rankWDays;
    vector<int> rankWeeks;

public:
    arrange();
    arrange(string _name, string _site, timeDate _DDLDate, timeScale _rankTime, vector<int> &_rankWeeks, vector<int> &_rankWDays, string _remark);
    ~arrange();
    timeDate getRecentDate(timeDate theDate);
    timeMonent getStartTime();
    timeMonent getEndTime();
    string printOut();
    vector<int> getWeekRank();
    vector<int> getWDayRank();
    void eraseWeekRank(int week);
    void eraseWDayRank(int wday);
    static arrange *addArrange();
    string store();
    void load(istream &fin);
    schedule *reset(schedule *sp = NULL, timeDate theData = 0);
};
#endif