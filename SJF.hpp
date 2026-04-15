#pragma once
#include <vector>
#include "Process.hpp"
using namespace std;

void sjf_non_preemptive(vector<Process>& processes);
void sjf_preemptive(vector<Process>& processes);