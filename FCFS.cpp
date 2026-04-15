#include "FCFS.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

void fcfs(vector<Process>& processes)
{
    if (processes.empty()) {
        cout << "\n=== FCFS ===\n";
        cout << "No processes to schedule.\n";
        return;
    }

    // Stable sort so equal-arrival processes keep their original input order.
    stable_sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrival_time < b.arrival_time;
    });

    int current_time = 0;
    vector<pair<int, int>> gantt; // {pid, start_time}

    for (auto& p : processes) {
        // Handle CPU idle time until the next process arrives.
        if (current_time < p.arrival_time) {
            current_time = p.arrival_time;
        }

        gantt.push_back({p.id, current_time});

        current_time += p.burst_time;
        p.remaining_time = 0;
        p.completion_time = current_time;
        p.turnaround_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
    }

    double total_wt = 0.0;
    double total_tat = 0.0;
    for (const auto& p : processes) {
        total_wt += p.waiting_time;
        total_tat += p.turnaround_time;
    }

    cout << "\n=== FCFS (Non-Preemptive) ===\n";
    cout << "PID\tAT\tBT\tCT\tTAT\tWT\n";
    for (const auto& p : processes) {
        cout << p.id << "\t"
             << p.arrival_time << "\t"
             << p.burst_time << "\t"
             << p.completion_time << "\t"
             << p.turnaround_time << "\t"
             << p.waiting_time << "\n";
    }

    cout << fixed << setprecision(2);
    cout << "Average Waiting Time : " << total_wt / processes.size() << "\n";
    cout << "Average Turnaround Time : " << total_tat / processes.size() << "\n";

    cout << "Gantt Chart : ";
    for (size_t i = 0; i < processes.size(); ++i) {
        cout << gantt[i].second << " [P" << gantt[i].first << "] ";
    }
    cout << current_time << "\n";
}