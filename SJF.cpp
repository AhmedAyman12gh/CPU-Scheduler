#include "SJF.hpp"
#include <iostream>
#include <climits>
using namespace std;

// ─── Non-Preemptive SJF ───────────────────────────────────────────────────────
void sjf_non_preemptive(vector<Process>& processes) {
    int current_time = 0;
    int completed = 0;
    int n = processes.size();
    vector<bool> completed_flags(n, false); 
    vector<pair<int,int>> gantt;

    while (completed < n) {
        int shortest = -1;
        int min_burst = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (!completed_flags[i] &&
                processes[i].arrival_time <= current_time &&
                processes[i].burst_time < min_burst) {
                min_burst = processes[i].burst_time;
                shortest = i;
            }
        }

        if (shortest == -1) {
            current_time++;
            continue;
        }

        Process& p = processes[shortest];
        for (int t = 0; t < p.burst_time; t++) {
            gantt.push_back({p.id, current_time + t});
        }

        current_time      += p.burst_time;
        p.completion_time  = current_time;
        p.turnaround_time  = p.completion_time - p.arrival_time;
        p.waiting_time     = p.turnaround_time - p.burst_time;
        p.remaining_time   = 0;
        completed_flags[shortest] = true;   // mark done locally
        completed++;
    }

    // ── Stats ──
    double total_wt = 0, total_tat = 0;
    for (auto& p : processes) {
        total_wt  += p.waiting_time;
        total_tat += p.turnaround_time;
    }

    cout << "\n=== SJF Non-Preemptive ===\n";
    cout << "PID\tAT\tBT\tCT\tTAT\tWT\n";
    for (auto& p : processes) {
        cout << p.id             << "\t"
             << p.arrival_time   << "\t"
             << p.burst_time     << "\t"
             << p.completion_time << "\t"
             << p.turnaround_time << "\t"
             << p.waiting_time    << "\n";
    }
    cout << "Average Waiting Time    : " << total_wt  / n << "\n";
    cout << "Average Turnaround Time : " << total_tat / n << "\n";
}



void sjf_preemptive(vector<Process>& processes) {
    int current_time = 0;
    int completed = 0;
    int n = processes.size();
    vector<bool> completed_flags(n, false);  
    vector<pair<int,int>> gantt;

    while (completed < n) {
        int shortest = -1;
        int min_remaining = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (!completed_flags[i] &&
                processes[i].arrival_time <= current_time &&
                processes[i].remaining_time < min_remaining) {
                min_remaining = processes[i].remaining_time;
                shortest = i;
            }
        }

        if (shortest == -1) {
            current_time++;
            continue;
        }

        gantt.push_back({processes[shortest].id, current_time});
        processes[shortest].remaining_time--;
        current_time++;

        if (processes[shortest].remaining_time == 0) {
            Process& p        = processes[shortest];
            p.completion_time = current_time;
            p.turnaround_time = p.completion_time - p.arrival_time;
            p.waiting_time    = p.turnaround_time - p.burst_time;
            completed_flags[shortest] = true;   // mark done locally
            completed++;
        }
    }

    
    double total_wt = 0, total_tat = 0;
    for (auto& p : processes) {
        total_wt  += p.waiting_time;
        total_tat += p.turnaround_time;
    }

    cout << "\n=== SJF Preemptive (SRTF) ===\n";
    cout << "PID\tAT\tBT\tCT\tTAT\tWT\n";
    for (auto& p : processes) {
        cout << p.id             << "\t"
             << p.arrival_time   << "\t"
             << p.burst_time     << "\t"
             << p.completion_time << "\t"
             << p.turnaround_time << "\t"
             << p.waiting_time    << "\n";
    }
    cout << "Average Waiting Time    : " << total_wt  / n << "\n";
    cout << "Average Turnaround Time : " << total_tat / n << "\n";
}