#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#include "Task.h"
#include "MultiProcessor.h"

vector<Task*> generatePeriodicTaskSet (vector<Task*> periodicTasks, int timeLimit)
{
    vector<Task*> periodicTaskSet;
    for(int task = 0; task < periodicTasks.size(); task++)
    {
        Task* currentTask = periodicTasks[task];
        int absoluteArrival = currentTask->arrivalTime;
        while(absoluteArrival <= timeLimit)
        {
            periodicTaskSet.push_back(new Task(currentTask->id+absoluteArrival, currentTask->type, absoluteArrival, currentTask->executionTime, 0, currentTask->period));
            absoluteArrival = absoluteArrival + currentTask->period;
        }
    }

    return periodicTaskSet;
}

bool compareArrivalTime(Task* task1, Task* task2)
{
    return task1->arrivalTime < task2->arrivalTime;
}

int main()
{
    Task task1(343, 0, 0, 3, 50);
    Task task2(420, 1, 0, 5, 50);
    Task task3(666, 2, 12, 4, 50);
    Task task4(64, 3, 10, 8, 50);

    Task pTask1(1000, 0, 0, 2, 0, 4);
    Task pTask2(2000, 1, 3, 5, 0, 10);
    Task pTask3(3000, 2, 5, 7, 0, 12);
    Task pTask4(4000, 3, 0, 3, 0, 5);
    Task pTask5(5000, 4, 0, 5, 0, 7);

    vector<Task*> pend;

    vector<Task*> pen;

    pend.push_back(&task1);
    pend.push_back(&task3);
    pend.push_back(&task2);
    pend.push_back(&task4);

    pen.push_back(&pTask1);
    pen.push_back(&pTask2);
    pen.push_back(&pTask3);
    pen.push_back(&pTask4);
    pen.push_back(&pTask5);

    vector<Task*> resultVec = generatePeriodicTaskSet(pen, 24);

    sort(resultVec.begin(), resultVec.end(), compareArrivalTime);
    MultiProcessor amd(3, resultVec);
    amd.periodicTasks = pen;

    while (!amd.step())
    {
        cout<<endl;
    }
    cout<<(double)amd.totalExecutionTime/resultVec.size();
    return 0;
}
