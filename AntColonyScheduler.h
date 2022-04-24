#ifndef ANTCOLONYSCHEDULER_H
#define ANTCOLONYSCHEDULER_H

#include <vector>
#include "Task.h"
#include "SingleThread.h"
#include <map>
using namespace std;

class ProcessorUtilization {
public:
    SingleThread* thread;
    double utilization;
    void setUtilization(Task task)
    {
        utilization = (task.executionTime/thread->cycleSpeedForTask(task))/task.period;
        if(utilization >= 1)
        {
            utilization = 99999999;
        }
    }
    ProcessorUtilization()
    {
        thread = nullptr;
        utilization = 0;
    }
    ProcessorUtilization(SingleThread* _single, Task task)
    {
        thread = _single;

        utilization = ((double)task.executionTime/thread->cycleSpeedForTask(task))/task.period;
        if(utilization >= 1)
        {
            utilization = 99999999;
        }
    }
};

class AntColonyScheduler {
public:
    vector<Task*> tasks;
    vector<SingleThread*> threads;
    map<Task*, vector<ProcessorUtilization>> utilizationGraph;
    void initialize()
    {
        for(int i = 0; i < tasks.size(); i++)
        {
            vector<ProcessorUtilization> fuck;
            for(int j = 0; j < threads.size(); j++)
            {
                fuck.push_back(ProcessorUtilization(threads[j], *tasks[i]));
            }
            utilizationGraph.insert(pair<Task*, vector<ProcessorUtilization>>(tasks[i], fuck));
        }
    }
};

#endif