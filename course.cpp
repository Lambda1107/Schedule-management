#include "main.h"
#include "course.h"
#include <iostream>
#include <vector>
using namespace std;
string getTimeMonentText(timeMonent tim);

//全局变量
extern vector<plan> globalPlan;     //每天计划的集合
extern int g_courseLong;            //每节课时长 单位分钟
extern timeDate g_startDate;        //计划开始日期
extern vector<timeScale> g_timeTab; //每天时间表

vector<int> course::isMut()
{
    return courseWeeks;
}

void course::eraseWRank(int week)
{
    vector<int>::iterator ite = courseWeeks.begin();
    while (ite != courseWeeks.end())
    {
        if ((*ite) == week)
        {
            courseWeeks.erase(ite);
            break;
        }
        ite++;
    }
}

timeMonent course::getStartTime()
{
    return g_timeTab[rank - 1].startTime;
}

timeMonent course::getEndTime()
{
    return g_timeTab[rank + rankNum - 2].endTime;
}
void course::printOut()
{
    cout << getTimeMonentText(getStartTime()) << " to " << getTimeMonentText(getEndTime()) << " " << name << " at " << site << endl;
}
course::course()
{
}
course::course(string _name, string _site, int _rank, int _rankNum, vector<int> &_courseWeeks)
{
    name = _name;
    site = _site;
    rank = _rank;
    rankNum = _rankNum;
    courseWeeks = _courseWeeks;
}

course::~course()
{
}