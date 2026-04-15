# CPU Scheduler - Agent Instructions

## Build & Run
```bash
g++ -o main.exe main.cpp RoundRobin.cpp SJF.cpp Process.hpp RoundRobin.hpp SJF.hpp
./main.exe
```

## Project Structure
- `main.cpp` - Entry point; controls which algorithm runs
- `RoundRobin.cpp` - Round Robin scheduling with Gantt chart output
- `SJF.cpp` - Shortest Job First (preemptive/non-preemptive)
- `Process.hpp` - Process data structure
- `RoundRobin.hpp`, `SJF.hpp` - Algorithm headers

## Running Specific Algorithms
In `main.cpp`, comment/uncomment to switch:
```cpp
RoundRobin rr(time_quantum, processes);
rr.execute();
// sjf_non_preemptive(processes);
// sjf_preemptive(processes);
```

## Round Robin execute() Output
The `execute()` function prints:
1. Process arrival notifications
2. Tick-by-tick execution with remaining time
3. Process completion messages  
4. Gantt chart at end showing all execution blocks with timestamps