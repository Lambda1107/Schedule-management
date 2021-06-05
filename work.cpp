#include "schedule.h"
#include "work.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

work::work() {}
work::work(string _name, string _remark, timeDate _DDLDate, timeDateMonent _rankTime, timeMonent _timeLong)
{
    name = _name;
    remark = _remark;
    DDLDate = _DDLDate;
    rankTime = _rankTime;
    timeLong = _timeLong;
}
work::~work() {}

schedule *work::addWork()
{
    string _name, _remark, str;
    timeDate _DDLDate = 0;
    timeDateMonent _rankTime = 0;
    timeMonent _timeLong = 0;
    do
    {
        cout << "是否安排时间？（yes or no）:";
        if (getLineVar(cin, str))
            return NULL;
    } while (str != "yes" && str != "no");
    if (str == "yes")
    {
        int year, mon, day, hour, min;
        cout << "请输入安排的时间（年 月 日 小时 分钟）：";
        if (getLineVar(cin, year, mon, day, hour, min))
            return NULL;
        time_t t = 0;
        tm *tmpDate = localtime(&t);
        tmpDate->tm_year = year - 1900;
        tmpDate->tm_mon = mon - 1;
        tmpDate->tm_mday = day;
        _rankTime = (mktime(tmpDate) / (24 * 3600)) * 24 * 60 + hour * 60 + min;
        cout << "请输入需要多长时间（小时 分钟）：";
        if (getLineVar(cin, hour, min))
            return NULL;
        _timeLong = hour * 60 + min;
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
    cout << "请输入作业名称：";
    getline(cin, _name);
    cout << "请输入备注：";
    getline(cin, _remark);
    return (new work(_name, _remark, _DDLDate, _rankTime, _timeLong));
}

timeMonent work::getStartTime()
{
    return rankTime % (60 * 24);
}

string work::printOut(timeDate theDate)
{
    if (rankTime / (24 * 60) == 0)
    {
        return "before " + getDataText(DDLDate) + " " + name + " at " + " (" + remark + ")";
    }
    else
    {
        return getTimeMonentText(getStartTime()) + " to " + getTimeMonentText(getStartTime() + timeLong) + " " + name + " (" + remark + ")";
    }
}

timeDate work::getRecentDate(timeDate theDate)
{
    if (theDate > DDLDate)
        return -1;
    else if (theDate < rankTime / (60 * 24))
        return rankTime / (60 * 24);
    else
        return DDLDate;
}

schedule *work::reset(schedule *sp, timeDate theData)
{
    string str;
    cout << "您想修改什么？" << endl
         << "1、安排时间 2、DDL时间 3、作业名 4、备注" << endl;
    int option;
    if (getLineVar(cin, option))
        return NULL;
    switch (option)
    {
    case 1:
    {
        int year, mon, day, hour, min;
        cout << "请输入安排的日期：";
        if (getLineVar(cin, year, mon, day))
            return NULL;
        cout << "请输入安排的时间：";
        if (getLineVar(cin, hour, min))
            return NULL;
        time_t t = 0;
        tm *tmpDate = localtime(&t);
        tmpDate->tm_year = year - 1900;
        tmpDate->tm_mon = mon - 1;
        tmpDate->tm_mday = day;

        rankTime = (mktime(tmpDate) / (24 * 3600)) * 24 * 60 + hour * 60 + min;

        cout << "请输入需要多长时间（小时 分钟）：";
        if (getLineVar(cin, hour, min))
            return NULL;
        timeLong = hour * 60 + min;
        break;
    }
    case 2:
        cout << "请输入DDL时间：";
        if (getLineVar(cin, DDLDate))
            return NULL;
        break;
    case 3:
        cout << "请输入新作业名：";
        getline(cin, name);
        break;
    case 4:
        cout << "请输入新备注：";
        getline(cin, remark);
        break;
    default:
        break;
    }

    return this;
}

string work::store()
{
    string str = "";
    str = str + "3\n" +
          name + "\n" +
          to_string(DDLDate) + " " + to_string(rankTime) + " " + to_string(timeLong) + "\n" +
          remark;
    return str;
}

void work::load(istream &fin)
{
    getline(fin, name);
    getLineVar(fin, DDLDate, rankTime, timeLong);
    getline(fin, remark);
}

void work::eraseRankDate(timeDate theRankDate)
{
    if ((rankTime / (24 * 60)) == theRankDate)
        rankTime = 0;
}

vector<timeDate> work::getRankDate()
{
    vector<timeDate> tmpVec(0);
    if ((rankTime / (24 * 60)) != 0)
        tmpVec.push_back(rankTime / (24 * 60));
    return tmpVec;
}