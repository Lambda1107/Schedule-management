#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "course.h"
#include "main.h"
using namespace std;
/*��������
����������6������׽�ȥ���տ�  ��arrange�ࣩ
2 4 6 8�ܵ����������п� ��һ��������ϵ�ܹ� 7��00��9��00 ��course�ࣩ
������12��ǰҪ��c++����ҵ ��work�ࣩ
ÿ�ܶ�8��ǰҪ����������ҵ ��work�ࣩ
��ϰ����11�� (arrange��)
�����Ѿ���ϰ��7.3 �ܹ���Ҫ��ϰ����10�� ��progress�ࣩ
ÿ����Ҫд10�������⣨work�ࣩ
�����Ѿ�����29�Σ��ܹ���Ҫ��45�Σ�6.25�Ž�ֹ��progress�ࣩ
*/

//ȫ�ֱ���
vector<plan> globalPlan(0);     //ÿ��ƻ��ļ���
int g_courseLong = 0;           //ÿ�ڿ�ʱ�� ��λ����
timeDate g_startDate = 0;       //�ƻ���ʼ����
vector<timeScale> g_timeTab(0); //ÿ��ʱ���

string getTimeMonentText(timeMonent tim)
{

    string str = "";
    str += to_string(tim / 60);
    str += ":";
    str += to_string(tim % 60);
    return str;
}

timeDate getNowDate() //ȡ�õ�ǰ����
{
    time_t t;
    t = time(0);
    return t / (24 * 3600);
}

int getWeek(timeDate td) //ȡ��ָ����������ڿ�ʼ���ڵ�����
{
    td -= g_startDate;
    return (td / 7) + 1;
}

int getWday(timeDate td) //�õ����ڼ�
{
    time_t t = td * 24 * 3600;
    tm *tt = localtime(&t);
    return tt->tm_wday;
}

string getWdayText(int wday) //����ת����
{
    switch (wday)
    {
    case 0:
        return "Sunday";
        break;
    case 1:
        return "Monday";
        break;
    case 2:
        return "Tuesday";
        break;
    case 3:
        return "Wednesday";
        break;
    case 4:
        return "Thursday";
        break;
    case 5:
        return "Friday";
        break;
    case 6:
        return "Saturday";
        break;
    default:
        return "Saturday";
        break;
    }
}

vector<timeScale> getTimeMapFromText(string str)
{
    //�ָ��ַ���
    size_t pos;
    vector<string> result;
    str += ','; //��չ�ַ����Է������
    int size = str.size();
    for (int i = 0; i < size; i++)
    {
        pos = str.find(',', i);
        if (pos < size)
        {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos;
        }
    }
    //�õ����ַ���vector
    vector<timeScale> pts(result.size());
    int hour, min;

    for (int i = 0; i < result.size(); i++)
    {
        istringstream istr(result[i]);
        istr >> hour >> min;
        pts[i].startTime = hour * 60 + min;
        pts[i].endTime = pts[i].startTime + g_courseLong;
    }
    return pts;
}

bool sortCoursesFunction(course *a, course *b)
{
    return a->getStartTime() < b->getStartTime();
}

int loadInformation()
{
    string tmpInput;
    ifstream fin;
    fin.open("data.txt");
    fin >> g_courseLong >> g_startDate;
    fin.get();
    getline(fin, tmpInput);
    g_timeTab = getTimeMapFromText(tmpInput);
    fin.close();
    return 1;
}
void settingInformation()
{
    timeDate startDate;
    string tmpInput;
    int courseLong;
    int year, mon, day;
    time_t t = time(0);
    tm *tmpDate = localtime(&t);
    cout << "����û�����û�����Ϣ��" << endl
         << "����������ʼ�Ͽεĵ�һ������һ��������(�Կո�ָ�)��";
    cin >> year >> mon >> day;
    tmpDate->tm_year = year - 1900;
    tmpDate->tm_mon = mon - 1;
    tmpDate->tm_mday = day;
    startDate = mktime(tmpDate) / (24 * 3600); //ȡ������ʱ���
    g_startDate = startDate;
    cout << "��������ÿ�ڿε�ʱ��(����)��";
    cin >> courseLong;
    g_courseLong = courseLong;
    cout << "��������ÿ�ڿε��Ͽ�ʱ��(��֮���ö��Ÿ���)��";
    cin.get();
    getline(cin, tmpInput);
    g_timeTab = getTimeMapFromText(tmpInput);
    cout << "*************������ɣ�*****************" << endl;
}

void storeInformation()
{
}
string getDataText(timeDate a)
{
    time_t t = a * 3600 * 24;
    tm *tt = localtime(&t);
    char c[20];
    strftime(c, 20, "%d %b", tt);
    return string(c);
}
void listCourse(int week) //��ʾ�γ�
{
    system("cls");
    cout << week << " " << getDataText(g_startDate + week * 7 - 7) << " to " << getDataText(g_startDate + week * 7 - 1) << endl;
    cout << "=================" << endl;
    bool b = 0;
    for (auto &a : globalPlan)
    {
        if (getWeek(a.Date) == week)
        {
            b = 1;
            cout << getDataText(a.Date) << " " << getWdayText(getWday(a.Date)) << ':' << endl;
            int i = 1;
            for (auto &tmpCourse : a.courseList)
            {
                cout << i << "��";
                tmpCourse->printOut();
                i++;
            }
        }
        else if (b)
        {
            break;
        }
    }
}

vector<plan>::iterator findDate(timeDate theDate)
{
    bool b = 0;
    auto j = globalPlan.begin();
    while (j != globalPlan.end())
    {
        if (j->Date == theDate)
        {
            b = 1;
            break;
        }
        else if (j->Date > theDate)
        {
            break;
        }
        ++j;
    }
    return j;
}

void addPlan() //��ӿγ�
{
    int weekday, courseRank, courseNum;
    string name, site, str;
    cout << "������γ����ܼ�(1-7)��";
    cin >> weekday;
    cout << "������γ��ǵڼ��ڿο�ʼ(1-" << g_timeTab.size() << ")��";
    cin >> courseRank;
    cout << "�������ܹ��������ڿΣ�(1-" << g_timeTab.size() - courseRank + 1 << ")��";
    cin >> courseNum;
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
    //result���������Ͽ�����
    course *tmpCourse = new course(name, site, courseRank, courseNum, result);
    for (auto &a : result)
    {
        timeDate theDate = g_startDate + a * 7 + weekday - 8;
        auto j = findDate(theDate);
        if (j != globalPlan.end() && j->Date == theDate) //�ж��Ƿ��Ѿ�������ļƻ���
        {
            j->courseList.push_back(tmpCourse);
            sort(j->courseList.begin(), j->courseList.end(), sortCoursesFunction);
        }
        else
        {
            vector<course *> tmpCourseList;
            tmpCourseList.push_back(tmpCourse);
            globalPlan.insert(j, 1, {theDate, tmpCourseList});
        }
    }
}
void removePlan(int week)
{
    int weekday, planRank;
    cout << "����ɾ���ܼ��ļƻ���1-7����";
    cin >> weekday;

    timeDate theDate = g_startDate + week * 7 + weekday - 8;
    auto j = findDate(theDate);
    if (j != globalPlan.end())
        cout << "����ɾ���ڼ����ƻ���1-" << j->courseList.size() << "����";
    else
    {
        cout << "��Ǹ����ѡ������û�мƻ���������Ӱɣ�";
        return;
    }
    cin >> planRank;
    auto tmpDPcourse = *(j->courseList.begin() + planRank - 1); //Ҫɾ����course��ָ��
    vector<int> tmpWeekRank = tmpDPcourse->isMut();             //Ҫɾ����course�������б�
    if (tmpWeekRank.size() > 1)                                 //����һ������ظ��ƻ�
    {
        string str;
        do //�жϺϷ�����
        {
            cout << "����һ������ظ��ƻ����Ƿ�ɾ��������Щ�ƻ�����yes or no��:";
            cin >> str;
        } while (str != "yes" && str != "no");
        if (str == "yes")
        {
            for (auto v : tmpWeekRank) //����ÿһ��������ε�����
            {
                theDate = g_startDate + v * 7 + weekday - 8;
                auto tmpDate = findDate(theDate);
                vector<course *>::iterator it = tmpDate->courseList.begin();
                while (it != tmpDate->courseList.end()) //��������Ŀ��ҵ�Ҫɾ���Ŀγ̲�ɾ��
                {
                    if (*it == tmpDPcourse)
                    {
                        tmpDate->courseList.erase(it); //�ڵ���γ��б���ɾ������γ̵�ָ��
                        break;
                    }
                    it++;
                }
                if (tmpDate->courseList.size() == 0)
                    globalPlan.erase(tmpDate); //���ɾ���˾Ͱ�����ɾ��
            }
            delete tmpDPcourse; //�ͷ�����γ̵��ڴ�
        }
        else //��Ҫȫɾ��
        {
            //�ĵ�����γ̵��ſ���
            tmpDPcourse->eraseWRank(week);
            j->courseList.erase(j->courseList.begin() + planRank - 1); //��ѡ��������ɾ������γ̵ĵ�ַ
            if (j->courseList.size() == 0)                             //���ɾ���˾Ͱ�����ɾ��
            {
                globalPlan.erase(j);
            }
        }
    }
    else //�ⲻ�Ǹ�����ظ�������
    {
        delete tmpDPcourse;
        j->courseList.erase(j->courseList.begin() + planRank - 1);
        if (j->courseList.size() == 0)
        {
            globalPlan.erase(j);
        }
    }

    cout << "***************ɾ���ɹ�*********************" << endl;
}
int main()
{
    int operation;
    if (loadInformation() == -1)
        settingInformation();
    int week = getWeek(getNowDate());
    do
    {
        listCourse(week);
        cout << "******��ѡ�����*******" << endl
             << "1�����ӿγ�   2��ɾ���γ�   3����ʾ���ܿγ�   4����ʾ���ܿγ�   5���޸Ŀγ�   6���˳�" << endl;
        cin >> operation;
        switch (operation)
        {
        case 1:
            addPlan();
            break;
        case 2:
            removePlan(week);
            break;
        case 3:
            ++week;
            break;
        case 4:
            --week;
            break;
        case 5:
            //globalPlan.reset();
            break;
        default:
            break;
        }
    } while (operation != 6);
    storeInformation();
}
