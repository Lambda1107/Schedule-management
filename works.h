#include "work.h"
#ifndef WORKS_H
#define WORKS_H
class works : public work
{
private:
    vector<timeDateMonent> rankTimes;

public:
    void submit(timeDate theDate);
    string printOut(timeDate theDate = 0);
    static works *addWorks();
    schedule *reset(schedule *sp = NULL, timeDate theData = 0);
    
    works(/* args */);
    ~works();
};
#endif