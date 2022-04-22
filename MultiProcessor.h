#ifndef MULTIPROCESSOR_H
#define MULTIPROCESSOR_H
#include <iostream>
#include <vector>
#include "Task.h"
#include "SingleThread.h"
using namespace std;

class MultiProcessor {
public:
    vector<Task*> pendingTaskPool;
    vector<Task*> readyTaskPool;
    vector<SingleThread*> threads;
    int simulationStep = 0;
    bool step();
    SingleThread* getInactiveThread();
    MultiProcessor(int threadCount, vector<Task*> _pending);
};

#endif 