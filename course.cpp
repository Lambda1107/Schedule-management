#include "schedule.h"
#include "course.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

timeMonent course::getStartTime()
{
    return g_timeTab[rank - 1].startTime;
}

timeMonent course::getEndTime()
{
    return g_timeTab[rank + rankNum - 2].endTime;
}

string course::printOut(timeDate theDate)
{
    string str;
    str += getTimeMonentText(getStartTime()) + " to " + getTimeMonentText(getEndTime()) + " " + name;
    if (site != "" && site != " " && site != "无")
        str += " at " + site;
    if (isSubmit[theDate])
    {
        str += "   已完成";
    }
    return str;
}

course::course()
{
}

course::course(string _name, string _site, int _rank, int _rankNum, vector<int> &_courseWeeks, int _courseWeekDay)
{
    name = _name;
    site = _site;
    rank = _rank;
    rankNum = _rankNum;
    sort(_courseWeeks.begin(), _courseWeeks.end());
    _courseWeeks.erase(unique(_courseWeeks.begin(), _courseWeeks.end()), _courseWeeks.end());
    courseWeeks = _courseWeeks;
    courseWeekDay = _courseWeekDay;
}

course *course::addCourse()
{
    int weekday, scheduleRank, scheduleNum;
    string name, site, str;
    cout << "请输入课程在周几(1-7)：";
    if (getLineVar(cin, weekday))
        return NULL;
    cout << "请输入课程是第几节课开始(1-" << g_timeTab.size() << ")：";
    if (getLineVar(cin, scheduleRank))
        return NULL;
    cout << "请输入总共持续几节课：(1-" << g_timeTab.size() - scheduleRank + 1 << ")：";
    if (getLineVar(cin, scheduleNum))
        return NULL;
    cout << "请输入课程名：";
    getline(cin, name);
    cout << "请输入课程地点：";
    getline(cin, site);
    cout << "请输入上课周数(以逗号分隔)：";
    getline(cin, str);
    vector<int> result = toIntVec(splitString(str, ','));
    //result向量就是上课周数
    return (new course(name, site, scheduleRank, scheduleNum, result, weekday));
}

timeDate course::getRecentDate(timeDate theDate)
{
    int tmpWDay = getWday(theDate);
    while (tmpWDay != courseWeekDay)
    {
        tmpWDay++;
        theDate++;
        if (tmpWDay > 7)
        {
            tmpWDay = 1;
        }
    }
    bool b = 0;
    int tmpWeek = getWeek(theDate);
    for (auto theWeek : courseWeeks)
    {
        if (theWeek >= tmpWeek)
        {
            b = 1;
            tmpWeek = theWeek;
            break;
        }
    }
    if (b)
        return g_startDate + tmpWeek * 7 + tmpWDay - 8;
    else
        return -1;
}
schedule *course::reset(schedule *sp, timeDate theData)
{
    course *tmpCourseP;
    if (sp != NULL && theData != 0)
    {
        tmpCourseP = new course();
        *tmpCourseP = *(course *)sp;
        vector<int> tmpWeekRank;
        tmpWeekRank.push_back(getWeek(theData));
        tmpCourseP->courseWeeks = tmpWeekRank;
    }
    else
    {
        delete tmpCourseP;
        tmpCourseP = this;
    }
    cout << "您想修改什么？" << endl
         << "1、时间  2、课程名  3、课程地点" << endl;
    int option;
    if (getLineVar(cin, option))
        return NULL;

    switch (option)
    {
    case 1:
    {
        int weekday, scheduleRank, scheduleNum;
        string str;
        vector<int> result;
        cout << "请输入课程在周几(1-7)：";
        if (getLineVar(cin >> weekday))
            return NULL;
        cout << "请输入课程是第几节课开始(1-" << g_timeTab.size() << ")：";
        if (getLineVar(cin >> scheduleRank))
            return NULL;
        cout << "请输入总共持续几节课：(1-" << g_timeTab.size() - scheduleRank + 1 << ")：";
        if (getLineVar(cin >> scheduleNum))
            return NULL;
        cout << "请输入上课周数(以逗号分隔)：";
        getline(cin, str);
        result = toIntVec(splitString(str));
        // result向量就是上课周数
        tmpCourseP->courseWeekDay = weekday;
        tmpCourseP->courseWeeks = result;
        tmpCourseP->rank = scheduleRank;
        tmpCourseP->rankNum = scheduleNum;
        break;
    }
    case 2:
        cout << "请输入课程名：";

        getline(cin, tmpCourseP->name);
        break;
    case 3:
        cout << "请输入课程地点：";

        getline(cin, tmpCourseP->site);
        break;
    default:
        break;
    }

    return tmpCourseP;
}
string course::store()
{
    string str = "";
    str = str + "1\n" + name + "\n" + site + "\n" + to_string(rank) + " " + to_string(rankNum) + " " + to_string(courseWeekDay) + "\n";
    for (auto a : courseWeeks)
    {
        str += to_string(a);
        str += ",";
    }
    str += "\n";
    for (auto a : isSubmit)
    {
        str += to_string(a.first);
        str += ",";
    }
    return str;
}
void course::load(istream &fin)
{
    string str;
    getline(fin, name);
    getline(fin, site);
    getLineVar(fin, rank, rankNum, courseWeekDay);
    getline(fin, str);
    courseWeeks = toIntVec(splitString(str, ','));
    getline(fin, str);
    vector<timeDate> tmpDateVec = toDateVec(splitString(str));
    for (auto a : tmpDateVec)
    {
        isSubmit[a] = true;
    }
}

course::~course()
{
}

vector<timeDate> course::getRankDate()
{
    vector<timeDate> tmpRankDate(0);
    for (auto week : courseWeeks)
    {
        tmpRankDate.push_back(week * 7 + courseWeekDay - 8 + g_startDate);
    }
    return tmpRankDate;
}

void course::eraseRankDate(timeDate theRankDate)
{
    int tmpWeek = getWeek(theRankDate);
    auto findIt = find(courseWeeks.begin(), courseWeeks.end(), theRankDate);
    courseWeeks.erase(findIt);
}