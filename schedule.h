#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <ctime>

using namespace std;
#ifndef SCHEDULE_H
#define SCHEDULE_H

typedef unsigned long long timeDate;
typedef unsigned int timeMonent;
typedef unsigned long long timeDateMonent;

struct timeScale
{
    timeMonent startTime;
    timeMonent endTime;
};

class schedule
{
protected:
    map<timeDate,bool> isSubmit;

public:
    schedule() {}
    virtual ~schedule() {}
    virtual timeMonent getStartTime() = 0;
    virtual timeDate getRecentDate(timeDate theDate) = 0;
    virtual string printOut(timeDate theDate = 0) = 0;
    virtual vector<timeDate> getRankDate() = 0;
    virtual void eraseRankDate(timeDate theRankDate) = 0;
    virtual schedule *reset(schedule *sp = NULL, timeDate theData = 0) = 0;
    virtual string store() = 0;
    virtual void load(istream &fin) = 0;
    virtual void submit(timeDate theDate = -1);
};

struct plan
{
    timeDate Date;
    vector<schedule *> scheduleList;
};

//全局变量
extern vector<plan> globalPlan;                 //每天计划的集合
extern vector<schedule *> globalAllSchedule;    //所有计划的集合
extern vector<schedule *> globalNoPlanSchedule; //所有没安排时间计划的集合
extern int g_scheduleLong;                      //每节课时长 单位分钟
extern timeDate g_startDate;                    //计划开始日期
extern vector<timeScale> g_timeTab;             //每天时间表

//全局函数
bool strToVar(string str);
template <typename T, typename... Ts>
bool strToVar(string str, T &var, Ts &...Vars)
{
    while (*(str.begin()) == ' ' || *(str.begin()) == '\n')
        str.erase(0, 1);
    if (*(str.begin()) == 'q' || *(str.begin()) == 'Q')
        return true;
    str = "0 " + str;
    istringstream ss(str);
    ss >> var;
    ss >> var;
    size_t i = str.find(' ', 2);
    if (i == str.npos)
        i = str.length();
    str.erase(0, i);
    strToVar(str, Vars...);
    return false;
}

template <typename... Ts>
bool getLineVar(istream &is, Ts &...Vars)
{
    string str;
    getline(is, str);
    return strToVar(str, Vars...);
}

string getTimeMonentText(timeMonent tim, char syb = ':');
void insertPlan(schedule *tmpSchedule);
vector<string> splitString(string str, char syb = ',');
int getWday(timeDate td);
int getWeek(timeDate td);
void removeNoPlanSchedule(schedule *tmpSchedule);
string getDataText(timeDate a);
vector<int> toIntVec(vector<string> str);
vector<timeDate> toDateVec(vector<string> str);
#endif