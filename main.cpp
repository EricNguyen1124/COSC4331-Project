#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#include "Task.h"
#include "MultiProcessor.h"

int main()
{
    Task task1(343, 0, 0, 3, 50);

    Task task2(420, 0, 0, 5, 50);

    Task task3(666, 0, 12, 4, 50);

    Task task4(64, 0, 10, 8, 50);

    vector<Task*> pend;

    pend.push_back(&task1);
    pend.push_back(&task3);
    pend.push_back(&task2);
    pend.push_back(&task4);

    MultiProcessor amd(5, pend);

    while (!amd.step())
    {
        cout << "STEP" << endl;
    }

    return 0;
}
