#include "schedule.h"
#include "works.h"
#include <iostream>
using namespace std;

works::works()
{
}

void works::submit(timeDate theDate)
{
    cout << "您想在多少天后添加相同事件？（0表示不添加，默认为0）:";
    int day;
    if (getLineVar(cin, day))
        return;
    schedule::submit(theDate);
    if (day != 0)
    {
        works &theworks = (*this);
        works *tmpScheduleP = new works(theworks);
        tmpScheduleP->DDLDate += day;
        tmpScheduleP->rankTime += day * (24 * 60);
        insertPlan(tmpScheduleP);
    }
}

schedule *works::addWorks()
{
    work *pw = work::addWork();
    if (pw == NULL)
        return NULL;
    works *pws = new works;
    pws->name = pw->name;
    pws->remark = pw->remark;
    pws->DDLDate = pw->DDLDate;
    pws->rankTime = pw->rankTime;
    pws->timeLong = pw->timeLong;
    return pws;
}