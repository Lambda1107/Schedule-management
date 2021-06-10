#include "schedule.h"
#include <ctime>
#ifndef PROGRESS_H
#define PROGESS_H
class progress : public schedule
{
private:
    tm deadline;
    int progressBar;
public:
    progress(/* args */);
    ~progress();
};
#endif