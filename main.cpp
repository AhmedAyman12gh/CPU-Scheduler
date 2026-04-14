#include <iostream>
#include "Process.hpp"
#include "RoundRobin.hpp"
#include <queue>
#include <vector>
using namespace std;

int main(){
    vector<Process> processes;
    processes.push_back(Process(1, 0, 5));
    processes.push_back(Process(2, 0, 3));
    processes.push_back(Process(3, 0, 8));
    processes.push_back(Process(4, 1, 6));
    processes.push_back(Process(5, 0, 4));
    processes.push_back(Process(6, 0, 7));  
    processes.push_back(Process(7, 0, 2));
    processes.push_back(Process(8, 2, 9));


    int time_quantum = 2;
    RoundRobin rr(time_quantum, processes);
    rr.execute();

    cout << "\nAverage Waiting Time: " << rr.getWaitingTime() << " ms\n";
    cout << "Average Turnaround Time: " << rr.getTurnaroundTime() << " ms\n";

    return 0;
}