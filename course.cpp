#include "course.h"
#include "course.h"
#include <iostream>
#include <vector>
#include <algorithm>
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
    return;  //��Ϊ�γ��ǵ���ÿ����ģ�ɶҲ���ö�
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

course::course(string _name, string _site, int _rank, int _rankNum, vector<int> &_courseWeeks, int _courseWeekDay)
{
    name = _name;
    site = _site;
    rank = _rank;
    rankNum = _rankNum;
    courseWeeks = _courseWeeks;
    courseWeekDay = _courseWeekDay;
}

course *course::addCourse()
{
    int weekday, scheduleRank, scheduleNum;
    string name, site, str;
    cout << "������γ����ܼ�(1-7)��";
    cin >> weekday;
    cout << "������γ��ǵڼ��ڿο�ʼ(1-" << g_timeTab.size() << ")��";
    cin >> scheduleRank;
    cout << "�������ܹ��������ڿΣ�(1-" << g_timeTab.size() - scheduleRank + 1 << ")��";
    cin >> scheduleNum;
    cout << "������γ�����";
    cin.ignore(1, '\n');
    getline(cin, name);
    cout << "������γ̵ص㣺";
    getline(cin, site);
    cout << "�������Ͽ�����(�Զ��ŷָ�)��";
    getline(cin, str);
    size_t pos; //�ָ��ַ���
    vector<int> result;
    str += ','; //��չ�ַ����Է������
    int size = str.size();
    for (int i = 0; i < size; i++)
    {
        pos = str.find(',', i);
        if (pos < size)
        {
            std::string s = str.substr(i, pos - i);
            result.push_back(atoi(s.c_str()));
            i = pos;
        }
    }
    result.erase(unique(result.begin(), result.end()), result.end());
    //result���������Ͽ�����
    return (new course(name, site, scheduleRank, scheduleNum, result, weekday));
}

vector<int> course::getWDayRank()
{
    vector<int> vec(1);
    vec[0] = courseWeekDay;
    return vec;
}

course::~course()
{
}