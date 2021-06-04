#include "schedule.h"
#include "course.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

vector<int> course::getWeekRank()
{
    return courseWeeks;
}

void course::eraseWeekRank(int week)
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

void course::eraseWDayRank(int wday)
{
    return; //因为课程是单独每天算的，啥也不用动
}

timeMonent course::getStartTime()
{
    return g_timeTab[rank - 1].startTime;
}

timeMonent course::getEndTime()
{
    return g_timeTab[rank + rankNum - 2].endTime;
}

string course::printOut()
{
    return getTimeMonentText(getStartTime()) + " to " + getTimeMonentText(getEndTime()) + " " + name + " at " + site;
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
    cin >> weekday;
    cout << "请输入课程是第几节课开始(1-" << g_timeTab.size() << ")：";
    cin >> scheduleRank;
    cout << "请输入总共持续几节课：(1-" << g_timeTab.size() - scheduleRank + 1 << ")：";
    cin >> scheduleNum;
    cout << "请输入课程名：";
    cin.ignore(1, '\n');
    getline(cin, name);
    cout << "请输入课程地点：";
    getline(cin, site);
    cout << "请输入上课周数(以逗号分隔)：";
    getline(cin, str);
    vector<int> result = toIntVec(splitString(str, ','));
    //result向量就是上课周数
    return (new course(name, site, scheduleRank, scheduleNum, result, weekday));
}

vector<int> course::getWDayRank()
{
    vector<int> vec(1);
    vec[0] = courseWeekDay;
    return vec;
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
        tmpCourseP = this;
    }
    cout << "您想修改什么？" << endl
         << "1、时间  2、课程名  3、课程地点" << endl;
    int option;
    cin >> option;

    switch (option)
    {
    case 1:
    {
        int weekday, scheduleRank, scheduleNum;
        string str;
        vector<int> result;
        cout << "请输入课程在周几(1-7)：";
        cin >> weekday;
        cout << "请输入课程是第几节课开始(1-" << g_timeTab.size() << ")：";
        cin >> scheduleRank;
        cout << "请输入总共持续几节课：(1-" << g_timeTab.size() - scheduleRank + 1 << ")：";
        cin >> scheduleNum;
        cout << "请输入上课周数(以逗号分隔)：";
        cin.ignore(1, '\n');
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
        cin.ignore(1, '\n');
        getline(cin, tmpCourseP->name);
        break;
    case 3:
        cout << "请输入课程地点：";
        cin.ignore(1, '\n');
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
    str = str + "1\n" + name + "\n" + site + "\n" + to_string(rank) + "\n" + to_string(rankNum) + "\n" + to_string(courseWeekDay) + "\n";
    for (auto a : courseWeeks)
    {
        str += to_string(a);
        str += ",";
    }
    return str;
}
void course::load(istream &fin)
{
    string str;
    fin.ignore(1, '\n');
    getline(fin, name);
    getline(fin, site);
    fin >> rank >> rankNum >> courseWeekDay;
    fin.ignore(1, '\n');
    getline(fin, str);
    vector<int> result = toIntVec(splitString(str, ','));
    //result向量就是上课周数
    courseWeeks = result;
}

course::~course()
{
}