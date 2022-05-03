#include "SingleThread.h"
#include <cmath>  

SingleThread::SingleThread(int _type){
    activeTask = nullptr;
    type = _type;
}

int SingleThread::cycleSpeedForTask(Task task){
    int result = 4 - abs(task.type - type);
    if(result < 1)
    {
        return 1;
    }
    else
    {
        return result;
    }
}

void SingleThread::stepThread(){
    activeTask->executionTime = activeTask->executionTime - cycleSpeedForTask(*activeTask);
    activeTask->lifeSpan = activeTask->lifeSpan + 1;
}