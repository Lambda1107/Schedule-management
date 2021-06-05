#include <string>
#include <vector>
#include <ctime>
#include "schedule.h"
using namespace std;
#ifndef COURSE_H
#define COURSE_H

class course : public schedule
{
private:
    string name;
    string site;
    int rank;
    int rankNum;
    int courseWeekDay;
    vector<int> courseWeeks;

public:
    course();
    course(string _name, string _site, int _rank, int _rankNum, vector<int> &_courseWeeks, int _courseWeekDay);
    ~course();
    timeMonent getStartTime();
    timeMonent getEndTime();
    string printOut(timeDate theDate = 0);
    vector<timeDate> getRankDate();
    timeDate getRecentDate(timeDate theDate);
    void eraseRankDate(timeDate theRankDate);
    static course *addCourse();
    string store();
    void load(istream &fin);
    schedule *reset(schedule *sp = NULL, timeDate theData = 0);
};

#endif