#pragma once
#include <iostream>
#include "Process.hpp"
#include <vector>
#include <queue>
using namespace std;

class RoundRobin{
    public:
    int time_quantum;
    int current_time;
    vector<Process> allProcesses;
    queue<Process*> ready_queue;
    Process * current_process;
    bool is_live;


    RoundRobin(int time_quantum, vector<Process> allProcesses) {
        this->time_quantum = time_quantum;
        this->current_time = 0;
        this->allProcesses = allProcesses;
        this->current_process = nullptr;
        this->is_live = true;
    }
    void addProcess(int id ,  int arrival , int busrt);
    void execute();
    double getWaitingTime();
    double getTurnaroundTime();
};