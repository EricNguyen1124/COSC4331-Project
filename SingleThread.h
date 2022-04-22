#ifndef SINGLETHREAD_H
#define SINGLETHREAD_H

#include "Task.h"

class SingleThread {
public:
    Task* activeTask;
    void stepThread();
    SingleThread();
};

#endif