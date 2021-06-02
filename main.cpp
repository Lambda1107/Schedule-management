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
vector<plan> globalPlan(0);              //每天计划的集合
vector<schedule *> globalAllSchedule(0); //所有计划的集合
int g_scheduleLong = 0;                  //每节课时长 单位分钟
timeDate g_startDate = 0;                //计划开始日期
vector<timeScale> g_timeTab(0);          //每天时间表

string getTimeMonentText(timeMonent tim)
{

    string str = "";
    str += to_string(tim / 60);
    str += ":";
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
    //分割字符串
    size_t pos;
    vector<string> result;
    if (str[str.length() - 1] != ',')
        str += ','; //扩展字符串以方便操作
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
    cout << "您还没有设置基本信息！" << endl
         << "请输入您开始上课的第一周星期一的年月日(以空格分隔)：";
    cin >> year >> mon >> day;
    tmpDate->tm_year = year - 1900;
    tmpDate->tm_mon = mon - 1;
    tmpDate->tm_mday = day;
    startDate = mktime(tmpDate) / (24 * 3600); //取得日期时间戳
    g_startDate = startDate;
    cout << "请输入您每节课的时长(分钟)：";
    cin >> scheduleLong;
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

void listSchedule(int week) //显示课程
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
                cout << i << "、";
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
    }
}
void deleteSchedule(schedule *tmpSchedule)
{
    globalAllSchedule.erase(find(globalAllSchedule.begin(), globalAllSchedule.end(), tmpSchedule));
}

void addPlan() //添加计划
{
    int choice;
    cout << "您想添加哪种计划？" << endl
         << "1、课程" << endl;
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
    cout << "您想删除周几的计划（1-7）：";
    cin >> weekday;

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

    cin >> planRank;
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
            cin >> str;
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
                            deleteSchedule(*it);
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

    cout << "***************删除成功*********************" << endl;
}

void resetPlan(int week)
{
    int weekday, planRank;
    cout << "您想修改周几的计划（1-7）：";
    cin >> weekday;

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

    cin >> planRank;
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
            cin >> str;
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
                            deleteSchedule(*it);
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
        //如果删空了就把这天删了
        if (it_theDate->scheduleList.size() == 0)
            globalPlan.erase(it_theDate);
        insertPlan(tmpEPschedule->reset()); //调用自身函数修改
    }

    cout << "***************修改成功*********************" << endl;
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
        cout << "******请选择操作*******" << endl
             << "1、增加计划   2、删除计划   3、显示上周计划   4、显示下周计划   5、修改计划   6、退出" << endl;
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
