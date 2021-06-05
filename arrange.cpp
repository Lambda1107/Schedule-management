#include "schedule.h"
#include "arrange.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iterator>
using namespace std;
arrange::arrange()
{
}
arrange::arrange(string _name, string _site, timeDate _DDLDate, timeScale _rankTime, vector<int> &_rankWeeks, vector<int> &_rankWDays, string _remark)
{
    name = _name;
    site = _site;
    DDLDate = _DDLDate;
    rankTime = _rankTime;
    sort(_rankWeeks.begin(), _rankWeeks.end());
    _rankWeeks.erase(unique(_rankWeeks.begin(), _rankWeeks.end()), _rankWeeks.end());
    rankWeeks = _rankWeeks;
    sort(_rankWDays.begin(), _rankWDays.end());
    _rankWDays.erase(unique(_rankWDays.begin(), _rankWDays.end()), _rankWDays.end());
    rankWDays = _rankWDays;
    remark = _remark;
}

arrange::~arrange()
{
}
timeDate arrange::getRecentDate(timeDate theDate)
{
    timeDate theDate1 = theDate;
    int tmpWDay = getWday(theDate);
    bool b = 0;
    for (auto theWDay : rankWDays)
    {
        if (theWDay >= tmpWDay)
        {
            b = 1;
            tmpWDay = theWDay;
            break;
        }
    }
    if (!b && rankWDays.size() > 0)
    {
        tmpWDay = rankWDays[0];
        theDate = theDate + rankWDays[0] + 7 - getWday(theDate);
    }
    b = 0;
    int tmpWeek = getWeek(theDate);
    for (auto theWeek : rankWeeks)
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
    else if (DDLDate < theDate1)
        return -1;
    else
        return DDLDate;
    //返回最近的安排时间，如果没有就返回DDL时间，如果DDL时间已经过了就返回-1
}

timeMonent arrange::getStartTime()
{
    return rankTime.startTime;
}
timeMonent arrange::getEndTime()
{
    return rankTime.endTime;
}

string arrange::printOut()
{
    if (rankWeeks.size() == 0 || rankWDays.size() == 0)
    {
        return "before " + getDataText(DDLDate) + " " + name + " at " + site + " (" + remark + ")";
    }
    else
    {
        return getTimeMonentText(getStartTime()) + " to " + getTimeMonentText(getEndTime()) + " " + name + " at " + site + " (" + remark + ")";
    }
}

vector<int> arrange::getWeekRank()
{
    return rankWeeks;
}

vector<int> arrange::getWDayRank()
{
    return rankWDays;
}

arrange *arrange::addArrange()
{
    string _name, _site, str, _remark;
    vector<int> _weekdays(0);
    vector<int> _weeks(0);
    timeScale _rankTime;
    timeDate _DDLDate = 0;
    do
    {
        cout << "是否安排时间？（yes or no）:";
        cin >> str;
    } while (str != "yes" && str != "no");

    if (str == "yes")
    {
        cout << "事项在哪几周？（有多周可用逗号隔开）：";
        cin.ignore(1, '\n');
        getline(cin, str);
        _weeks = toIntVec(splitString(str, ','));

        cout << "事项在周几？（有多天可用逗号隔开）：";
        getline(cin, str);
        _weekdays = toIntVec(splitString(str, ','));

        cout << "事项开始时间？（时 分）：";
        int hou, min;
        cin >> hou >> min;
        _rankTime.startTime = hou * 60 + min;
        cout << "事项耗时？（时 分）：";
        cin >> hou >> min;
        _rankTime.endTime = _rankTime.startTime + hou * 60 + min;
    }
    int year, mon, day;
    time_t t = 0;
    tm *tmpDate = localtime(&t);
    cout << "请输入事项截止日期（年 月 日）：";
    cin >> year >> mon >> day;
    tmpDate->tm_year = year - 1900;
    tmpDate->tm_mon = mon - 1;
    tmpDate->tm_mday = day;
    //核心
    _DDLDate = mktime(tmpDate) / (24 * 3600); //取得日期时间戳
    //核心
    cin.ignore(1, '\n');
    cout << "请输入事件名称：";
    getline(cin, _name);
    cout << "请输入事件地点：";
    getline(cin, _site);
    cout << "请输入备注：";
    getline(cin, _remark);
    return (new arrange(_name, _site, _DDLDate, _rankTime, _weeks, _weekdays, _remark));
}

void arrange::eraseWeekRank(int week)
{
    vector<int>::iterator ite = rankWeeks.begin();
    while (ite != rankWeeks.end())
    {
        if ((*ite) == week)
        {
            rankWeeks.erase(ite);
            break;
        }
        ite++;
    }
}

void arrange::eraseWDayRank(int wday)
{
    vector<int>::iterator ite = rankWDays.begin();
    while (ite != rankWDays.end())
    {
        if ((*ite) == wday)
        {
            rankWDays.erase(ite);
            break;
        }
        ite++;
    }
}

string arrange::store()
{
    string str = "";
    str = str + "2\n" +
          name + "\n" +
          site + "\n" +
          to_string(DDLDate) + "\n" +
          to_string(rankTime.startTime) + "\n" +
          to_string(rankTime.endTime) + "\n" +
          remark + "\n";
    for (auto a : rankWDays)
    {
        str += to_string(a);
        str += ",";
    }
    str += '\n';
    for (auto a : rankWeeks)
    {
        str += to_string(a);
        str += ",";
    }
    return str;
}
void arrange::load(istream &fin)
{
    string str;
    fin.ignore(1, '\n');
    getline(fin, name);
    getline(fin, site);
    fin >> DDLDate >> rankTime.startTime >> rankTime.endTime;
    fin.ignore(1, '\n');
    getline(fin, remark);
    getline(fin, str);
    rankWDays = toIntVec(splitString(str));
    getline(fin, str);
    rankWeeks = toIntVec(splitString(str));
}
schedule *arrange::reset(schedule *sp, timeDate theData)
{
    arrange *tmpArrangeP = new arrange();
    if (sp != NULL && theData != 0)
    {
        *tmpArrangeP = *(arrange *)sp;
        vector<int> tmpWeekRank;
        tmpWeekRank.push_back(getWeek(theData));
        tmpArrangeP->rankWeeks = tmpWeekRank;
        vector<int> tmpWDayRank;
        tmpWeekRank.push_back(getWday(theData));
        tmpArrangeP->rankWDays = tmpWDayRank;
    }
    else
    {
        delete tmpArrangeP;
        tmpArrangeP = this;
    }
    cout << "您想修改什么？" << endl
         << "1、安排时间 2、DDL时间 3、事项名  4、事项地点 5、备注" << endl;
    int option;
    cin >> option;

    switch (option)
    {
    case 1:
    {
        timeScale _rankTime;
        string str;
        vector<int> _weeks, _weekdays;
        cout << "事项在哪几周？（有多周可用逗号隔开）：";
        cin.ignore(1, '\n');
        getline(cin, str);
        _weeks = toIntVec(splitString(str, ','));

        cout << "事项在周几？（有多天可用逗号隔开）：";
        getline(cin, str);
        _weekdays = toIntVec(splitString(str, ','));

        cout << "事项开始时间？（时 分）：";
        int hou, min;
        cin >> hou >> min;
        _rankTime.startTime = hou * 60 + min;
        cout << "事项耗时？（时 分）：";
        cin >> hou >> min;
        _rankTime.endTime = _rankTime.startTime + hou * 60 + min;

        tmpArrangeP->rankTime = _rankTime;
        tmpArrangeP->rankWDays = _weekdays;
        tmpArrangeP->rankWeeks = _weeks;
        break;
    }
    case 2:
        cout << "请输入DDL时间：";
        cin >> tmpArrangeP->DDLDate;
        break;
    case 3:
        cout << "请输入新事项名：";
        cin.ignore(1, '\n');
        getline(cin, tmpArrangeP->name);
        break;
    case 4:
        cout << "请输入新事项地点：";
        cin.ignore(1, '\n');
        getline(cin, tmpArrangeP->site);
        break;
    case 5:
        cout << "请输入新备注：";
        cin.ignore(1, '\n');
        getline(cin, tmpArrangeP->remark);
        break;
    default:
        break;
    }

    return tmpArrangeP;
}