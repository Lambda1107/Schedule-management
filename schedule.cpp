#include <algorithm>
#include "schedule.h"
void schedule::submit(timeDate theDate)
{
    if (theDate == -1)
    {
        for (auto tmpDate : getRankDate())
        {
            isSubmit[tmpDate] = true;
        }
        return;
    }
    isSubmit[theDate] = true;
}