#include "MultiProcessor.h"
#include <algorithm>

MultiProcessor::MultiProcessor(int threadCount, vector<Task*> _pending) : threads(), readyTaskPool()
{
    pendingTaskPool = _pending;
    threads.reserve(threadCount);
    for(int i = 0; i < threadCount; i++)
    {
        threads.push_back(new SingleThread());
    }
}

bool MultiProcessor::step()
{
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
            cout<<"TASK # "<<pendingTaskPool[i]->id<<" Failed in the pending pool"<<endl;
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

SingleThread* MultiProcessor::getInactiveThread()
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