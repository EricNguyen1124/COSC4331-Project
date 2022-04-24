#ifndef TASK_H
#define TASK_H

class Task {
public:
    int id;
    int type;
    int period;
    int arrivalTime;
    int executionTime;
    int relativeDeadline;
    int absoluteDeadline;
    Task(int _id, int _type, int _arrivalTime, int _executionTime, int _relativeDeadline);
    Task(int _id, int _type, int _arrivalTime, int _executionTime, int _relativeDeadline, int _period);
};

#endif