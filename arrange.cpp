#include "schedule.h"
#include "arrange.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
arrange::arrange()
{
}

arrange::arrange(string _name, string _site, timeDate _DDLDate, timeScale _rankTime, vector<timeDate> &_rankDate, string _remark)
{
    name = _name;
    site = _site;
    DDLDate = _DDLDate;
    rankTime = _rankTime;
    sort(_rankDate.begin(), _rankDate.end());
    _rankDate.erase(unique(_rankDate.begin(), _rankDate.end()), _rankDate.end());
    rankDate = _rankDate;
    remark = _remark;
}

arrange::~arrange()
{
}
timeDate arrange::getRecentDate(timeDate theDate)
{
    auto it_rankDate = lower_bound(rankDate.begin(), rankDate.end(), theDate + 1);
    if (theDate > DDLDate)
        return -1;
    else if (it_rankDate == rankDate.end())
        return DDLDate;
    else
        return *it_rankDate;

    //返回最近的安排时间，如果没有就返回DDL时间，如果DDL时间已经过了就返回-1
}

timeMonent arrange::getStartTime()
{
    return rankTime.startTime;
}

string arrange::printOut(timeDate theDate)
{
    string str;
    if (rankDate.size() == 0)
    {
        str += "before " + getDataText(DDLDate) + " " + name;
    }
    else
    {
        str += getTimeMonentText(getStartTime()) + " to " + getTimeMonentText(rankTime.endTime) + " " + name;
    }
    if (site != "" && site != " " && site != "无")
        str += " at " + site;
    if (remark != "" && remark != " " && remark != "无")
        str += " (" + remark + ")";
    if (isSubmit[theDate])
        str += "   已完成";
    return str;
}

arrange *arrange::addArrange()
{
    string _name, _site, str, _remark;
    vector<int> _weekdays(0);
    vector<int> _weeks(0);
    timeScale _rankTime = {};
    timeDate _DDLDate = 0;

    do
    {
        cout << "是否安排时间？（yes or no）:";
        if (getLineVar(cin, str))
            return NULL;
    } while (str != "yes" && str != "no");

    if (str == "yes")
    {
        cout << "事项在哪几周？（有多周可用逗号隔开）：";
        getline(cin, str);
        _weeks = toIntVec(splitString(str, ','));

        cout << "事项在周几？（有多天可用逗号隔开）：";
        getline(cin, str);
        _weekdays = toIntVec(splitString(str, ','));

        cout << "事项开始时间？（时 分）：";
        int hou, min;
        if (getLineVar(cin, hou, min))
            return NULL;
        _rankTime.startTime = hou * 60 + min;
        cout << "事项耗时？（时 分）：";
        if (getLineVar(cin, hou, min))
            return NULL;
        _rankTime.endTime = _rankTime.startTime + hou * 60 + min;
    }
    int year, mon, day;
    time_t t = 0;
    tm *tmpDate = localtime(&t);
    cout << "请输入事项截止日期（年 月 日）：";
    if (getLineVar(cin, year, mon, day))
        return NULL;
    tmpDate->tm_year = year - 1900;
    tmpDate->tm_mon = mon - 1;
    tmpDate->tm_mday = day;
    //核心
    _DDLDate = mktime(tmpDate) / (24 * 3600); //取得日期时间戳
    //核心

    cout << "请输入事件名称：";
    getline(cin, _name);
    cout << "请输入事件地点：";
    getline(cin, _site);
    cout << "请输入备注：";
    getline(cin, _remark);
    vector<timeDate> _rankDate(0);
    for (auto tmpDate : _weeks)
    {
        for (auto tmpWDays : _weekdays)
        {
            _rankDate.push_back(tmpDate * 7 + tmpWDays - 8 + g_startDate);
        }
    }
    return (new arrange(_name, _site, _DDLDate, _rankTime, _rankDate, _remark));
}

string arrange::store()
{
    string str = "";
    str = str + "2\n" +
          name + "\n" +
          site + "\n" +
          to_string(DDLDate) + " " + to_string(rankTime.startTime) + " " + to_string(rankTime.endTime) + "\n" +
          remark + "\n";
    for (auto a : rankDate)
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

void arrange::load(istream &fin)
{
    string str;
    getline(fin, name);
    getline(fin, site);
    getLineVar(fin, DDLDate, rankTime.startTime, rankTime.endTime);
    getline(fin, remark);
    getline(fin, str);
    rankDate = toDateVec(splitString(str));
    getline(fin, str);
    vector<timeDate> tmpDateVec = toDateVec(splitString(str));
    for (auto a : tmpDateVec)
    {
        isSubmit[a] = true;
    }
}

schedule *arrange::reset(schedule *sp, timeDate theData)
{
    arrange *tmpArrangeP = new arrange();
    if (sp != NULL && theData != 0)
    {
        *tmpArrangeP = *(arrange *)sp;
        vector<timeDate> tmpRankDate;
        tmpRankDate.push_back(theData);
        tmpArrangeP->rankDate = tmpRankDate;
    }
    else
    {
        delete tmpArrangeP;
        tmpArrangeP = this;
    }
    cout << "您想修改什么？" << endl
         << "1、安排时间 2、DDL时间 3、事项名  4、事项地点 5、备注" << endl;
    int option;
    if (getLineVar(cin, option))
        return NULL;

    switch (option)
    {
    case 1:
    {
        timeScale _rankTime;
        string str;
        vector<int> _weeks, _weekdays;
        cout << "事项在哪几周？（有多周可用逗号隔开）：";
        getline(cin, str);
        _weeks = toIntVec(splitString(str, ','));

        cout << "事项在周几？（有多天可用逗号隔开）：";
        getline(cin, str);

        _weekdays = toIntVec(splitString(str, ','));

        cout << "事项开始时间？（时 分）：";
        int hou, min;
        if (getLineVar(cin, hou, min))
            return NULL;

        _rankTime.startTime = hou * 60 + min;
        cout << "事项耗时？（时 分）：";
        if (getLineVar(cin, hou, min))
            return NULL;
        _rankTime.endTime = _rankTime.startTime + hou * 60 + min;

        tmpArrangeP->rankTime = _rankTime;

        vector<timeDate> _rankDate(0);
        for (auto tmpDate : _weeks)
        {
            for (auto tmpWDays : _weekdays)
            {
                _rankDate.push_back(tmpDate * 7 + tmpWDays - 8 + g_startDate);
            }
        }
        tmpArrangeP->rankDate = _rankDate;

        break;
    }
    case 2:
    {
        cout << "请输入DDL时间（年 月 日）：";
        int year, mon, day;
        if (getLineVar(cin, year, mon, day))
            return NULL;
        time_t t = 0;
        tm *tmpDate = localtime(&t);
        tmpDate->tm_year = year - 1900;
        tmpDate->tm_mon = mon - 1;
        tmpDate->tm_mday = day;
        tmpArrangeP->DDLDate = mktime(tmpDate) / (24 * 3600);
        break;
    }
    case 3:
        cout << "请输入新事项名：";
        getline(cin, tmpArrangeP->name);
        break;
    case 4:
        cout << "请输入新事项地点：";
        getline(cin, tmpArrangeP->site);
        break;
    case 5:
        cout << "请输入新备注：";
        getline(cin, tmpArrangeP->remark);
        break;
    default:
        break;
    }

    return tmpArrangeP;
}

vector<timeDate> arrange::getRankDate()
{
    return rankDate;
}

void arrange::eraseRankDate(timeDate theRankDate)
{
    auto it_date = find(rankDate.begin(), rankDate.end(), theRankDate);
    if (it_date != rankDate.end())
        rankDate.erase(it_date);
}