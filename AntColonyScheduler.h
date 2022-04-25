#ifndef ANTCOLONYSCHEDULER_H
#define ANTCOLONYSCHEDULER_H

#include <vector>
#include "Task.h"
#include "SingleThread.h"
#include <map>
#include <random>
#include <math.h>
#include <iostream>
using namespace std;

class ProcessorUtilization {
public:
    SingleThread* thread;
    double utilization;
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

class Visit{
public:
    Task* task;
    SingleThread* processor;
    Visit(Task* _task, SingleThread* _thread)
    {
        task = _task;
        processor = _thread;
    }
};

enum solutionStatus {Partial, Feasible, Infeasible};

inline bool operator<(const Visit &v1, const Visit &v2)
{
    return ((v1.task->id + v1.processor->type) < (v2.task->id + v2.processor->type));
}

class Ant{
public:
    vector<Visit> tour;
    solutionStatus solutionStatus = Partial;
    double getCumulativeUtilizationWithTour(map<Task*, vector<ProcessorUtilization>> utilizationGraph, SingleThread* thread)
    {
        double cumulativeUtil = 0.0;

        for (int i = 0; i < tour.size(); i++)
        {
            if(tour[i].processor == thread)
            {
                for (int j = 0; j < utilizationGraph[tour[i].task].size(); j++)
                {
                    if(utilizationGraph[tour[i].task].at(j).thread == thread)
                    {
                        cumulativeUtil = cumulativeUtil + utilizationGraph[tour[i].task].at(j).utilization;
                    }
                }
            }
        }
        
        return cumulativeUtil;
    }

    double getCumulativeUtilization(map<Task*, vector<ProcessorUtilization>> utilizationGraph, int threadNumber)
    {
        double cumulativeUtil;
        for( map<Task*,vector<ProcessorUtilization>>::iterator iter=utilizationGraph.begin(); iter!=utilizationGraph.end(); ++iter)  
        {  
            cumulativeUtil = cumulativeUtil + (*iter).second[threadNumber].utilization;
        }
        return cumulativeUtil;
    }

    vector<Visit> getViableNeighbors(map<Task*, vector<ProcessorUtilization>> utilizationGraph, vector<SingleThread*> threads)
    {
        vector<Visit> results;
        vector<Visit> visitsToDelete;

        for( map<Task*,vector<ProcessorUtilization>>::iterator iter=utilizationGraph.begin(); iter!=utilizationGraph.end(); ++iter)  
        {  
            for (int i = 0; i < (*iter).second.size(); i++)
            {
                results.push_back(Visit((*iter).first, (*iter).second[i].thread));
            }
        }

        for(int j = 0; j < tour.size(); j++)
        {
            for (int i = 0; i < results.size(); i++)
            {
                if(results.at(i).task == tour.at(j).task)
                {
                    visitsToDelete.push_back(results.at(i));
                }
            }
        }

        for (int i = 0; i < visitsToDelete.size(); i++)
        {
            for (int j = 0; j < results.size(); j++)
            {
                if (results[j].task == visitsToDelete[i].task && results[j].processor == visitsToDelete[i].processor)
                {
                    results.erase(results.begin() + j);
                }
            }
        }
        
        vector<double> utilizationsForAllThreads;

        for (int i = 0; i < threads.size(); i++)
        {
            utilizationsForAllThreads.push_back(getCumulativeUtilizationWithTour(utilizationGraph, threads[i]));
        }

        for (int i = 0; i < utilizationsForAllThreads.size(); i++)
        {
            for (int j = 0; j < results.size(); j++)
            {
                if(utilizationGraph[results[j].task].at(i).utilization + utilizationsForAllThreads[i] >= 1)
                {
                    visitsToDelete.push_back(results.at(j));
                }
            }
        }

        for (int i = 0; i < visitsToDelete.size(); i++)
        {
            for (int j = 0; j < results.size(); j++)
            {
                if (results[j].task == visitsToDelete[i].task && results[j].processor == visitsToDelete[i].processor)
                {
                    results.erase(results.begin() + j);
                }
            }
        }

        return results;
    }

    void createSchedule()
    {
        default_random_engine generator;
        uniform_real_distribution<double> distribution(0.0, 1.0);
        double randomResult = distribution(generator);
    }
};

class AntColonyScheduler {
public:
    vector<Task*> tasks;
    vector<SingleThread*> threads;
    map<Task*, vector<ProcessorUtilization>> utilizationGraph;
    map<Visit, double> pheromoneTrails;
    double pheromoneDecayRate = 0.02;
    vector<Ant*> ants;
    vector<Visit> bestTourSoFar;
    int bestTourSoFarLifeSpan = 0;
    int iteration = 1;
    double minConstant = 20.0;
    void initialize()
    {
        double pheromoneMax = (double)tasks.size()/(pheromoneDecayRate);

        for(int i = 0; i < tasks.size(); i++)
        {
            vector<ProcessorUtilization> utilizations;
            for(int j = 0; j < threads.size(); j++)
            {
                utilizations.push_back(ProcessorUtilization(threads[j], *tasks[i]));
                Visit iterVisit(tasks[i], threads[j]);
                pair<Visit, double> newPair(iterVisit, pheromoneMax);
                pheromoneTrails.insert(newPair);
            }
            utilizationGraph.insert(pair<Task*, vector<ProcessorUtilization>>(tasks[i], utilizations));
        }

        for (int i = 0; i < 50; i++)
        {
            ants.push_back(new Ant());
        }
        
    }
    void createSolutions()
    {
        double maxPheromone = (double)tasks.size()/(pheromoneDecayRate);
        double minPheromone = maxPheromone/(log(iteration + 1) * minConstant);
        do
        {
            for (int i = 0; i < ants.size(); i++)
            {
                if(ants[i]->solutionStatus == Feasible || ants[i]->solutionStatus == Infeasible)
                {
                    continue;
                }

                vector<Visit> viableNeighbors = ants[i]->getViableNeighbors(utilizationGraph, threads);
                if(viableNeighbors.size() == 0)
                {
                    if(ants[i]->tour.size() == tasks.size())
                    {
                        ants[i]->solutionStatus = Feasible;
                        continue;
                    }
                    else
                    {
                        ants[i]->solutionStatus = Infeasible;
                        continue;
                    }
                }

                vector<double> weights;
                double weightsTotal = 0;
                for (int j = 0; j < viableNeighbors.size(); j++)
                {
                    double weight = pheromoneTrails[viableNeighbors[j]];
                    weights.push_back(weight);
                    weightsTotal = weightsTotal + weight;
                }
                random_device rd;
                default_random_engine generator(rd());
                uniform_real_distribution<double> distribution(0.0, (double)weightsTotal);
                double randomResult = distribution(generator);

                int result = 0;

                for (int j = 0; j < viableNeighbors.size(); j++)
                {
                    if(randomResult < weights[j])
                    {
                        result = j;
                        break;
                    }
                    randomResult = randomResult - weights[j];
                }

                ants[i]->tour.push_back(viableNeighbors[result]);
            }
        } while(checkAntsFinished());

        vector<Visit> bestTourInIteration;
        for (int i = 0; i < ants.size(); i++)
        {
            if (bestTourInIteration.size() < ants[i]->tour.size())
            {
                bestTourInIteration = ants[i]->tour;
            }
        }
        
        if(bestTourSoFar.size() < bestTourInIteration.size())
        {
            bestTourSoFar = bestTourInIteration;
            //cout<<"Score of Best Tour is now " <<((double)bestTourSoFar.size()/(double)tasks.size())*100<<"%"<<endl ;
            cout<<"Score of Best Tour is now " <<bestTourSoFar.size()<<endl;
            bestTourSoFarLifeSpan = 0;
        }
        else
        {
            bestTourSoFarLifeSpan = bestTourSoFarLifeSpan + 1;
        }

        if(bestTourSoFarLifeSpan > 40)
        {
            for( map<Visit, double>::iterator iter=pheromoneTrails.begin(); iter!=pheromoneTrails.end(); ++iter)  
            {  
                (*iter).second = minPheromone;
            }
            bestTourSoFarLifeSpan = 0;
        }
        else
        {
            for( map<Visit, double>::iterator iter=pheromoneTrails.begin(); iter!=pheromoneTrails.end(); ++iter)  
            {  
                if(inBestTour((*iter).first, bestTourInIteration))
                {
                    double result = (((double) 1.0 - pheromoneDecayRate) * (*iter).second) + bestTourInIteration.size();
                    if(result >= maxPheromone)
                    {
                        (*iter).second = maxPheromone;
                    }
                    else if(result <= minPheromone)
                    {
                        (*iter).second = minPheromone;
                    }
                    else
                    {
                        (*iter).second = result;
                    }
                }
                else
                {
                    double result = ((double) 1.0 - pheromoneDecayRate) * (*iter).second;
                    if(result >= maxPheromone)
                    {
                        (*iter).second = maxPheromone;
                    }
                    else if(result <= minPheromone)
                    {
                        (*iter).second = minPheromone;
                    }
                    else
                    {
                        (*iter).second = result;
                    }
                }
            }
        }
        
        resetAnts();
    }

    bool inBestTour(Visit visit, vector<Visit> bestTour)
    {
        for (int i = 0; i < bestTour.size(); i++)
        {
            if(bestTour[i].task == visit.task && bestTour[i].processor == visit.processor)
            {
                return true;
            }
        }
        return false;
    }

    bool checkAntsFinished()
    {
        for (int i = 0; i < ants.size(); i++)
        {
            if(ants[i]->solutionStatus == Partial)
            {
                return true;
            }
        }
        return false;
    }

    void resetAnts()
    {
        for (int i = 0; i < ants.size(); i++)
        {
            ants[i]->tour.clear();
            ants[i]->solutionStatus = Partial;
        }
        
    }
};

#endif