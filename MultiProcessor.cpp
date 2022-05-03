#include "MultiProcessor.h"
#include "AntColonyScheduler.h"
#include <algorithm>

MultiProcessor::MultiProcessor(int threadCount, vector<Task*> _pending) : threads(), readyTaskPool(), periodicTasks()
{
    pendingTaskPool = _pending;
    threads.reserve(threadCount);
    for(int i = 0; i < threadCount; i++)
    {
        threads.push_back(new SingleThread(i));
    }
    totalExecutionTime = 0;
    AntColonyScheduler antsAnts;
    antsAnts.threads = threads;
    antsAnts.tasks = periodicTasks;
    antsAnts.initialize();
    for (int i = 0; i < 1001; i++)
    {
        antsAnts.createSolutions();
        antsAnts.iteration = antsAnts.iteration + 1;
        if(antsAnts.bestTourSoFar.size() == antsAnts.tasks.size() && antsAnts.bestTourSoFarLifeSpan > 20)
        {
            break;
        }
    }
    cout<<"debug"<<endl;
    cout<<"finished"<<endl;
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
            if (threads[i]->activeTask->executionTime <= 0)
            {
                totalExecutionTime = threads[i]->activeTask->lifeSpan + totalExecutionTime;
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
        //EDF
        for (int i = 0; i < readyTaskPool.size(); i++)
        {
            SingleThread* inactive = getInactiveThread();
            if(inactive == nullptr)
            {
                break;
            }

            inactive->activeTask = readyTaskPool[i];
        }

        //ACO

        vector<pair<SingleThread*, vector<pair<Task*, int>>>> fuc;


        for (int i = 0; i < threads.size(); i++)
        {
            vector<pair<Task*, int>> taskToCycleSpeed;
            for (int j = 0; j < readyTaskPool.size(); j++)
            {
                taskToCycleSpeed.push_back(pair<Task*, int>(readyTaskPool[j], threads[i]->cycleSpeedForTask(*readyTaskPool[j])));
            }
            fuc.push_back(pair<SingleThread*, vector<pair<Task*, int>>>(threads[i], taskToCycleSpeed));
        }
        //sort(fuc.begin(), fuc.end(), [](const pair<SingleThread*, pair<Task*, int>>& lhs, const pair<SingleThread*, pair<Task*, int>>& rhs){return lhs.second.second > rhs.second.second;});

        for (int i = 0; i < fuc.size(); i++)
        {
            sort(fuc[i].second.begin(), fuc[i].second.end(), [](const pair<Task*,int> & lhs, const pair<Task*,int>& rhs){return lhs.second > rhs.second;});
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

bool MultiProcessor::taskAlreadyAssigned(Task* task)
{
    for (int i = 0; i < threads.size(); i++)
    {
        if(threads[i]->activeTask == task)
        {
            return true;
        }
    }
    return false;
}

vector<pair<SingleThread*, Task*>> MultiProcessor::findBestAssginments(vector<pair<SingleThread*, vector<pair<Task*, int>>>> fuc)
{
    vector<pair<SingleThread*, Task*>> result;

    for (int i = 0; i < threads.size(); i++)
    {
        
    }
    
    
    return result;
}