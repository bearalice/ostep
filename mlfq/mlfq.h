#ifndef MLFQ_H
#define MLFQ_H

#include <fstream>
#include <queue>
#include <iostream>
#include <string>
#include <iomanip>
#include <vector>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <map>
#include <deque>
#include <memory>

using namespace std;

// construct a process by providing id, arrivalTime, burstTime, and ioFreq
// io time i fixed
class Process
{
public:
    int id;
    int arrivalTime;
    int startTime;
    int burstTime;
    int ioFreq = 0;
    int IOTime = 0;
    int curQueue = -1;
    int ticksLeft;
    int allotLeft;
    int executedTime = 0;
    bool inIO = false;
    int endTime;
    int responseTime = -1; // response time

    Process(int id, int arrivalTime, int burstTime, int ioFreq)
    {
        this->id = id;
        this->startTime = arrivalTime;
        this->arrivalTime = arrivalTime;
        this->burstTime = burstTime;
        this->ioFreq = ioFreq;
    }
};

// quantum time, or time slice is fixed
// allotment time is fixed
class Queue
{
public:
    deque<shared_ptr<Process>> queue;
    int quantum = 10;
    int allotment = 1;
    Queue(){};
    Queue(int quantum, int allotment)
    {
        this->quantum = quantum;
        this->allotment = allotment;
        this->queue = deque<shared_ptr<Process>>();
    };
};

struct ProcessComparator
{
    bool operator()(shared_ptr<Process> &a, shared_ptr<Process> &b)
    {
        return a->startTime > b->startTime;
    }
};

class MLFQ
{
public:
    map<int, shared_ptr<Queue>> queues;

    vector<shared_ptr<Process>> finishedProcesses;

    priority_queue<shared_ptr<Process>, vector<shared_ptr<Process>>, ProcessComparator> readyQueue;

    int totalProcesses = 0;
    int totalFinishedProcesses = 0;
    int currentTime = 0;
    int numQueues = 0;
    int boostTime = 100;

    int lastRunProcess = -1;

    // MLFQ(int numQueues);

public:
    // constructor of MLFQ
    MLFQ(int numQueues)
    {
        this->numQueues = numQueues;
        for (int i = 0; i < numQueues; i++)
        {
            queues[i] = make_shared<Queue>();
        }
    }

    int findQueue()
    {
        for (int i = numQueues - 1; i >= 0; i--)
        {
            if ((queues[i]->queue).size() > 0)
            {
                return i;
            }
        }
        return -1;
    }

    void boost()
    {
        for (int i = 0; i < numQueues; i++)
        {
            if (queues[i]->queue.size() > 0)
            {
                while (queues[i]->queue.empty() == false)
                {
                    auto p = (queues[i]->queue).front();
                    p->curQueue = numQueues - 1;
                    p->ticksLeft = queues[numQueues - 1]->quantum;
                    p->allotLeft = queues[numQueues - 1]->allotment;
                    queues[i]->queue.pop_front();
                    queues[numQueues - 1]->queue.push_back(p);
                }
            }
        }
        cout << "Boosted at time" << currentTime << endl;
    }

    void addProcess(shared_ptr<Process> p)
    {
        if (p->curQueue == -1)
        {
            p->curQueue = numQueues - 1;
            queues[numQueues - 1]->queue.push_front(p);
        }
        else
        {
            if (p->inIO)
            {
                cout << "Job " << p->id << " IO finishes at time "
                     << "[" << currentTime << "]" << endl;
                p->inIO = false;
                queues[p->curQueue]->queue.push_front(p);
                p->ticksLeft = queues[p->curQueue]->quantum;
                p->allotLeft = queues[p->curQueue]->allotment;
            }
            else
            {

                queues[p->curQueue]->queue.push_back(p);
            }
        }
    }

    void show()
    {
        cout << "Final statistics:" << endl;
        for (int i = 0; i < finishedProcesses.size(); i++)
        {
            auto p = finishedProcesses[i];
            cout << "Job " << p->id << " arrived at "
                 << "[" << p->arrivalTime << "]"
                 << "response time at "
                 << "[" << p->responseTime << "] "
                 << "turnaround time is " << p->endTime - p->arrivalTime << endl;
        }
    }

    void run()
    {
        while (totalFinishedProcesses < totalProcesses)
        {
            while (readyQueue.empty() == false && readyQueue.top()->startTime <= currentTime)
            {
                auto p = readyQueue.top();
                readyQueue.pop();
                addProcess(p);
                cout << "added job: " << p->id << endl;
            }

            // find the queue with the highest priority
            int queueIndex = findQueue();
            // cout << "now q with highest priority is: " << queueIndex << endl;
            if (queueIndex == -1)
            {
                currentTime++;
                continue;
            }

            // get the job from the queue
            auto p = queues[queueIndex]->queue.front();
            queues[queueIndex]->queue.pop_front();

            if (lastRunProcess == -1 || p->id != lastRunProcess)
            {
                lastRunProcess = p->id;
                p->ticksLeft = queues[p->curQueue]->quantum;
                p->allotLeft = queues[p->curQueue]->allotment;
            }

            // check if this is the first time running the job
            if (p->responseTime == -1)
            {
                p->responseTime = currentTime;
                cout << "Job " << p->id << " starts to run at time "
                     << "[" << currentTime << "]" << endl;
            }

            p->executedTime += queues[p->curQueue]->allotment;
            p->ticksLeft -= p->allotLeft;

            cout << "[" << currentTime << "] "
                 << "Run Job " << p->id << " at priority " << p->curQueue << " [Ticks " << p->ticksLeft << " Allot " << p->allotLeft << " worked " << p->executedTime << " of " << p->burstTime << " ]" << endl;

            // check if the job is finished
            if (p->executedTime >= p->burstTime)
            {
                cout << "Job " << p->id << " finished at time "
                     << "[" << currentTime + 1 << "]" << endl;
                finishedProcesses.push_back(p);
                totalFinishedProcesses += 1;
                p->endTime = currentTime;
                currentTime += 1;
                continue;
            }

            // check if the job is doing IO
            if (p->ioFreq != 0 && p->executedTime % p->ioFreq == 0)
            {
                p->inIO = true;
                cout << "Job " << p->id << " starts to do IO at time "
                     << "[" << currentTime + 1 << "]" << endl;
                p->startTime = currentTime + p->IOTime;
                readyQueue.push(p);
            }

            // check if the job is using up its allotment
            if (p->ticksLeft <= 0)
            {
                int nextQueue = max(0, p->curQueue - 1);
                p->curQueue = nextQueue;
                if (p->inIO == false)
                {
                    queues[p->curQueue]->queue.push_back(p);
                }

                p->ticksLeft = queues[p->curQueue]->quantum;
                p->allotLeft = queues[p->curQueue]->allotment;
                cout << "Job " << p->id << " is moved to priority " << p->curQueue << " at time "
                     << "[" << currentTime + 1 << "]" << endl;
            }
            else
            {
                if (p->inIO == false)
                {
                    queues[p->curQueue]->queue.push_front(p);
                }
            }

            // check if boost
            if (currentTime != 0 && currentTime % boostTime == 0)
            {
                boost();
            }
            currentTime += 1;
        }
    }
};

#endif