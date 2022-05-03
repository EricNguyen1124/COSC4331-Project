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
    vector<Task*> periodicTasks;
    int simulationStep = 0;
    int totalExecutionTime;
    bool step();
    SingleThread* getInactiveThread();
    bool taskAlreadyAssigned(Task* task);
    MultiProcessor(int threadCount, vector<Task*> _pending);
    vector<pair<SingleThread*, Task*>> findBestAssginments(vector<pair<SingleThread*, vector<pair<Task*, int>>>>);
};

#endif 