#ifndef TASK_H
#define TASK_H

class Task {
public:
    int id;
    int period;
    int arrivalTime;
    int executionTime;
    int relativeDeadline;
    int absoluteDeadline;
    Task(int _id, int _period, int _arrivalTime, int _executionTime, int _relativeDeadline);
};

#endif