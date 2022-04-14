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
        int deadline = arrivalTime+period;
        bool missedDeadline = false;
};

class Processor {
    public:
        Task* activeTask;
        vector<Task*> pendingTaskPool;
        vector<Task*> readyTaskPool;
        int simulationStep;
        bool step() {
            simulationStep++;
            activeTask->executionTime = activeTask->executionTime - 1;
            
            //move tasks that need to arrive into readyTaskPool
            for(int i = 0; i < pendingTaskPool.size(); i++)
            {
                if(pendingTaskPool[i]->arrivalTime == simulationStep)
                {
                    vector<Task*>::iterator position = find(pendingTaskPool.begin(), pendingTaskPool.end(), pendingTaskPool[i]);
                    if(position != pendingTaskPool.end())
                    {
                        readyTaskPool.insert(readyTaskPool.end(), std::make_move_iterator(position), std::make_move_iterator(position));
                    }
                }
            }

            //move done task from readdyTaskPool
            if(activeTask->executionTime == 0)
            {
                vector<Task*>::iterator position = find(readyTaskPool.begin(), readyTaskPool.end(), activeTask);
                if(position != readyTaskPool.end())
                {
                    readyTaskPool.erase(position);
                }
            }

            if(!readyTaskPool.empty() && !pendingTaskPool.empty())
            {
                activeTask = readyTaskPool[0];
                //this is FIFO for now, this is where i need to decide the next activeTask

                for(int i = 0; i < readyTaskPool.size(); i++)
                {
                    cout<< readyTaskPool[i]->id << ": "<<readyTaskPool[i]->executionTime<<endl;
                }
                cout<<endl;
            }
            else
            {
                return true;
            }

            return false;
        }
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

    Processor intel;
    intel.pendingTaskPool.push_back(&task1);
    intel.pendingTaskPool.push_back(&task2);
    intel.activeTask = intel.readyTaskPool[0];

    while(!intel.step())
    {
        cout<<"STEP"<<endl;
    }

    return 0;
}