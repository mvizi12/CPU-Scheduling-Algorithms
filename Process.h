#pragma once
#include <string>
#ifndef Process_h
#define Process_h

using namespace std;

class Process
{
    private:
        string id;
        //The timer is used to help tell when a process is finished executing
        int burstTime, arrivalTime, turnaroundTime, waitingTime, timer;
        bool complete;
    public:
        Process()
        {
            timer = burstTime;
            complete = false;
        }
        Process(string id, int burstTime, int arrivalTime)
        {
            this->id = id;
            this->burstTime = burstTime;
            this->arrivalTime = arrivalTime;
            timer = burstTime;
            complete = false;
        }
        const string getID() const {return id;}
        const int getBurstTime() const {return burstTime;}
        const int getArrivalTime() const {return arrivalTime;}
        const int getTurnaroundTime() const {return turnaroundTime;}
        const int getWaitingTime() const {return waitingTime;}
        const bool getStatus() const {return complete;}

        void setTurnaroundTime(int turnaroundTime) {this->turnaroundTime = turnaroundTime;}
        void setWaitingTime(int waitingTime) {this->waitingTime = waitingTime;}
        void setStatus(bool complete) {this->complete = complete;}

        void MinusTimer(int i) {timer -= i;}
        //void setTimer(int i) {timer = i;}
        const int getTimer() const {return timer;}

        void PrintProcessDetails() const;
        void PrintProcessStatistics() const;
};

bool operator<(const Process& p1, const Process& p2) {return !(p1.getTimer() < p2.getTimer());}

void Process::PrintProcessDetails() const
{
    cout << id << ' ' << burstTime << ' ' << arrivalTime << '\n';
}

void Process::PrintProcessStatistics() const
{
    cout << id << ' ' << turnaroundTime << ' ' << waitingTime << '\n';
}
#endif