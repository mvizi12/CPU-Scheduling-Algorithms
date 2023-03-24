#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include "Process.h"

using namespace std;

void FCFS(queue<Process> jobQueue)
{
    Process executingProcess; //The process that's currently executing
    queue<Process> readyQueue; //Ready queue for FCFS
    queue<Process> tempQueue; //Temporary queue for displaying final statistics
    int cpuRunTime = -1; //A clock for the system. in other words how long the system has been running
    int averageTT = 0, averageWT = 0, n; //Average turnaround time and waiting time

    cout << "FCFS Scheduling\n";
    while (true) //FCFS
    {
        cpuRunTime++; //Increment system time

        //Update system state based on events that occur
        //Loops until each process has executed
        while(true)
        {
            if (!jobQueue.empty() && jobQueue.front().getArrivalTime() == cpuRunTime) //Add processes to ready queue in the order they arrive
            {
                readyQueue.push(jobQueue.front());
                jobQueue.pop();
            }
            if (jobQueue.empty() || jobQueue.front().getArrivalTime() > cpuRunTime) {break;} //Stop looking for processes since they haven't arrived yet
        }
        if (cpuRunTime == 0 || executingProcess.getStatus()) //If at the beginning of the system or the previous process is complete
        {
            executingProcess = readyQueue.front(); //Process at front of ready queue will start executing
            readyQueue.pop(); //Remove executing process from ready queue
            cout << cpuRunTime << ' ' << executingProcess.getID() << " CPU allocated to process\n";
        }

        if (executingProcess.getTimer() == 0) {executingProcess.setStatus(true);} //If process has finished, mark it as complete

        if (executingProcess.getStatus()) //If the process is complete
        {
            cout << cpuRunTime << ' ' << executingProcess.getID() << " Process terminated\n";
            executingProcess.setTurnaroundTime(cpuRunTime - executingProcess.getArrivalTime()); //Calculate turnaround time
            executingProcess.setWaitingTime(executingProcess.getTurnaroundTime() - executingProcess.getBurstTime()); //Calculate waiting time
            tempQueue.push(executingProcess); //Add to temp queue for displaying final statistics later
            if (readyQueue.empty()) {break;} //If ready queue is empty we're done
        }
        executingProcess.MinusTimer(1);
    }
    cout << cpuRunTime << " Complete\n";

    n = tempQueue.size(); //Number of total processes
    while(!tempQueue.empty()) //Loop through temp queue and display final statistics
    {
        tempQueue.front().PrintProcessStatistics();
        averageTT += tempQueue.front().getTurnaroundTime();
        averageWT += tempQueue.front().getWaitingTime();
        tempQueue.pop();
    }
    averageTT /= n;
    averageWT /= n;
    cout << "Average Turnaround Time: " << averageTT << '\n';
    cout << "Average Waiting Time: " << averageWT << '\n';
}

void RR(queue<Process> jobQueue)
{
    Process executingProcess;
    queue<Process> readyQueue;
    queue<Process> tempQueue;
    int cpuRunTime = -1;
    int averageTT = 0, averageWT = 0, n;
    int quantum = 3, quantumTimer; //Quantum is how long a process can run before being stopped and added to the end of the ready queue

    cout << "RR Scheduling\n";
    while (true) //RR
    {
        cpuRunTime++; //Increment system time

        //Update system state based on events that occur
        //Loops until each process has executed
        while(true)
        {
            if (!jobQueue.empty() && jobQueue.front().getArrivalTime() == cpuRunTime)
            {
                readyQueue.push(jobQueue.front());
                jobQueue.pop();
            }
            if (jobQueue.empty() || jobQueue.front().getArrivalTime() > cpuRunTime) {break;}
        }
        if (cpuRunTime == 0 || executingProcess.getStatus())
        {
            executingProcess = readyQueue.front();
            readyQueue.pop();
            quantumTimer = quantum; //Start the quantum timer
            cout << cpuRunTime << ' ' << executingProcess.getID() << " CPU allocated to process\n";
        }

        if (executingProcess.getTimer() == 0) {executingProcess.setStatus(true);}
        else if (quantumTimer == 0) //Else if quantum expired, preempt the executing process and start the next process
        {
            cout << cpuRunTime << ' ' << executingProcess.getID() << " Quantum expired - " << executingProcess.getTimer() << "ms remaining\n";
            readyQueue.push(executingProcess); //Push executing process to end of queue
            executingProcess = readyQueue.front(); //Process at front of ready queue will start executing
            readyQueue.pop(); //Remove executing process from ready queue
            quantumTimer = quantum; //Restart the quantum timer
            continue;
        }

        if (executingProcess.getStatus())
        {
            cout << cpuRunTime << ' ' << executingProcess.getID() << " Process terminated\n";
            executingProcess.setTurnaroundTime(cpuRunTime - executingProcess.getArrivalTime());
            executingProcess.setWaitingTime(executingProcess.getTurnaroundTime() - executingProcess.getBurstTime());
            tempQueue.push(executingProcess);
            if (readyQueue.empty()) {break;}
        }
        else if (quantum > 0) //Just to make sure not counting down if the quantum timer has already expired
        {
            executingProcess.MinusTimer(1);
            quantumTimer -= 1;
        }
    }
    cout << cpuRunTime << " Complete\n";

    n = tempQueue.size(); //Number of total processes
    while(!tempQueue.empty()) //Loop through temp queue and display final statistics
    {
        tempQueue.front().PrintProcessStatistics();
        averageTT += tempQueue.front().getTurnaroundTime();
        averageWT += tempQueue.front().getWaitingTime();
        tempQueue.pop();
    }
    averageTT /= n;
    averageWT /= n;
    cout << "Average Turnaround Time: " << averageTT << '\n';
    cout << "Average Waiting Time: " << averageWT << '\n';
}
void SRTF(queue<Process> jobQueue)
{
    Process executingProcess;
    //A priority queue will order the processes based on their burst length
    //Processes with shorter burst lengths will be at the top of the queue
    //Every time a process ias added to the queue it's ordered
    priority_queue<Process> readyQueue;
    queue<Process> tempQueue;
    int cpuRunTime = -1;
    int averageTT = 0, averageWT = 0, n;
    bool started = false; //Flag to let me know if at least one process is executing or has executed

    cout << "SRTF Scheduling\n";
    while (true) //SRTF
    {
        cpuRunTime++; //Increment system time

        //Update system state based on events that occur
        //Loops until each process has executed
        while(true)
        {
            if (!jobQueue.empty() && jobQueue.front().getArrivalTime() == cpuRunTime) //Add processes to ready queue in the order they arrive
            {
                readyQueue.push(jobQueue.front());
                jobQueue.pop();
                //I think this if statement might be unnecessary since I'm using a priority queue for the ready queue
                if (started && (executingProcess.getTimer() > readyQueue.top().getTimer())) //If the executing process has more burst time left than the next shortest process, preempt it
                {
                    cout << cpuRunTime << ' ' << executingProcess.getID() << " Process preempted by process with shorter burst time - " << executingProcess.getTimer() << " ms remaining\n";
                    readyQueue.push(executingProcess);
                    executingProcess = readyQueue.top();
                    readyQueue.pop();
                    cout << cpuRunTime << ' ' << executingProcess.getID() << " CPU allocated to process\n";
                }
            }
            if (jobQueue.empty() || jobQueue.front().getArrivalTime() > cpuRunTime) {break;} //Stop looking for processes since they haven't arrived yet
        }
        if (cpuRunTime == 0 || executingProcess.getStatus())
        {
            executingProcess = readyQueue.top();
            started = true;
            readyQueue.pop();
            cout << cpuRunTime << ' ' << executingProcess.getID() << " CPU allocated to process\n";
        }

        if (executingProcess.getTimer() == 0) {executingProcess.setStatus(true);} //If process has finished, mark it as complete

        if (executingProcess.getStatus()) //If the process is complete
        {
            cout << cpuRunTime << ' ' << executingProcess.getID() << " Process terminated\n";
            executingProcess.setTurnaroundTime(cpuRunTime - executingProcess.getArrivalTime()); //Calculate turnaround time
            executingProcess.setWaitingTime(executingProcess.getTurnaroundTime() - executingProcess.getBurstTime()); //Calculate waiting time
            tempQueue.push(executingProcess); //Add to temp queue for displaying final statistics later
            if (readyQueue.empty()) {break;} //If ready queue is empty we're done
        }
        executingProcess.MinusTimer(1);
    }
    cout << cpuRunTime << " Complete\n";

    n = tempQueue.size(); //Number of total processes
    while(!tempQueue.empty()) //Loop through temp queue and display final statistics
    {
        tempQueue.front().PrintProcessStatistics();
        averageTT += tempQueue.front().getTurnaroundTime();
        averageWT += tempQueue.front().getWaitingTime();
        tempQueue.pop();
    }
    averageTT /= n;
    averageWT /= n;
    cout << "Average Turnaround Time: " << averageTT << '\n';
    cout << "Average Waiting Time: " << averageWT << '\n';
}

int main()
{
    queue<Process> jobqueue; //A queue of all processes in the system now and the future
    string id, burstTime, arrivalTime; //Variables to hold txt from file
    int iBurstTime, iArrivalTime; //Integer versions of the above variables
    ifstream fin; //Creates a filestream variable for reading

    fin.open("Prog3inputfileS23.txt"); //Opens specified file

    /*Goes through the file, creates processes from its contents, and adds them to the job queue*/
    while(!fin.eof()) //Loops through each line in the file
    {
        getline(fin, id, ' ');
        getline(fin, burstTime, ' ');
        getline(fin, arrivalTime, '\n');

        if (id.empty()) {break;} //Accounts for potential extra whitespace at end of file
        stringstream geek(burstTime); //Converts "bursTime" to an integer and stores it in "geek"
        geek >> iBurstTime; //Places the value in "geek" in "iBurstTime"
        stringstream geek2(arrivalTime);
        geek2 >> iArrivalTime;
        Process process(id, iBurstTime, iArrivalTime); //Creates a process with the attributes from the file
        jobqueue.push(process); //Pushes the process onto the job queue
    }

    FCFS(jobqueue);
    cout << endl;
    RR(jobqueue);
    cout << endl;
    SRTF(jobqueue);
    cout << endl;

    fin.close();
    return 0;
}