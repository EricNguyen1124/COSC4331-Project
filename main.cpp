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
    int relativeDeadline;
    int absoluteDeadline;
    Task(int _id, int _period, int _arrivalTime, int _executionTime, int _relativeDeadline)
    {
        id = _id;
        period = _period;
        arrivalTime = _arrivalTime;
        executionTime = _executionTime;
        relativeDeadline = _relativeDeadline;
        absoluteDeadline = arrivalTime + relativeDeadline;
    }
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
    void stepThread() {
        activeTask->executionTime = activeTask->executionTime - 1;
    }
    SingleThread()
    {
        activeTask = nullptr;
    }
};

class MultiProcessor {
public:
    vector<Task*> pendingTaskPool;
    vector<Task*> readyTaskPool;
    vector<SingleThread*> threads;
    int simulationStep = 0;
    bool step() {
        cout<<"STEP "<<simulationStep<<endl;
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

        simulationStep++;

        for(int i = 0; i < readyTaskPool.size(); i++)
        {
            if(readyTaskPool[i]->absoluteDeadline < simulationStep)
            {
                cout<<"TASK # "<<readyTaskPool[i]->id<<" Failed in the ready pool"<<endl;
                return true;
            }
        }

        for(int i = 0; i < pendingTaskPool.size(); i++)
        {
            if(pendingTaskPool[i]->absoluteDeadline < simulationStep)
            {
                cout<<"TASK # "<<pendingTaskPool[i]->id<<" Failed in the ready pool"<<endl;
                return true;
            }
        }

        if(readyTaskPool.empty())
        {
            if(pendingTaskPool.empty())
            {
                cout<<"Done"<<endl;
                return true;
            }
            else
            {
                cout<<"No Tasks Ready"<<endl;
                return false;
            }
        }
        else
        {
            for (int i = 0; i < readyTaskPool.size(); i++)
            {
                SingleThread* inactive = getInactiveThread();
                if(inactive == nullptr)
                {
                    break;
                }

                inactive->activeTask = readyTaskPool[i];
            }

            for (int j = 0; j < threads.size(); j++)
            {
                if(threads[j]->activeTask != nullptr)
                {
                    cout<<"Thread Number: "<<j<<"  "<<"  Task ID: "<< threads[j]->activeTask->id<<"  Remaining: "<< threads[j]->activeTask->executionTime << endl;
                }
                else
                {
                    cout<<"Thread Number: "<<j<<"  "<<"  Task ID: "<<"  No task!"<<endl;
                }
            }
        }
        return false;
    }
    SingleThread* getInactiveThread()
    {
        for(int i = 0; i < threads.size(); i++)
        {
            if(threads[i]->activeTask == nullptr)
            {
                return threads[i];
            }
        }
        return nullptr;
    }
    //constructor here, initialize activeTask using a scheduler (can a scheduler be a callback function for Processor?)
    MultiProcessor(int threadCount, vector<Task*> _pending) : threads(), readyTaskPool()
    {
        pendingTaskPool = _pending;
        threads.reserve(threadCount);
        for(int i = 0; i < threadCount; i++)
        {
            threads.push_back(new SingleThread());
        }
    }
};

int main()
{
    Task task1(343, 0, 0, 3, 50);

    Task task2(420, 0, 0, 5, 50);

    Task task3(666, 0, 12, 4, 50);

    Task task4(64, 0, 10, 8, 50);

    vector<Task*> pend;

    Processor intel;
    
    pend.push_back(&task1);
    pend.push_back(&task3);
    pend.push_back(&task2);
    pend.push_back(&task4);

    MultiProcessor amd(5, pend);
    intel.activeTask = &task1;

    while (!amd.step())
    {
        cout << "STEP" << endl;
    }

    return 0;
}
