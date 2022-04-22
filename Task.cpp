#include <string>
#include <iostream>
using namespace std;

#include "Task.h"

Task::Task(int _id, int _arrivalTime, int _executionTime, int _relativeDeadline)
{
    id = _id;
    period = 0;
    arrivalTime = _arrivalTime;
    executionTime = _executionTime;
    relativeDeadline = _relativeDeadline;
    absoluteDeadline = arrivalTime + relativeDeadline;
}

Task::Task(int _id, int _arrivalTime, int _executionTime, int _relativeDeadline, int _period)
{
    id = _id;
    period = _period;
    arrivalTime = _arrivalTime;
    executionTime = _executionTime;
    relativeDeadline = period;
    absoluteDeadline = arrivalTime + period;
}