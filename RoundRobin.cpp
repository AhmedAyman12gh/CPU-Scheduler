#include <iostream>
#include "Process.hpp"
#include "RoundRobin.hpp"
#include <vector>
#include <queue>
#include <thread>   
#include <chrono>
#include <climits>
#include <algorithm>

using namespace std;


// add new process to the vector
void RoundRobin::addProcess(int id , int arrival , int burst ){
    Process new_process(id, arrival, burst);
    allProcesses.push_back(new_process);
}

// calculating wait time of each process then get the average
double RoundRobin::getWaitingTime(){
    double total_waiting_time = 0;
    for (Process &p : allProcesses) {
        p.waiting_time = p.completion_time - p.arrival_time - p.burst_time;
        total_waiting_time += p.waiting_time;
    }
    return total_waiting_time / allProcesses.size();
}

double RoundRobin::getTurnaroundTime(){
    double total_turnaround_time = 0;
    for (Process &p : allProcesses) {
        p.turnaround_time = p.completion_time - p.arrival_time;
        total_turnaround_time += p.turnaround_time;
    }
    return total_turnaround_time / allProcesses.size();
}





void RoundRobin::execute() {
    cout << "\n--- Starting Round Robin Scheduling ---\n\n";

    int completed_count = 0;
    int total_processes = allProcesses.size();
    vector<int> added_to_queue;
    Process* last_process = nullptr;

    auto addNewArrivals = [&]() {
        for (Process &p : allProcesses) {
            if (p.arrival_time <= current_time && p.remaining_time > 0) {
                bool already_added = false;
                for (int id : added_to_queue) {
                    if (id == p.id) {
                        already_added = true;
                        break;
                    }
                }
                if (!already_added) {
                    ready_queue.push(&p);
                    added_to_queue.push_back(p.id);
                    cout << "[Time " << current_time << "] Process " << p.id << " added to ready queue.\n";
                }
            }
        }
    };

    addNewArrivals();

    while (completed_count < total_processes) {

        //  FIXED ORDER: add new arrivals FIRST, then re-queue the preempted process
        addNewArrivals();

        if (last_process != nullptr && last_process->remaining_time > 0) {
            ready_queue.push(last_process);
        }
        last_process = nullptr;

        if (ready_queue.empty()) {
            int next_arrival = INT_MAX;
            for (Process &p : allProcesses) {
                if (p.remaining_time > 0 && p.arrival_time > current_time) {
                    next_arrival = min(next_arrival, p.arrival_time);
                }
            }
            if (next_arrival == INT_MAX) break;
            cout << "[Time " << current_time << "] CPU idle. Fast-forwarding to time " << next_arrival << "\n";
            current_time = next_arrival;
            continue;
        }

        current_process = ready_queue.front();
        ready_queue.pop();

        if (current_process->remaining_time <= 0) continue;

        cout << "\n[Time " << current_time << "] Process " << current_process->id << " gets the CPU.\n";

        int ticks_run = 0;
        while (ticks_run < time_quantum && current_process->remaining_time > 0) {
            current_process->remaining_time--;
            current_time++;
            ticks_run++;

            cout << "   -> Tick " << current_time
                 << " | Process " << current_process->id
                 << " remaining time: " << current_process->remaining_time << "\n";

            this_thread::sleep_for(chrono::milliseconds(100));

            if (current_process->remaining_time <= 0) {
                current_process->completion_time = current_time;
                cout << "[Time " << current_time << "] Process " << current_process->id << " FINISHED!\n";
                completed_count++;
                current_process = nullptr;
                break;
            }
        }

        if (current_process != nullptr) {
            cout << "[Time " << current_time << "] Process " << current_process->id << " paused. Moving to back of queue.\n";
            last_process = current_process;
        }
    }

    cout << "\n--- All Processes Completed ---\n";
}