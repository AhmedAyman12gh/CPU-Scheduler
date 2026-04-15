#include <iostream>
#include "Process.hpp"
#include "Priority.hpp"
#include <vector>
#include <queue>
#include <thread>   
#include <chrono>
#include <climits>
#include <algorithm>
using namespace std;

// add new process to the vector
void Priority::addProcess(int id , int burst_time, int arrival_time, int priority) {
    Process new_process(id, burst_time, arrival_time, priority);
    allProcesses.push_back(new_process);
}

// calculating wait time of each process then get the average
double Priority::getWaitingTime() {
    double total_waiting_time = 0;
    for (Process &p : allProcesses) {
        p.waiting_time = p.completion_time - p.arrival_time - p.burst_time;
        total_waiting_time += p.waiting_time;
    }
    return total_waiting_time / allProcesses.size();
}

// calculating turnaround time of each process then get the average
double Priority::getTurnaroundTime() {
    double total_turnaround_time = 0;
    for (Process &p : allProcesses) {
        p.turnaround_time = p.completion_time - p.arrival_time;
        total_turnaround_time += p.turnaround_time;
    }
    return total_turnaround_time / allProcesses.size();
}

// Implementation for priority scheduling algorithm

//   int current_time;
//   vector<Process> allProcesses;
//   queue<Process> process_queue;
//  Process * current_process;
//  bool is_live;
void Priority::execute( bool preemptive) {
    cout << "\n--- Starting Priority Scheduling (" << (preemptive ? "Preemptive" : "Non-Preemptive") << ") ---\n\n";

    int completed_count = 0;
    int total_processes = allProcesses.size();
    vector<int> added_to_queue;  // To track which processes have been added to the queue id only
    Process* last_process = nullptr;

    // Function to add new arriving processes to the ready queue
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
                    process_queue.push(p);
                    added_to_queue.push_back(p.id);
                    cout << "[Time " << current_time << "] Process " << p.id << " added to ready queue.\n";
                }
            }
        }
    }; 
   
    

    while (completed_count < total_processes) {
        if (!process_queue.empty()) {
            // Find the process with the highest priority (lowest priority number)
            Process highest_priority_process = process_queue.front();
            queue<Process> temp_queue;
            while (!process_queue.empty()) {
                Process p = process_queue.front();
                process_queue.pop();
                if (p.priority < highest_priority_process.priority) {
                    highest_priority_process = p;
                }
                temp_queue.push(p);
            }
            // Restore the processes back to the main queue
            while (!temp_queue.empty()) {
                process_queue.push(temp_queue.front());
                temp_queue.pop();
            }

            // Execute the highest priority process
            current_process = &highest_priority_process;
            is_live = true;

            int exec_time = preemptive ? 1 : current_process->remaining_time;
            cout << "[Time " << current_time << "] Executing Process " << current_process->id << " (Priority: " << current_process->priority << ")\n";
            this_thread::sleep_for(chrono::seconds(exec_time)); // Simulate execution time

            current_time += exec_time;
            current_process->remaining_time -= exec_time;

            if (current_process->remaining_time <= 0) {
                current_process->completion_time = current_time;
                completed_count++;
                cout << "[Time " << current_time << "] Process " << current_process->id << " completed.\n";
                is_live = false;
            }
        } else {
            // If no processes are ready, just advance time
            current_time++;
        }
        addNewArrivals();
    } 
    



                

}   
