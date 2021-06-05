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

    //��������İ���ʱ�䣬���û�оͷ���DDLʱ�䣬���DDLʱ���Ѿ����˾ͷ���-1
}

timeMonent arrange::getStartTime()
{
    return rankTime.startTime;
}

string arrange::printOut(timeDate theDate)
{
    if (rankDate.size() == 0)
    {
        return "before " + getDataText(DDLDate) + " " + name + " at " + site + " (" + remark + ")";
    }
    else
    {
        return getTimeMonentText(getStartTime()) + " to " + getTimeMonentText(rankTime.endTime) + " " + name + " at " + site + " (" + remark + ")";
    }
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
        cout << "�Ƿ���ʱ�䣿��yes or no��:";
        if (getLineVar(cin, str))
            return NULL;
    } while (str != "yes" && str != "no");

    if (str == "yes")
    {
        cout << "�������ļ��ܣ����ж��ܿ��ö��Ÿ�������";
        getline(cin, str);
        _weeks = toIntVec(splitString(str, ','));

        cout << "�������ܼ������ж�����ö��Ÿ�������";
        getline(cin, str);
        _weekdays = toIntVec(splitString(str, ','));

        cout << "���ʼʱ�䣿��ʱ �֣���";
        int hou, min;
        if (getLineVar(cin, hou, min))
            return NULL;
        _rankTime.startTime = hou * 60 + min;
        cout << "�����ʱ����ʱ �֣���";
        if (getLineVar(cin, hou, min))
            return NULL;
        _rankTime.endTime = _rankTime.startTime + hou * 60 + min;
    }
    int year, mon, day;
    time_t t = 0;
    tm *tmpDate = localtime(&t);
    cout << "�����������ֹ���ڣ��� �� �գ���";
    if (getLineVar(cin, year, mon, day))
        return NULL;
    tmpDate->tm_year = year - 1900;
    tmpDate->tm_mon = mon - 1;
    tmpDate->tm_mday = day;
    //����
    _DDLDate = mktime(tmpDate) / (24 * 3600); //ȡ������ʱ���
    //����

    cout << "�������¼����ƣ�";
    getline(cin, _name);
    cout << "�������¼��ص㣺";
    getline(cin, _site);
    cout << "�����뱸ע��";
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
    cout << "�����޸�ʲô��" << endl
         << "1������ʱ�� 2��DDLʱ�� 3��������  4������ص� 5����ע" << endl;
    int option;
    if (getLineVar(cin, option))
        return NULL;

    switch (option)
    {
    case 1:
    {
        timeScale _rankTime;
        string str;

        cout << "���ʼʱ�䣿��ʱ �֣���";
        int hou, min;
        if (getLineVar(cin, hou, min))
            return NULL;
        _rankTime.startTime = hou * 60 + min;
        cout << "�����ʱ����ʱ �֣���";
        if (getLineVar(cin, hou, min))
            return NULL;
        _rankTime.endTime = _rankTime.startTime + hou * 60 + min;

        tmpArrangeP->rankTime = _rankTime;

        break;
    }
    case 2:
        cout << "������DDLʱ�䣺";
        if (getLineVar(cin, tmpArrangeP->DDLDate))
            return NULL;
        break;
    case 3:
        cout << "����������������";
        getline(cin, tmpArrangeP->name);
        break;
    case 4:
        cout << "������������ص㣺";
        getline(cin, tmpArrangeP->site);
        break;
    case 5:
        cout << "�������±�ע��";
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