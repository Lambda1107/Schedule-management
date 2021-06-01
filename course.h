#include "main.h"
#include <string>
#include <vector>
#include <ctime>
using namespace std;
#ifndef COURSE_H
#define COURSE_H

class course
{
private:
    string name;
    string site;
    int rank;
    int rankNum;
    vector<int> courseWeeks;

public:
    course();
    course(string _name, string _site, int _rank, int _rankNum, vector<int> &_courseWeeks);
    ~course();
    timeMonent getStartTime();
    timeMonent getEndTime();
    void printOut();
    vector<int> isMut();
    void eraseWRank(int week);
};

struct plan
{
    timeDate Date;
    vector<course> courseList;
};

#endif