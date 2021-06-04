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

/*测试数据
这周五晚上6点跟曾兆杰去吃烧烤  （arrange类）
2 4 6 8周的周四晚上有课 新一代网络体系架构 7：00到9：00 （schedule类）
这周六12点前要交c++大作业 （work类）
每周二8点前要交概率论作业 （work类）
复习物理11章 (arrange类)
工数已经复习到7.3 总共需要复习到第10章 （progress类）
每天需要写10个高中题（work类）
乐跑已经跑了29次，总共需要跑45次，6.25号截止（progress类）
*/

//全局变量
vector<plan> globalPlan(0);                 //每天计划的集合
vector<schedule *> globalAllSchedule(0);    //所有计划的集合
vector<schedule *> globalNoPlanSchedule(0); //所有没安排时间计划的集合
int g_scheduleLong = 0;                     //每节课时长 单位分钟
timeDate g_startDate = 0;                   //计划开始日期
vector<timeScale> g_timeTab(0);             //每天时间表
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

timeDate getNowDate() //取得当前日期
{
    time_t t;
    t = time(0);
    return t / (24 * 3600);
}

int getWeek(timeDate td) //取得指定日期相对于开始日期的周数
{
    td -= g_startDate;
    return (td / 7) + 1;
}

int getWday(timeDate td) //得到星期几
{
    time_t t = td * 24 * 3600;
    tm *tt = localtime(&t);
    return tt->tm_wday;
}

vector<string> splitString(string str, char syb)
{
    //分割字符串
    size_t pos;
    vector<string> result;
    if (str[str.length() - 1] != syb)
        str += syb; //扩展字符串以方便操作
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
    //得到子字符串vector
    return result;
}

string getWdayText(int wday) //数字转文字
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
    cout << "请输入您开始上课的第一周星期一的日期（年 月 日）：";
    if (getLineVar(cin, year, mon, day))
        return;
    tmpDate->tm_year = year - 1900;
    tmpDate->tm_mon = mon - 1;
    tmpDate->tm_mday = day;
    startDate = mktime(tmpDate) / (24 * 3600); //取得日期时间戳
    g_startDate = startDate;
    cout << "请输入您每节课的时长(分钟)：";
    if (getLineVar(cin, scheduleLong))
        return;

    g_scheduleLong = scheduleLong;
    cout << "请输入您每节课的上课时间(课之间用逗号隔开)：";
    cin.get();
    getline(cin, tmpInput);
    g_timeTab = getTimeMapFromText(tmpInput);
    cout << "*************设置完成！*****************" << endl;
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

void listSchedule(int week) //显示课程
{
    system("cls");

    for (int i = 0; i < 50; i++)
        cout << "●";
    cout << endl;
    cout << setw(42) << left << "●"
         << setw(2) << week << " " << setw(6) << getDataText(g_startDate + week * 7 - 7) << " to " << setw(43) << left << getDataText(g_startDate + week * 7 - 1)
         << "●" << endl;
    cout << "●";
    for (int i = 0; i < 96; i++)
        cout << "=";
    cout << "●" << endl;

    bool b = 0;
    for (auto &a : globalPlan)
    {
        if (getWeek(a.Date) == week)
        {
            b = 1;
            cout << setw(5) << "●" << setw(5) << getDataText(a.Date) << " " << setw(86) << getWdayText(getWday(a.Date)) + ':' << "●" << endl;
            int i = 1;
            for (auto &tmpSchedule : a.scheduleList)
            {
                cout << setw(5) << "●" << setw(93) << to_string(i) + "、" + tmpSchedule->printOut() << "●" << endl;
                i++;
            }
            cout << setw(98) << "●"
                 << "●" << endl;
        }
        else if (b)
        {
            break;
        }
    }
    if (!b)
    {
        cout << setw(98) << "●"
             << "●" << endl;
        cout << setw(42) << left << "●"
             << "这周还没有计划！"
             << setw(42) << right << "●"
             << endl;
        cout << setw(98) << left << "●"
             << "●" << endl;
    }

    cout << "●";
    for (int i = 0; i < 96; i++)
        cout << "=";
    cout << "●" << endl;

    cout << setw(98) << "●"
         << "●" << endl;

    if (globalNoPlanSchedule.size() != 0)
    {
        int i = 1;
        for (auto a : globalNoPlanSchedule)
        {
            cout << setw(5) << "●" << setw(93) << to_string(i) + "、" + a->printOut() << "●" << endl;
            i++;
        }
    }
    else
    {
        cout << setw(40) << left << "●"
             << "还没有无安排的计划！"
             << setw(40) << right << "●"
             << endl;
    }
    cout << setw(98) << left << "●"
         << "●" << endl;
    for (int i = 0; i < 50; i++)
        cout << "●";
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
            if (j != globalPlan.end() && j->Date == theDate) //判断是否已经有这天的计划了
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

void addPlan() //添加计划
{
    int choice;
    cout << "您想添加哪种计划？" << endl
         << "1、课程  2、事项" << endl;
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
    //判断删除类型
    cout << "您想删除已安排的计划未还是时间的计划？（0、已安排 1、未安排）：";
    int option;
    if (getLineVar(cin, option))
        return;
    if (option)
    {
        if (globalNoPlanSchedule.size() == 0)
        {
            cout << "抱歉，没有未安排的计划，请先添加吧！" << endl;
            system("pause");
            return;
        }
        int choice;
        cout << "您想删除第几个计划？（1-" << globalNoPlanSchedule.size() << "）：";
        if (getLineVar(cin, choice))
            return;
        while (choice > globalNoPlanSchedule.size() || choice < 1)
        {
            cout << "抱歉，您的输入有误，请检查后重新输入";
            if (getLineVar(cin, choice))
                return;
        }
        auto it_theSchedule = globalNoPlanSchedule.begin() + choice - 1;
        deleteSchedule(*it_theSchedule);
    }
    else
    {

        int weekday, planRank;
        cout << "您想删除周几的计划（1-7）：";
        if (getLineVar(cin, weekday))
            return;

        timeDate theDate = g_startDate + week * 7 + weekday - 8;
        auto it_theDate = findDate(theDate);

        if (it_theDate->Date == theDate)
            cout << "您想删除第几个计划（1-" << it_theDate->scheduleList.size() << "）：";
        else
        {
            cout << "抱歉，所选的日期没有计划，请先添加吧！" << endl;
            system("pause");
            return;
        }

        if (getLineVar(cin, planRank))
            return;
        auto tmpDPschedule = *(it_theDate->scheduleList.begin() + planRank - 1); //要删除的schedule的指针
        vector<int> tmpWeekRank = tmpDPschedule->getWeekRank();                  //要删除的schedule的周数列表
        vector<int> tmpWDayRank = tmpDPschedule->getWDayRank();                  //要删除的schedule的一周内天数列表
        if (tmpWeekRank.size() > 1 || tmpWDayRank.size() > 1)                    //这是一个多次重复计划
        {
            //判断合法输入
            string str;
            do
            {
                cout << "这是一个多次重复计划，是否删除所有这些计划？（yes or no）:";
                if (getLineVar(cin, str))
                    return;
            } while (str != "yes" && str != "no");
            //判断输入yes or no
            if (str == "yes")
            {
                for (auto tmpWeek : tmpWeekRank) //遍历每一个有这个schedule的周数
                {
                    for (auto tmpWeekDay : tmpWDayRank) //遍历每一个有这个schedule的周中的天数
                    {
                        //计算日期指针
                        theDate = g_startDate + tmpWeek * 7 + tmpWeekDay - 8;
                        auto tmpDate = findDate(theDate);
                        //遍历当天的schedule找到要删除的schedule并删除
                        vector<schedule *>::iterator it = tmpDate->scheduleList.begin();
                        while (it != tmpDate->scheduleList.end())
                        {
                            if (*it == tmpDPschedule)
                            {
                                tmpDate->scheduleList.erase(it); //在当天schedule列表中删除这个schedule的指针
                                break;
                            }
                            it++;
                        }
                        //如果删空了就把这天删了
                        if (tmpDate->scheduleList.size() == 0)
                            globalPlan.erase(tmpDate);
                    }
                }
                delete tmpDPschedule; //释放这个schedule的内存
                deleteSchedule(tmpDPschedule);
            }
            else //不要全删了
            {
                //改掉这个schedule的weekRank
                tmpDPschedule->eraseWeekRank(week);
                //改掉这个schedule的wDayRank
                tmpDPschedule->eraseWDayRank(week);
                it_theDate->scheduleList.erase(it_theDate->scheduleList.begin() + planRank - 1); //在选中天数中删除这个课程的地址
                if (it_theDate->scheduleList.size() == 0)                                        //如果删空了就把这天删了
                {
                    globalPlan.erase(it_theDate);
                }
            }
        }
        else //这不是个多次重复的任务
        {
            delete tmpDPschedule; //直接释放掉内存
            deleteSchedule(tmpDPschedule);
            it_theDate->scheduleList.erase(it_theDate->scheduleList.begin() + planRank - 1);
            //如果删空了就把这天删了
            if (it_theDate->scheduleList.size() == 0)
            {
                globalPlan.erase(it_theDate);
            }
        }
    }
    cout << "***************删除成功*********************" << endl;
}

void resetPlan(int week)
{
    //判断修改类型
    cout << "您想更改已安排的计划未还是时间的计划？（0、已安排 1、未安排）：";
    int option;
    if (getLineVar(cin, option))
        return;
    if (option)
    {
        if (globalNoPlanSchedule.size() == 0)
        {
            cout << "抱歉，没有未安排的计划，请先添加吧！" << endl;
            system("pause");
            return;
        }
        int choice;
        cout << "您想修改第几个计划？（1-" << globalNoPlanSchedule.size() << "）：";
        if (getLineVar(cin, choice))
            return;
        while (choice > globalNoPlanSchedule.size() || choice < 1)
        {
            cout << "抱歉，您的输入有误，请检查后重新输入";
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
        cout << "您想修改周几的计划（1-7）：";
        if (getLineVar(cin, weekday))
            return;

        timeDate theDate = g_startDate + week * 7 + weekday - 8;
        auto it_theDate = findDate(theDate);

        if (it_theDate->Date == theDate)
            cout << "您想修改第几个计划（1-" << it_theDate->scheduleList.size() << "）：";
        else
        {
            cout << "抱歉，所选的日期没有计划，请先添加吧！" << endl;
            system("pause");
            return;
        }

        if (getLineVar(cin, planRank))
            return;
        auto tmpEPschedule = *(it_theDate->scheduleList.begin() + planRank - 1); //要修改的schedule的指针
        vector<int> tmpWeekRank = tmpEPschedule->getWeekRank();                  //要修改的schedule的周数列表
        vector<int> tmpWDayRank = tmpEPschedule->getWDayRank();                  //要修改的schedule的一周内天数列表
        if (tmpWeekRank.size() > 1 || tmpWDayRank.size() > 1)                    //这是一个多次重复计划
        {
            //判断合法输入
            string str;
            do
            {
                cout << "这是一个多次重复计划，是否修改所有这些计划？（yes or no）:";
                if (getLineVar(cin, str))
                    return;
            } while (str != "yes" && str != "no");
            //判断输入yes or no
            if (str == "yes")
            {
                for (auto tmpWeek : tmpWeekRank) //遍历每一个有这个schedule的周数
                {
                    for (auto tmpWeekDay : tmpWDayRank) //遍历每一个有这个schedule的周中的天数
                    {
                        //计算日期指针
                        theDate = g_startDate + tmpWeek * 7 + tmpWeekDay - 8;
                        auto tmpDate = findDate(theDate);
                        //遍历当天的schedule找到要删除的schedule并删除
                        vector<schedule *>::iterator it = tmpDate->scheduleList.begin();
                        while (it != tmpDate->scheduleList.end())
                        {
                            if (*it == tmpEPschedule)
                            {
                                tmpDate->scheduleList.erase(it); //在当天schedule列表中删除这个schedule的指针
                                break;
                            }
                            it++;
                        }
                        //如果删空了就把这天删了
                        if (tmpDate->scheduleList.size() == 0)
                            globalPlan.erase(tmpDate);
                    }
                }
                insertPlan(tmpEPschedule->reset()); //调用自身函数修改
            }
            else //只改一个
            {
                //改掉这个schedule的weekRank
                tmpEPschedule->eraseWeekRank(week);
                //改掉这个schedule的wDayRank
                tmpEPschedule->eraseWDayRank(week);
                it_theDate->scheduleList.erase(it_theDate->scheduleList.begin() + planRank - 1); //在选中天数中删除这个schedule的地址
                //如果删空了就把这天删了
                if (it_theDate->scheduleList.size() == 0)
                    globalPlan.erase(it_theDate);
                insertPlan(tmpEPschedule->reset(tmpEPschedule, theDate)); //改完之后插回去
            }
        }
        else //这不是个多次重复的任务
        {
            it_theDate->scheduleList.erase(it_theDate->scheduleList.begin() + planRank - 1);
            deleteSchedule(*(it_theDate->scheduleList.begin() + planRank - 1));
            //如果删空了就把这天删了
            if (it_theDate->scheduleList.size() == 0)
                globalPlan.erase(it_theDate);
            insertPlan(tmpEPschedule->reset()); //调用自身函数修改
        }
    }
    cout << "*****************修改成功*********************" << endl;
}

int main()
{
    int operation;
    if (loadInformation() == -1)
    {
        cout << "您还没有设置基本信息！" << endl;
        settingInformation();
    }
    int week = getWeek(getNowDate());
    do
    {
        listSchedule(week);
        cout << "***************************************请选择操作***************************************************" << endl
             << "\t\t1、增加计划\t2、删除计划\t3、显示上周计划\n\t\t4、显示下周计划\t5、修改计划\t6、周数跳转\n\t\t7、设置信息\t8、保存\t\t9、退出" << endl;
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
            cout << "跳转到（周）：";
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
