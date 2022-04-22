#ifndef ANTCOLONYSCHEDULER_H
#define ANTCOLONYSCHEDULER_H

#include <vector>
#include "Task.h"
#include "SingleThread.h"
using namespace std;

class AntColonyScheduler {
public:
    vector<Task*> tasksToSchedule;
    vector<SingleThread*> threads;
    void generateSchedule()
    {
        
    }
};

#endif