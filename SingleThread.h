#ifndef SINGLETHREAD_H
#define SINGLETHREAD_H

#include "Task.h"

class SingleThread {
public:
    int type;
    Task* activeTask;
    int cycleSpeedForTask(Task task);
    void stepThread();
    SingleThread(int _type);
};

#endif