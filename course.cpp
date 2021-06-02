#include "course.h"
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
    return; //��Ϊ�γ��ǵ���ÿ����ģ�ɶҲ���ö�
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
    sort(result.begin(), result.end());
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

schedule *course::reset(schedule *sp, timeDate theData)
{
    course *tmpCourseP = new course();
    if (sp != NULL && theData != 0)
    {
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
    cin >> option;

    switch (option)
    {
    case 1:
    {
        int weekday, scheduleRank, scheduleNum;
        string str;
        vector<int> result;
        cout << "������γ����ܼ�(1-7)��";
        cin >> weekday;
        cout << "������γ��ǵڼ��ڿο�ʼ(1-" << g_timeTab.size() << ")��";
        cin >> scheduleRank;
        cout << "�������ܹ��������ڿΣ�(1-" << g_timeTab.size() - scheduleRank + 1 << ")��";
        cin >> scheduleNum;
        cout << "�������Ͽ�����(�Զ��ŷָ�)��";
        cin.ignore(1, '\n');
        getline(cin, str);
        size_t pos; //�ָ��ַ���
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
        // result���������Ͽ�����
        tmpCourseP->courseWeekDay = weekday;
        tmpCourseP->courseWeeks = result;
        tmpCourseP->rank = scheduleRank;
        tmpCourseP->rankNum = scheduleNum;
        break;
    }
    case 2:
        cout << "������γ�����";
        cin.ignore(1, '\n');
        getline(cin, tmpCourseP->name);
        break;
    case 3:
        cout << "������γ̵ص㣺";
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
    sort(result.begin(), result.end());
    result.erase(unique(result.begin(), result.end()), result.end());
    //result���������Ͽ�����
    courseWeeks = result;
}

course::~course()
{
}