#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include "schedule.h"
#include "course.h"
#include "arrange.h"
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
vector<plan> globalPlan(0);                 //ÿ��ƻ��ļ���
vector<schedule *> globalAllSchedule(0);    //���мƻ��ļ���
vector<schedule *> globalNoPlanSchedule(0); //����û����ʱ��ƻ��ļ���
int g_scheduleLong = 0;                     //ÿ�ڿ�ʱ�� ��λ����
timeDate g_startDate = 0;                   //�ƻ���ʼ����
vector<timeScale> g_timeTab(0);             //ÿ��ʱ���
bool strToVar(string str) { return false; }
template <typename T, typename... Ts>
bool strToVar(string str, T &var, Ts &...Vars)
{
    while (*(str.begin()) == ' ')
        str.erase(0, 1);
    if (*(str.begin()) == 'q' || *(str.begin()) == 'Q')
        return true;
    str = "0 " + str;
    istringstream ss(str);
    ss >> var;
    ss >> var;
    size_t i = str.find(' ');
    if (i == str.npos)
        i = str.length();
    str.erase(0, i);
    strToVar(str, Vars...);
    return false;
}

template <typename... Ts>
bool getLineVar(istream &is, Ts &...Vars)
{
    string str;
    getline(is, str);
    return strToVar(str, Vars...);
}

string getTimeMonentText(timeMonent tim, char syb)
{

    string str = "";
    str += to_string(tim / 60);
    str += syb;
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

vector<string> splitString(string str, char syb)
{
    //�ָ��ַ���
    size_t pos;
    vector<string> result;
    if (str[str.length() - 1] != syb)
        str += syb; //��չ�ַ����Է������
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
    return result;
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
    vector<string> result = splitString(str, ',');
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
bool sortSchedulesByDateFunction(schedule *a, schedule *b)
{
    return a->getRecentDate(getNowDate()) < b->getRecentDate(getNowDate());
}

void settingInformation()
{
    timeDate startDate;
    string tmpInput;
    int scheduleLong;
    int year, mon, day;
    time_t t = {};
    tm *tmpDate = localtime(&t);
    cout << "����������ʼ�Ͽεĵ�һ������һ�����ڣ��� �� �գ���";
    if (getLineVar(cin, year, mon, day))
        return;
    tmpDate->tm_year = year - 1900;
    tmpDate->tm_mon = mon - 1;
    tmpDate->tm_mday = day;
    startDate = mktime(tmpDate) / (24 * 3600); //ȡ������ʱ���
    g_startDate = startDate;
    cout << "��������ÿ�ڿε�ʱ��(����)��";
    if (getLineVar(cin, scheduleLong))
        return;

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
    strftime(c, 20, "%b %d", tt);
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
    fin.ignore(1, '\n');
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
        case 2:
            tmpScheduleP = new arrange();
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
        fou << getTimeMonentText(tmpSchedule.startTime, ' ') << ',';
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

    for (int i = 0; i < 50; i++)
        cout << "��";
    cout << endl;
    cout << setw(42) << left << "��"
         << setw(2) << week << " " << setw(6) << getDataText(g_startDate + week * 7 - 7) << " to " << setw(43) << left << getDataText(g_startDate + week * 7 - 1)
         << "��" << endl;
    cout << "��";
    for (int i = 0; i < 96; i++)
        cout << "=";
    cout << "��" << endl;

    bool b = 0;
    for (auto &a : globalPlan)
    {
        if (getWeek(a.Date) == week)
        {
            b = 1;
            cout << setw(5) << "��" << setw(5) << getDataText(a.Date) << " " << setw(86) << getWdayText(getWday(a.Date)) + ':' << "��" << endl;
            int i = 1;
            for (auto &tmpSchedule : a.scheduleList)
            {
                cout << setw(5) << "��" << setw(93) << to_string(i) + "��" + tmpSchedule->printOut() << "��" << endl;
                i++;
            }
            cout << setw(98) << "��"
                 << "��" << endl;
        }
        else if (b)
        {
            break;
        }
    }
    if (!b)
    {
        cout << setw(98) << "��"
             << "��" << endl;
        cout << setw(42) << left << "��"
             << "���ܻ�û�мƻ���"
             << setw(42) << right << "��"
             << endl;
        cout << setw(98) << left << "��"
             << "��" << endl;
    }

    cout << "��";
    for (int i = 0; i < 96; i++)
        cout << "=";
    cout << "��" << endl;

    cout << setw(98) << "��"
         << "��" << endl;

    if (globalNoPlanSchedule.size() != 0)
    {
        int i = 1;
        for (auto a : globalNoPlanSchedule)
        {
            cout << setw(5) << "��" << setw(93) << to_string(i) + "��" + a->printOut() << "��" << endl;
            i++;
        }
    }
    else
    {
        cout << setw(40) << left << "��"
             << "��û���ް��ŵļƻ���"
             << setw(40) << right << "��"
             << endl;
    }
    cout << setw(98) << left << "��"
         << "��" << endl;
    for (int i = 0; i < 50; i++)
        cout << "��";
    cout << endl;
}

vector<int> toIntVec(vector<string> str)
{
    vector<int> result(0);
    for (int i = 0; i < str.size(); i++)
    {
        size_t pos = str[i].find('-');
        if (pos == str[i].npos)
        {
            result.push_back(atoi(str[i].c_str()));
        }
        else
        {
            int a = atoi(str[i].substr(0, pos).c_str());
            int b = atoi(str[i].substr(pos + 1).c_str());
            while (a <= b)
            {
                result.push_back(a);
                a++;
            }
        }
    }
    sort(result.begin(), result.end());
    result.erase(unique(result.begin(), result.end()), result.end());
    result.erase(result.begin(), lower_bound(result.begin(), result.end(), 1));
    return result;
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
    bool b = 1;
    for (auto &week : tmpSchedule->getWeekRank())
    {
        for (auto &weekday : tmpSchedule->getWDayRank())
        {
            b = 0;
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
        sort(globalAllSchedule.begin(), globalAllSchedule.end(), sortSchedulesByDateFunction);
    }

    if (b && find(globalNoPlanSchedule.begin(), globalNoPlanSchedule.end(), tmpSchedule) == globalNoPlanSchedule.end())
    {
        globalNoPlanSchedule.push_back(tmpSchedule);
        sort(globalNoPlanSchedule.begin(), globalNoPlanSchedule.end(), sortSchedulesByDateFunction);
    }
}

void removeNoPlanSchedule(schedule *tmpSchedule)
{
    auto it = find(globalNoPlanSchedule.begin(), globalNoPlanSchedule.end(), tmpSchedule);
    if (it != globalNoPlanSchedule.end())
        globalNoPlanSchedule.erase(it);
}

void deleteSchedule(schedule *tmpSchedule)
{
    auto it = find(globalAllSchedule.begin(), globalAllSchedule.end(), tmpSchedule);
    if (it != globalAllSchedule.end())
        globalAllSchedule.erase(it);
    removeNoPlanSchedule(tmpSchedule);
}

void addPlan() //��Ӽƻ�
{
    int choice;
    cout << "����������ּƻ���" << endl
         << "1���γ�  2������" << endl;
    if (getLineVar(cin, choice))
        return;
    schedule *tmpSchedule = NULL;
    switch (choice)
    {
    case 1:
        tmpSchedule = course::addCourse();
        break;
    case 2:
        tmpSchedule = arrange::addArrange();
        break;
    default:
        break;
    }
    if (tmpSchedule != NULL)
        insertPlan(tmpSchedule);
}

void removePlan(int week)
{
    //�ж�ɾ������
    cout << "����ɾ���Ѱ��ŵļƻ�δ����ʱ��ļƻ�����0���Ѱ��� 1��δ���ţ���";
    int option;
    if (getLineVar(cin, option))
        return;
    if (option)
    {
        if (globalNoPlanSchedule.size() == 0)
        {
            cout << "��Ǹ��û��δ���ŵļƻ���������Ӱɣ�" << endl;
            system("pause");
            return;
        }
        int choice;
        cout << "����ɾ���ڼ����ƻ�����1-" << globalNoPlanSchedule.size() << "����";
        if (getLineVar(cin, choice))
            return;
        while (choice > globalNoPlanSchedule.size() || choice < 1)
        {
            cout << "��Ǹ���������������������������";
            if (getLineVar(cin, choice))
                return;
        }
        auto it_theSchedule = globalNoPlanSchedule.begin() + choice - 1;
        deleteSchedule(*it_theSchedule);
    }
    else
    {

        int weekday, planRank;
        cout << "����ɾ���ܼ��ļƻ���1-7����";
        if (getLineVar(cin, weekday))
            return;

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

        if (getLineVar(cin, planRank))
            return;
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
                if (getLineVar(cin, str))
                    return;
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
                deleteSchedule(tmpDPschedule);
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
    }
    cout << "***************ɾ���ɹ�*********************" << endl;
}

void resetPlan(int week)
{
    //�ж��޸�����
    cout << "��������Ѱ��ŵļƻ�δ����ʱ��ļƻ�����0���Ѱ��� 1��δ���ţ���";
    int option;
    if (getLineVar(cin, option))
        return;
    if (option)
    {
        if (globalNoPlanSchedule.size() == 0)
        {
            cout << "��Ǹ��û��δ���ŵļƻ���������Ӱɣ�" << endl;
            system("pause");
            return;
        }
        int choice;
        cout << "�����޸ĵڼ����ƻ�����1-" << globalNoPlanSchedule.size() << "����";
        if (getLineVar(cin, choice))
            return;
        while (choice > globalNoPlanSchedule.size() || choice < 1)
        {
            cout << "��Ǹ���������������������������";
            if (getLineVar(cin, choice))
                return;
        }
        auto it_theSchedule = globalNoPlanSchedule.begin() + choice - 1;
        deleteSchedule(*it_theSchedule);
        insertPlan((*it_theSchedule)->reset());
    }
    else
    {

        int weekday, planRank;
        cout << "�����޸��ܼ��ļƻ���1-7����";
        if (getLineVar(cin, weekday))
            return;

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

        if (getLineVar(cin, planRank))
            return;
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
                if (getLineVar(cin, str))
                    return;
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
            deleteSchedule(*(it_theDate->scheduleList.begin() + planRank - 1));
            //���ɾ���˾Ͱ�����ɾ��
            if (it_theDate->scheduleList.size() == 0)
                globalPlan.erase(it_theDate);
            insertPlan(tmpEPschedule->reset()); //�����������޸�
        }
    }
    cout << "*****************�޸ĳɹ�*********************" << endl;
}

int main()
{
    int operation;
    if (loadInformation() == -1)
    {
        cout << "����û�����û�����Ϣ��" << endl;
        settingInformation();
    }
    int week = getWeek(getNowDate());
    do
    {
        listSchedule(week);
        cout << "***************************************��ѡ�����***************************************************" << endl
             << "\t\t1�����Ӽƻ�\t2��ɾ���ƻ�\t3����ʾ���ܼƻ�\n\t\t4����ʾ���ܼƻ�\t5���޸ļƻ�\t6��������ת\n\t\t7��������Ϣ\t8������\t\t9���˳�" << endl;
        if (getLineVar(cin, operation))
            operation = 9;
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
        case 6:
            cout << "��ת�����ܣ���";
            getLineVar(cin, week);
            break;
        case 7:
            settingInformation();
            break;
        case 8:
            storeInformation();
            break;
        default:
            break;
        }
    } while (operation != 9);
    storeInformation();
}
