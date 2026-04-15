#ifndef PRIORITY_HPP
#define PRIORITY_HPP
#include <iostream>
#include "Process.hpp"
#include <vector>
#include <queue>
using namespace std;

struct Compare {
    bool operator()(Process* a, Process* b) {
        return a->priority > b->priority;
    }
};
class Priority{
    public:
    int current_time;
    vector<Process> allProcesses;
    priority_queue<Process*, vector<Process*>, Compare> process_queue;
    Process * current_process;
    bool is_live;
    bool is_preemptive;
    Priority( vector<Process> allProcesses){
        this->allProcesses=allProcesses;
        this->current_process=nullptr;
        this->is_preemptive=false;
        this->current_time=0;
        this->is_live=false;
    };

    void addProcess(int id , int burst_time, int arrival_time = 0, int priority = 0);
    void execute(bool preemptive);
    double getWaitingTime();
    double getTurnaroundTime();
};

#endif
