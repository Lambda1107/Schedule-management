#include "schedule.h"
#include "work.h"
#ifndef WORKS_H
#define WORKS_H
class works : public work
{
public:
    void submit(timeDate theDate);
    static schedule *addWorks();
    works();
};
#endif