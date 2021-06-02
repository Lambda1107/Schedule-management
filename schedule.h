#include <string>
#include <vector>
#include <ctime>
using namespace std;
#ifndef SCHEDULE_H
#define SCHEDULE_H

typedef unsigned long long timeDate;
typedef unsigned int timeMonent;

struct timeScale
{
    timeMonent startTime;
    timeMonent endTime;
};

class schedule
{
public:
    schedule() {}
    virtual ~schedule() {}
    virtual timeMonent getStartTime() = 0;
    virtual void printOut() = 0;
    virtual vector<int> getWeekRank() = 0;
    virtual vector<int> getWDayRank() = 0;
    virtual void eraseWeekRank(int week) = 0;
    virtual void eraseWDayRank(int wday) = 0;
    virtual schedule *reset(schedule *sp = NULL, timeDate theData = 0) = 0;
};

struct plan
{
    timeDate Date;
    vector<schedule *> scheduleList;
};

//全局变量
extern vector<plan> globalPlan;     //每天计划的集合
extern int g_scheduleLong;          //每节课时长 单位分钟
extern timeDate g_startDate;        //计划开始日期
extern vector<timeScale> g_timeTab; //每天时间表

string getTimeMonentText(timeMonent tim);
int getWday(timeDate td);
int getWeek(timeDate td);
#endif