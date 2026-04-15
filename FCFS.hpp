#pragma once

#include <vector>
#include "Process.hpp"

using namespace std;

// First Come First Serve (non-preemptive)
// Uses the shared Process model from Process.hpp.
void fcfs(vector<Process>& processes);