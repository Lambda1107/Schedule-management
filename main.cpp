#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include "schedule.h"
#include "course.h"
using namespace std;
/*��������
����������6������׽�ȥ���տ�  ��arrange�ࣩ
2 4 6 8�ܵ����������п� ��һ��������ϵ�ܹ� 7��00��9��00 ��schedule�ࣩ
������12��ǰҪ��c++����ҵ ��work�ࣩ
ÿ�ܶ�8��ǰҪ����������ҵ ��work�ࣩ
��ϰ����11�� (arrange��)
�����Ѿ���ϰ��7.3 �ܹ���Ҫ��ϰ����10�� ��progress�ࣩ
ÿ����Ҫд10�������⣨work�ࣩ
�����Ѿ�����29�Σ��ܹ���Ҫ��45�Σ�6.25�Ž�ֹ��progress�ࣩ
*/

//ȫ�ֱ���
vector<plan> globalPlan(0);              //ÿ��ƻ��ļ���
vector<schedule *> globalAllSchedule(0); //���мƻ��ļ���
int g_scheduleLong = 0;                  //ÿ�ڿ�ʱ�� ��λ����
timeDate g_startDate = 0;                //�ƻ���ʼ����
vector<timeScale> g_timeTab(0);          //ÿ��ʱ���

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
    if (str[str.length() - 1] != ',')
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
        pts[i].endTime = pts[i].startTime + g_scheduleLong;
    }
    return pts;
}

bool sortSchedulesFunction(schedule *a, schedule *b)
{
    return a->getStartTime() < b->getStartTime();
}

void settingInformation()
{
    timeDate startDate;
    string tmpInput;
    int scheduleLong;
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
    cin >> scheduleLong;
    g_scheduleLong = scheduleLong;
    cout << "��������ÿ�ڿε��Ͽ�ʱ��(��֮���ö��Ÿ���)��";
    cin.get();
    getline(cin, tmpInput);
    g_timeTab = getTimeMapFromText(tmpInput);
    cout << "*************������ɣ�*****************" << endl;
}

string getDataText(timeDate a)
{
    time_t t = a * 3600 * 24;
    tm *tt = localtime(&t);
    char c[20];
    strftime(c, 20, "%d %b", tt);
    return string(c);
}
int loadInformation()
{
    string tmpInput;
    ifstream fin;
    fin.open("data.txt");
    if (!fin)
        return -1;
    fin >> g_scheduleLong >> g_startDate;
    fin.get();
    getline(fin, tmpInput);
    g_timeTab = getTimeMapFromText(tmpInput);
    int category;
    while (fin >> category)
    {
        schedule *tmpScheduleP;
        switch (category)
        {
        case 1:

            tmpScheduleP = new course();
            tmpScheduleP->load(fin);

            break;

        default:
            break;
        }
        insertPlan(tmpScheduleP);
    }
    fin.close();
    return 1;
}

void storeInformation()
{
    ofstream fou;
    fou.open("data.txt");
    fou << g_scheduleLong << ' ' << g_startDate << '\n';
    for (auto tmpSchedule : g_timeTab)
    {
        fou << getTimeMonentText(tmpSchedule.startTime) << ',';
    }
    fou << '\n';
    for (auto tmpSchedule : globalAllSchedule)
    {
        fou << tmpSchedule->store() << '\n';
    }
    fou.close();
}

void listSchedule(int week) //��ʾ�γ�
{
    system("cls");
    cout << week << " " << getDataText(g_startDate + week * 7 - 7) << " to " << getDataText(g_startDate + week * 7 - 1) << endl;
    cout << "=================================" << endl;
    bool b = 0;
    for (auto &a : globalPlan)
    {
        if (getWeek(a.Date) == week)
        {
            b = 1;
            cout << getDataText(a.Date) << " " << getWdayText(getWday(a.Date)) << ':' << endl;
            int i = 1;
            for (auto &tmpSchedule : a.scheduleList)
            {
                cout << i << "��";
                tmpSchedule->printOut();
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

void insertPlan(schedule *tmpSchedule)
{
    for (auto &week : tmpSchedule->getWeekRank())
    {
        for (auto &weekday : tmpSchedule->getWDayRank())
        {
            timeDate theDate = g_startDate + week * 7 + weekday - 8;
            auto j = findDate(theDate);
            if (j != globalPlan.end() && j->Date == theDate) //�ж��Ƿ��Ѿ�������ļƻ���
            {
                j->scheduleList.push_back(tmpSchedule);
                sort(j->scheduleList.begin(), j->scheduleList.end(), sortSchedulesFunction);
            }
            else
            {
                vector<schedule *> tmpScheduleList;
                tmpScheduleList.push_back(tmpSchedule);
                globalPlan.insert(j, 1, {theDate, tmpScheduleList});
            }
        }
    }
    if (find(globalAllSchedule.begin(), globalAllSchedule.end(), tmpSchedule) == globalAllSchedule.end())
    {
        globalAllSchedule.push_back(tmpSchedule);
    }
}
void deleteSchedule(schedule *tmpSchedule)
{
    globalAllSchedule.erase(find(globalAllSchedule.begin(), globalAllSchedule.end(), tmpSchedule));
}

void addPlan() //��Ӽƻ�
{
    int choice;
    cout << "����������ּƻ���" << endl
         << "1���γ�" << endl;
    cin >> choice;
    schedule *tmpSchedule;
    switch (choice)
    {
    case 1:
        tmpSchedule = course::addCourse();
        break;
    default:
        break;
    }
    insertPlan(tmpSchedule);
}
void removePlan(int week)
{
    int weekday, planRank;
    cout << "����ɾ���ܼ��ļƻ���1-7����";
    cin >> weekday;

    timeDate theDate = g_startDate + week * 7 + weekday - 8;
    auto it_theDate = findDate(theDate);

    if (it_theDate->Date == theDate)
        cout << "����ɾ���ڼ����ƻ���1-" << it_theDate->scheduleList.size() << "����";
    else
    {
        cout << "��Ǹ����ѡ������û�мƻ���������Ӱɣ�" << endl;
        system("pause");
        return;
    }

    cin >> planRank;
    auto tmpDPschedule = *(it_theDate->scheduleList.begin() + planRank - 1); //Ҫɾ����schedule��ָ��
    vector<int> tmpWeekRank = tmpDPschedule->getWeekRank();                  //Ҫɾ����schedule�������б�
    vector<int> tmpWDayRank = tmpDPschedule->getWDayRank();                  //Ҫɾ����schedule��һ���������б�
    if (tmpWeekRank.size() > 1 || tmpWDayRank.size() > 1)                    //����һ������ظ��ƻ�
    {
        //�жϺϷ�����
        string str;
        do
        {
            cout << "����һ������ظ��ƻ����Ƿ�ɾ��������Щ�ƻ�����yes or no��:";
            cin >> str;
        } while (str != "yes" && str != "no");
        //�ж�����yes or no
        if (str == "yes")
        {
            for (auto tmpWeek : tmpWeekRank) //����ÿһ�������schedule������
            {
                for (auto tmpWeekDay : tmpWDayRank) //����ÿһ�������schedule�����е�����
                {
                    //��������ָ��
                    theDate = g_startDate + tmpWeek * 7 + tmpWeekDay - 8;
                    auto tmpDate = findDate(theDate);
                    //���������schedule�ҵ�Ҫɾ����schedule��ɾ��
                    vector<schedule *>::iterator it = tmpDate->scheduleList.begin();
                    while (it != tmpDate->scheduleList.end())
                    {
                        if (*it == tmpDPschedule)
                        {
                            tmpDate->scheduleList.erase(it); //�ڵ���schedule�б���ɾ�����schedule��ָ��
                            deleteSchedule(*it);
                            break;
                        }
                        it++;
                    }
                    //���ɾ���˾Ͱ�����ɾ��
                    if (tmpDate->scheduleList.size() == 0)
                        globalPlan.erase(tmpDate);
                }
            }
            delete tmpDPschedule; //�ͷ����schedule���ڴ�
        }
        else //��Ҫȫɾ��
        {
            //�ĵ����schedule��weekRank
            tmpDPschedule->eraseWeekRank(week);
            //�ĵ����schedule��wDayRank
            tmpDPschedule->eraseWDayRank(week);
            it_theDate->scheduleList.erase(it_theDate->scheduleList.begin() + planRank - 1); //��ѡ��������ɾ������γ̵ĵ�ַ
            if (it_theDate->scheduleList.size() == 0)                                        //���ɾ���˾Ͱ�����ɾ��
            {
                globalPlan.erase(it_theDate);
            }
        }
    }
    else //�ⲻ�Ǹ�����ظ�������
    {
        delete tmpDPschedule; //ֱ���ͷŵ��ڴ�
        deleteSchedule(tmpDPschedule);
        it_theDate->scheduleList.erase(it_theDate->scheduleList.begin() + planRank - 1);
        //���ɾ���˾Ͱ�����ɾ��
        if (it_theDate->scheduleList.size() == 0)
        {
            globalPlan.erase(it_theDate);
        }
    }

    cout << "***************ɾ���ɹ�*********************" << endl;
}

void resetPlan(int week)
{
    int weekday, planRank;
    cout << "�����޸��ܼ��ļƻ���1-7����";
    cin >> weekday;

    timeDate theDate = g_startDate + week * 7 + weekday - 8;
    auto it_theDate = findDate(theDate);

    if (it_theDate->Date == theDate)
        cout << "�����޸ĵڼ����ƻ���1-" << it_theDate->scheduleList.size() << "����";
    else
    {
        cout << "��Ǹ����ѡ������û�мƻ���������Ӱɣ�" << endl;
        system("pause");
        return;
    }

    cin >> planRank;
    auto tmpEPschedule = *(it_theDate->scheduleList.begin() + planRank - 1); //Ҫ�޸ĵ�schedule��ָ��
    vector<int> tmpWeekRank = tmpEPschedule->getWeekRank();                  //Ҫ�޸ĵ�schedule�������б�
    vector<int> tmpWDayRank = tmpEPschedule->getWDayRank();                  //Ҫ�޸ĵ�schedule��һ���������б�
    if (tmpWeekRank.size() > 1 || tmpWDayRank.size() > 1)                    //����һ������ظ��ƻ�
    {
        //�жϺϷ�����
        string str;
        do
        {
            cout << "����һ������ظ��ƻ����Ƿ��޸�������Щ�ƻ�����yes or no��:";
            cin >> str;
        } while (str != "yes" && str != "no");
        //�ж�����yes or no
        if (str == "yes")
        {
            for (auto tmpWeek : tmpWeekRank) //����ÿһ�������schedule������
            {
                for (auto tmpWeekDay : tmpWDayRank) //����ÿһ�������schedule�����е�����
                {
                    //��������ָ��
                    theDate = g_startDate + tmpWeek * 7 + tmpWeekDay - 8;
                    auto tmpDate = findDate(theDate);
                    //���������schedule�ҵ�Ҫɾ����schedule��ɾ��
                    vector<schedule *>::iterator it = tmpDate->scheduleList.begin();
                    while (it != tmpDate->scheduleList.end())
                    {
                        if (*it == tmpEPschedule)
                        {
                            tmpDate->scheduleList.erase(it); //�ڵ���schedule�б���ɾ�����schedule��ָ��
                            deleteSchedule(*it);
                            break;
                        }
                        it++;
                    }
                    //���ɾ���˾Ͱ�����ɾ��
                    if (tmpDate->scheduleList.size() == 0)
                        globalPlan.erase(tmpDate);
                }
            }

            insertPlan(tmpEPschedule->reset()); //�����������޸�
        }
        else //ֻ��һ��
        {
            //�ĵ����schedule��weekRank
            tmpEPschedule->eraseWeekRank(week);
            //�ĵ����schedule��wDayRank
            tmpEPschedule->eraseWDayRank(week);
            it_theDate->scheduleList.erase(it_theDate->scheduleList.begin() + planRank - 1); //��ѡ��������ɾ�����schedule�ĵ�ַ
            //���ɾ���˾Ͱ�����ɾ��
            if (it_theDate->scheduleList.size() == 0)
                globalPlan.erase(it_theDate);
            insertPlan(tmpEPschedule->reset(tmpEPschedule, theDate)); //����֮����ȥ
        }
    }
    else //�ⲻ�Ǹ�����ظ�������
    {
        it_theDate->scheduleList.erase(it_theDate->scheduleList.begin() + planRank - 1);
        //���ɾ���˾Ͱ�����ɾ��
        if (it_theDate->scheduleList.size() == 0)
            globalPlan.erase(it_theDate);
        insertPlan(tmpEPschedule->reset()); //�����������޸�
    }

    cout << "***************�޸ĳɹ�*********************" << endl;
}

int main()
{
    int operation;
    if (loadInformation() == -1)
        settingInformation();
    int week = getWeek(getNowDate());
    do
    {
        listSchedule(week);
        cout << "******��ѡ�����*******" << endl
             << "1�����Ӽƻ�   2��ɾ���ƻ�   3����ʾ���ܼƻ�   4����ʾ���ܼƻ�   5���޸ļƻ�   6���˳�" << endl;
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
            --week;
            break;
        case 4:
            ++week;
            break;
        case 5:
            resetPlan(week);
            break;
        default:
            break;
        }
    } while (operation != 6);
    storeInformation();
}
