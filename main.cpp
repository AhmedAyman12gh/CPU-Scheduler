#include <iostream>
#include "Process.hpp"
#include "RoundRobin.hpp"
#include <queue>
#include <vector>
using namespace std;

int main(){
    // process (id,  burst_time) non-preemptive scheduling
    // or process (id,  burst_time, 0) non-preemptive scheduling
    // if arrival wanted process(id,  burst_time, arrival_time) preemptive scheduling
    // if priority wanted process(id,  burst_time, 0, priority) non-preemptive priority scheduling
    // if priority wanted process(id,  burst_time, arrival_time, priority) preemptive priority scheduling

    vector<Process> processes;
    processes.push_back(Process(1, 5));
    processes.push_back(Process(2, 3));
    processes.push_back(Process(3, 8));
    processes.push_back(Process(4, 1));
    processes.push_back(Process(5, 4));
    processes.push_back(Process(6, 7));  
    processes.push_back(Process(7, 2));
    processes.push_back(Process(8, 9));


    int time_quantum = 2;
    RoundRobin rr(time_quantum, processes);
    rr.execute();

    cout << "\nAverage Waiting Time: " << rr.getWaitingTime() << " ms\n";
    cout << "Average Turnaround Time: " << rr.getTurnaroundTime() << " ms\n";

    return 0;
}