#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Task {
public:
    int id;
    int period;
    int arrivalTime;
    int executionTime;
    int absoluteDeadline = arrivalTime + period;
    bool missedDeadline = false;
    //constructor needs to initialzie deadline to arrivaltime+period. might need a isPeriodic boolean for Task
};


class Processor {
public:
    Task* activeTask;
    vector<Task*> pendingTaskPool; //may need to move these pools to multiprocessor
    vector<Task*> readyTaskPool;
    int simulationStep = 0;
    bool step() {
        vector<int> idsToDelete;
        //move tasks that need to arrive into readyTaskPool
        for (int i = 0; i < pendingTaskPool.size(); i++)
        {
            if (pendingTaskPool[i]->arrivalTime == simulationStep)
            {
                readyTaskPool.push_back(pendingTaskPool[i]);
                idsToDelete.push_back(pendingTaskPool[i]->id);
            }
        }

        for (int i = 0; i < idsToDelete.size(); i++)
        {
            for (int j = 0; j < pendingTaskPool.size(); j++)
            {
                if (pendingTaskPool[j]->id == idsToDelete[i])
                {
                    pendingTaskPool.erase(pendingTaskPool.begin() + j);
                }
            }
        }

        if (activeTask != nullptr)
        {
            activeTask->executionTime = activeTask->executionTime - 1;
            //remove done task from readyTaskPool if execution is finished
            if (activeTask->executionTime == 0)
            {
                vector<Task*>::iterator position = find(readyTaskPool.begin(), readyTaskPool.end(), activeTask);
                if (position != readyTaskPool.end())
                {
                    readyTaskPool.erase(position);
                }
                activeTask = nullptr;
            }
        }
        simulationStep++;



        //if all tasks are done and gone, early return

        if (readyTaskPool.empty())
        {
            if (pendingTaskPool.empty())
            {
                cout << "DONE";
                return true;
            }
            else
            {
                cout << "no tasks ready" << endl;
                return false;
            }
        }
        else
        {
            activeTask = readyTaskPool[0];
            //this is FIFO for now, this is where i need to decide the next activeTask
            for (int i = 0; i < readyTaskPool.size(); i++)
            {
                cout << readyTaskPool[i]->id << ": " << readyTaskPool[i]->executionTime << endl;
            }
            cout << endl;
            return false;
        }
    }
    //constructor here, initialize activeTask using a scheduler (can a scheduler be a callback function for Processor?)
};

class SingleThread {
public:
    Task* activeTask;
    bool stepThread() {
        activeTask->executionTime = activeTask->executionTime - 1;
    }
};

class MultiProcessor {
public:
    vector<Task*> pendingTaskPool;
    vector<Task*> readyTaskPool;
    vector<SingleThread*> threads;
    int simulationStep = 0;
    bool step() {
        vector<int> idsToDelete;
        //move tasks that need to arrive into readyTaskPool
        for (int i = 0; i < pendingTaskPool.size(); i++)
        {
            if (pendingTaskPool[i]->arrivalTime == simulationStep)
            {
                readyTaskPool.push_back(pendingTaskPool[i]);
                idsToDelete.push_back(pendingTaskPool[i]->id);
            }
        }

        //delete ready tasks from pending
        for (int i = 0; i < idsToDelete.size(); i++)
        {
            for (int j = 0; j < pendingTaskPool.size(); j++)
            {
                if (pendingTaskPool[j]->id == idsToDelete[i])
                {
                    pendingTaskPool.erase(pendingTaskPool.begin() + j);
                }
            }
        }

        for (int i = 0; i < threads.size(); i++)
        {
            if (threads[i]->activeTask != nullptr)
            {
                threads[i]->stepThread();
                if (threads[i]->activeTask->executionTime == 0)
                {
                    vector<Task*>::iterator position = find(readyTaskPool.begin(), readyTaskPool.end(), threads[i]->activeTask);
                    if (position != readyTaskPool.end())
                    {
                        readyTaskPool.erase(position);
                    }
                }
            }
            threads[i]->activeTask = nullptr;
        }

        for (int i = 0; i < readyTaskPool.size(); i++)
        {
            for (int j = 0; j < threads.size(); j++)
            {
                if (threads[j]->activeTask == nullptr)
                {
                    threads[j]->activeTask = readyTaskPool[i];
                }
            }
        }

    }
    //constructor here, initialize activeTask using a scheduler (can a scheduler be a callback function for Processor?)
};

int main()
{
    Task task1;
    task1.arrivalTime = 0;
    task1.executionTime = 3;
    task1.id = 343;

    Task task2;
    task2.arrivalTime = 0;
    task2.executionTime = 5;
    task2.id = 420;

    Task task3;
    task3.arrivalTime = 12;
    task3.executionTime = 4;
    task3.id = 666;

    Task task4;
    task4.arrivalTime = 10;
    task4.executionTime = 8;
    task4.id = 64;

    Processor intel;
    intel.pendingTaskPool.push_back(&task1);
    intel.pendingTaskPool.push_back(&task3);
    intel.pendingTaskPool.push_back(&task2);
    intel.pendingTaskPool.push_back(&task4);
    intel.activeTask = &task1;

    while (!intel.step())
    {
        cout << "STEP" << endl;
    }

    return 0;
}
