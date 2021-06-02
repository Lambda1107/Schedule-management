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
/*测试数据
这周五晚上6点跟曾兆杰去吃烧烤  （arrange类）
2 4 6 8周的周四晚上有课 新一代网络体系架构 7：00到9：00 （course类）
这周六12点前要交c++大作业 （work类）
每周二8点前要交概率论作业 （work类）
复习物理11章 (arrange类)
工数已经复习到7.3 总共需要复习到第10章 （progress类）
每天需要写10个高中题（work类）
乐跑已经跑了29次，总共需要跑45次，6.25号截止（progress类）
*/

//全局变量
vector<plan> globalPlan(0);     //每天计划的集合
int g_courseLong = 0;           //每节课时长 单位分钟
timeDate g_startDate = 0;       //计划开始日期
vector<timeScale> g_timeTab(0); //每天时间表

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
    cout << "您还没有设置基本信息！" << endl
         << "请输入您开始上课的第一周星期一的年月日(以空格分隔)：";
    cin >> year >> mon >> day;
    tmpDate->tm_year = year - 1900;
    tmpDate->tm_mon = mon - 1;
    tmpDate->tm_mday = day;
    startDate = mktime(tmpDate) / (24 * 3600); //取得日期时间戳
    g_startDate = startDate;
    cout << "请输入您每节课的时长(分钟)：";
    cin >> courseLong;
    g_courseLong = courseLong;
    cout << "请输入您每节课的上课时间(课之间用逗号隔开)：";
    cin.get();
    getline(cin, tmpInput);
    g_timeTab = getTimeMapFromText(tmpInput);
    cout << "*************设置完成！*****************" << endl;
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
void listCourse(int week) //显示课程
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
                cout << i << "、";
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

void addPlan() //添加课程
{
    int weekday, courseRank, courseNum;
    string name, site, str;
    cout << "请输入课程在周几(1-7)：";
    cin >> weekday;
    cout << "请输入课程是第几节课开始(1-" << g_timeTab.size() << ")：";
    cin >> courseRank;
    cout << "请输入总共持续几节课：(1-" << g_timeTab.size() - courseRank + 1 << ")：";
    cin >> courseNum;
    cout << "请输入课程名：";
    cin.ignore(1, '\n');
    getline(cin, name);
    cout << "请输入课程地点：";
    getline(cin, site);
    cout << "请输入上课周数(以逗号分隔)：";
    getline(cin, str);
    size_t pos; //分割字符串
    vector<int> result;
    str += ','; //扩展字符串以方便操作
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
    //result向量就是上课周数
    course *tmpCourse = new course(name, site, courseRank, courseNum, result);
    for (auto &a : result)
    {
        timeDate theDate = g_startDate + a * 7 + weekday - 8;
        auto j = findDate(theDate);
        if (j != globalPlan.end() && j->Date == theDate) //判断是否已经有这天的计划了
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
    cout << "您想删除周几的计划（1-7）：";
    cin >> weekday;

    timeDate theDate = g_startDate + week * 7 + weekday - 8;
    auto j = findDate(theDate);
    if (j != globalPlan.end())
        cout << "您想删除第几个计划（1-" << j->courseList.size() << "）：";
    else
    {
        cout << "抱歉，所选的日期没有计划，请先添加吧！";
        return;
    }
    cin >> planRank;
    auto tmpDPcourse = *(j->courseList.begin() + planRank - 1); //要删除的course的指针
    vector<int> tmpWeekRank = tmpDPcourse->isMut();             //要删除的course的周数列表
    if (tmpWeekRank.size() > 1)                                 //这是一个多次重复计划
    {
        string str;
        do //判断合法输入
        {
            cout << "这是一个多次重复计划，是否删除所有这些计划？（yes or no）:";
            cin >> str;
        } while (str != "yes" && str != "no");
        if (str == "yes")
        {
            for (auto v : tmpWeekRank) //遍历每一个有这个课的周数
            {
                theDate = g_startDate + v * 7 + weekday - 8;
                auto tmpDate = findDate(theDate);
                vector<course *>::iterator it = tmpDate->courseList.begin();
                while (it != tmpDate->courseList.end()) //遍历当天的课找到要删除的课程并删除
                {
                    if (*it == tmpDPcourse)
                    {
                        tmpDate->courseList.erase(it); //在当天课程列表中删除这个课程的指针
                        break;
                    }
                    it++;
                }
                if (tmpDate->courseList.size() == 0)
                    globalPlan.erase(tmpDate); //如果删空了就把这天删了
            }
            delete tmpDPcourse; //释放这个课程的内存
        }
        else //不要全删了
        {
            //改掉这个课程的排课周
            tmpDPcourse->eraseWRank(week);
            j->courseList.erase(j->courseList.begin() + planRank - 1); //在选中天数中删除这个课程的地址
            if (j->courseList.size() == 0)                             //如果删空了就把这天删了
            {
                globalPlan.erase(j);
            }
        }
    }
    else //这不是个多次重复的任务
    {
        delete tmpDPcourse;
        j->courseList.erase(j->courseList.begin() + planRank - 1);
        if (j->courseList.size() == 0)
        {
            globalPlan.erase(j);
        }
    }

    cout << "***************删除成功*********************" << endl;
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
        cout << "******请选择操作*******" << endl
             << "1、增加课程   2、删除课程   3、显示下周课程   4、显示上周课程   5、修改课程   6、退出" << endl;
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
