#include <iostream>
#include "Process.hpp"
#include "RoundRobin.hpp"
#include <vector>
#include <queue>
#include <thread>   
#include <chrono>
#include <climits>
#include <algorithm>
#include <iomanip> 
using namespace std;

struct GanttBlock {
    int process_id;
    int start_time;
    int end_time;
};


// add new process to the vector
void RoundRobin::addProcess(int id , int burst, int arrival) {
    Process new_process(id, burst, arrival);
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
    vector<GanttBlock> gantt_chart;
    int block_start_time = 0;
    int current_block_pid = -1;

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

        if (current_block_pid != current_process->id) {
            if (current_block_pid != -1) {
                gantt_chart.push_back({current_block_pid, block_start_time, current_time});
            }
            current_block_pid = current_process->id;
            block_start_time = current_time;
        }

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
                if (current_block_pid == current_process->id) {
                    gantt_chart.push_back({current_block_pid, block_start_time, current_time});
                    current_block_pid = -1;
                }
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

    // --- UPDATED ALIGNED GANTT CHART LOGIC ---
if (gantt_chart.empty()) return;

    cout << "\n========== GANTT CHART ==========\n";

    // 1. Top Border
    cout << " ";
    for (const auto& block : gantt_chart) {
        int id_width = (block.process_id >= 10) ? 2 : 1;
        // " Px " takes 3 + id_width characters. 
        cout << string(id_width + 3, '-') << " "; 
    }
    cout << "\n|";

    // 2. Process ID Row
    for (const auto& block : gantt_chart) {
        cout << " P" << block.process_id << " |";
    }
    cout << "\n ";

    // 3. Bottom Border
    for (const auto& block : gantt_chart) {
        int id_width = (block.process_id >= 10) ? 2 : 1;
        cout << string(id_width + 3, '-') << " ";
    }
    cout << "\n";

    // 4. Timeline (Timestamps)
    // Print the starting time of the very first block
    string last_time_str = to_string(gantt_chart[0].start_time);
    cout << last_time_str;

    for (const auto& block : gantt_chart) {
        int id_width = (block.process_id >= 10) ? 2 : 1;
        // The exact character width of " Px |" is id_width + 4
        int block_width = id_width + 4; 
        
        // Calculate exact spaces needed to reach the next '|'
        // by subtracting the length of the number we just printed
        int spaces = block_width - last_time_str.length();
        if (spaces < 1) spaces = 1; // Safety fallback
        
        cout << string(spaces, ' ') << block.end_time;
        
        // Update the last printed string so the next loop knows how much to offset
        last_time_str = to_string(block.end_time); 
    }
    cout << "\n\n================================\n";
}