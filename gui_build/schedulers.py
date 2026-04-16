class Process:
    def __init__(self, pid, arrival_time, burst_time, priority=0):
        self.pid = pid
        self.arrival_time = arrival_time
        self.burst_time = burst_time
        self.remaining_time = burst_time
        self.priority = priority
        self.completion_time = 0
        self.waiting_time = 0
        self.turnaround_time = 0
        self.has_started = False

class SchedulerEngine:
    def __init__(self, algo_type, quantum=1):
        self.algo_type = algo_type
        self.quantum = quantum
        self.processes = []
        self.current_time = 0
        self.ready_queue = []
        self.current_process = None
        self.completed_processes = []
        self.rr_ticks = 0

    def add_process(self, pid, arrival, burst, priority=0):
        self.processes.append(Process(pid, arrival, burst, priority))

    def is_finished(self):
        return len(self.completed_processes) == len(self.processes)

    def tick(self):
        # Add arriving processes to ready queue
        for p in self.processes:
            if p.arrival_time == self.current_time and p.remaining_time > 0:
                if p not in self.ready_queue and p != self.current_process:
                    self.ready_queue.append(p)

        # Handle Preemption and Algorithm Logic
        if self.algo_type == "FCFS":
            self._fcfs_logic()
        elif self.algo_type == "SJF (Non-Preemptive)":
            self._sjf_non_preemptive_logic()
        elif self.algo_type == "SJF (Preemptive)":
            self._sjf_preemptive_logic()
        elif self.algo_type == "Priority (Non-Preemptive)":
            self._priority_non_preemptive_logic()
        elif self.algo_type == "Priority (Preemptive)":
            self._priority_preemptive_logic()
        elif self.algo_type == "Round Robin":
            self._rr_logic()

        # Execute current process
        executing_pid = None
        if self.current_process:
            executing_pid = self.current_process.pid
            self.current_process.remaining_time -= 1
            if self.algo_type == "Round Robin":
                self.rr_ticks += 1

            if self.current_process.remaining_time == 0:
                self.current_process.completion_time = self.current_time + 1
                self.current_process.turnaround_time = self.current_process.completion_time - self.current_process.arrival_time
                self.current_process.waiting_time = self.current_process.turnaround_time - self.current_process.burst_time
                self.completed_processes.append(self.current_process)
                self.current_process = None
                self.rr_ticks = 0

        self.current_time += 1
        return executing_pid

    def _fcfs_logic(self):
        if not self.current_process and self.ready_queue:
            self.ready_queue.sort(key=lambda x: x.arrival_time)
            self.current_process = self.ready_queue.pop(0)

    def _sjf_non_preemptive_logic(self):
        if not self.current_process and self.ready_queue:
            self.ready_queue.sort(key=lambda x: (x.burst_time, x.arrival_time))
            self.current_process = self.ready_queue.pop(0)

    def _sjf_preemptive_logic(self):
        if self.ready_queue:
            self.ready_queue.sort(key=lambda x: (x.remaining_time, x.arrival_time))
            if not self.current_process:
                self.current_process = self.ready_queue.pop(0)
            elif self.ready_queue[0].remaining_time < self.current_process.remaining_time:
                self.ready_queue.append(self.current_process)
                self.current_process = self.ready_queue.pop(0)

    def _priority_non_preemptive_logic(self):
        if not self.current_process and self.ready_queue:
            # Smaller number = higher priority [cite: 6]
            self.ready_queue.sort(key=lambda x: (x.priority, x.arrival_time))
            self.current_process = self.ready_queue.pop(0)

    def _priority_preemptive_logic(self):
        if self.ready_queue:
            self.ready_queue.sort(key=lambda x: (x.priority, x.arrival_time))
            if not self.current_process:
                self.current_process = self.ready_queue.pop(0)
            elif self.ready_queue[0].priority < self.current_process.priority:
                self.ready_queue.append(self.current_process)
                self.current_process = self.ready_queue.pop(0)

    def _rr_logic(self):
        if self.current_process and self.rr_ticks == self.quantum:
            self.ready_queue.append(self.current_process)
            self.current_process = None
            self.rr_ticks = 0
            
        if not self.current_process and self.ready_queue:
            self.current_process = self.ready_queue.pop(0)