#include "SingleThread.h"

SingleThread::SingleThread(){
    activeTask = nullptr;
}

void SingleThread::stepThread(){
    activeTask->executionTime = activeTask->executionTime - 1;
}