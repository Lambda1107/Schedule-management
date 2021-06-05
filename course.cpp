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
    cout << "������γ����ܼ�(1-7)��";
    if (getLineVar(cin, weekday))
        return NULL;
    cout << "������γ��ǵڼ��ڿο�ʼ(1-" << g_timeTab.size() << ")��";
    if (getLineVar(cin, scheduleRank))
        return NULL;
    cout << "�������ܹ��������ڿΣ�(1-" << g_timeTab.size() - scheduleRank + 1 << ")��";
    if (getLineVar(cin, scheduleNum))
        return NULL;
    cout << "������γ�����";
    getline(cin, name);
    cout << "������γ̵ص㣺";
    getline(cin, site);
    cout << "�������Ͽ�����(�Զ��ŷָ�)��";
    getline(cin, str);
    vector<int> result = toIntVec(splitString(str, ','));
    //result���������Ͽ�����
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
    cout << "�����޸�ʲô��" << endl
         << "1��ʱ��  2���γ���  3���γ̵ص�" << endl;
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
        cout << "������γ����ܼ�(1-7)��";
        if (getLineVar(cin >> weekday))
            return NULL;
        cout << "������γ��ǵڼ��ڿο�ʼ(1-" << g_timeTab.size() << ")��";
        if (getLineVar(cin >> scheduleRank))
            return NULL;
        cout << "�������ܹ��������ڿΣ�(1-" << g_timeTab.size() - scheduleRank + 1 << ")��";
        if (getLineVar(cin >> scheduleNum))
            return NULL;
        cout << "�������Ͽ�����(�Զ��ŷָ�)��";
        getline(cin, str);
        result = toIntVec(splitString(str));
        // result���������Ͽ�����
        tmpCourseP->courseWeekDay = weekday;
        tmpCourseP->courseWeeks = result;
        tmpCourseP->rank = scheduleRank;
        tmpCourseP->rankNum = scheduleNum;
        break;
    }
    case 2:
        cout << "������γ�����";

        getline(cin, tmpCourseP->name);
        break;
    case 3:
        cout << "������γ̵ص㣺";

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
    getline(fin, name);
    getline(fin, site);
    getLineVar(fin, rank, rankNum, courseWeekDay);
    getline(fin, str);
    vector<int> result = toIntVec(splitString(str, ','));
    courseWeeks = result;
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