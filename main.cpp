#include <iostream>
#include "Process.hpp"
#include "Priority.hpp"
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
    processes.push_back(Process(1, 3, 0, 1)); // id, burst_time, arrival_time, priority
    processes.push_back(Process(2, 2, 0, 3));
    processes.push_back(Process(3, 5, 0, 0));
 



    Priority p(processes);
    p.execute(false); // Non-preemptive priority scheduling


    cout << "\nAverage Waiting Time: " << p.getWaitingTime() << " ms\n";
    cout << "Average Turnaround Time: " << p.getTurnaroundTime() << " ms\n";

    return 0;
}