#include <string>
#include <iostream>
using namespace std;

#include "Task.h"

Task::Task(int _id, int _period, int _arrivalTime, int _executionTime, int _relativeDeadline)
{
    id = _id;
    period = _period;
    arrivalTime = _arrivalTime;
    executionTime = _executionTime;
    relativeDeadline = _relativeDeadline;
    absoluteDeadline = arrivalTime + relativeDeadline;
}
