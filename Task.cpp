#include <string>
#include <iostream>
using namespace std;

#include "Task.h"

Task::Task(int _id, int _type, int _arrivalTime, int _executionTime, int _relativeDeadline)
{
    id = _id;
    type = _type;
    period = 0;
    arrivalTime = _arrivalTime;
    executionTime = _executionTime;
    relativeDeadline = _relativeDeadline;
    absoluteDeadline = arrivalTime + relativeDeadline;
    lifeSpan = 0;
}

Task::Task(int _id, int _type, int _arrivalTime, int _executionTime, int _relativeDeadline, int _period)
{
    id = _id;
    type = _type;
    period = _period;
    arrivalTime = _arrivalTime;
    executionTime = _executionTime;
    relativeDeadline = period;
    absoluteDeadline = arrivalTime + period;
    lifeSpan = 0;
}