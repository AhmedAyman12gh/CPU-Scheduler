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
    Process(int id, int arrival_time, int burst_time) {
        this->id = id;
        this->arrival_time = arrival_time;
        this->burst_time = burst_time;
        this->remaining_time = burst_time;
        this->completion_time = 0;
        this->waiting_time = 0;
        this->turnaround_time = 0;
    }
};