#pragma once
#include <iostream>

using namespace std;
class Process {
    public:
    int id;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int waiting_time;
    int turnaround_time;
    int priority;

    // if arrival_time is not provided, it defaults to 0 (non-preemptive scheduling)
    // if priority is not provided, it defaults to 0


    Process(int id,  int burst_time, int arrival_time = 0, int priority = 0) {
        this->id = id;
        this->arrival_time = arrival_time;
        this->burst_time = burst_time;
        this->remaining_time = burst_time;
        this->completion_time = 0;
        this->waiting_time = 0;
        this->turnaround_time = 0;
        this->priority = priority;
    }
 

};