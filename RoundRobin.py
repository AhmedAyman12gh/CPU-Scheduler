import math
from collections import deque

class Process:
    def __init__(self, pid, burst, arrival):
        self.id = pid
        self.burst_time = burst
        self.arrival_time = arrival
        self.remaining_time = burst
        self.completion_time = 0
        self.waiting_time = 0
        self.turnaround_time = 0

class GanttBlock:
    def __init__(self, process_id, start_time, end_time):
        self.process_id = process_id
        self.start_time = start_time
        self.end_time = end_time

class RoundRobin:
    def __init__(self, time_quantum):
        self.time_quantum = time_quantum
        self.all_processes = []
        self.ready_queue = deque()
        self.current_time = 0
        self.current_process = None

    # add new process to the list
    def add_process(self, pid, burst, arrival):
        new_process = Process(pid, burst, arrival)
        self.all_processes.append(new_process)

    # calculating wait time of each process then get the average
    def get_waiting_time(self):
        if not self.all_processes:
            return 0.0
        total_waiting_time = 0
        for p in self.all_processes:
            p.waiting_time = p.completion_time - p.arrival_time - p.burst_time
            total_waiting_time += p.waiting_time
        return total_waiting_time / len(self.all_processes)

    def get_turnaround_time(self):
        if not self.all_processes:
            return 0.0
        total_turnaround_time = 0
        for p in self.all_processes:
            p.turnaround_time = p.completion_time - p.arrival_time
            total_turnaround_time += p.turnaround_time
        return total_turnaround_time / len(self.all_processes)




    # --- NON-LIVE EXECUTION (Standard fast simulation) ---
    def execute(self):
        print("\n--- Starting Round Robin Scheduling ---\n")

        completed_count = 0
        total_processes = len(self.all_processes)
        added_to_queue = set() # using a set instead of a list for faster O(1) lookups
        last_process = None
        
        gantt_chart = []
        block_start_time = 0
        current_block_pid = -1

        def add_new_arrivals():
            for p in self.all_processes:
                if p.arrival_time <= self.current_time and p.remaining_time > 0:
                    if p.id not in added_to_queue:
                        self.ready_queue.append(p)
                        added_to_queue.add(p.id)
                        print(f"[Time {self.current_time}] Process {p.id} added to ready queue.")

        add_new_arrivals()

        while completed_count < total_processes:
            
            # FIXED ORDER: add new arrivals FIRST, then re-queue the preempted process
            add_new_arrivals()

            if last_process is not None and last_process.remaining_time > 0:
                self.ready_queue.append(last_process)
            last_process = None

            if not self.ready_queue:
                next_arrival = float('inf')
                for p in self.all_processes:
                    if p.remaining_time > 0 and p.arrival_time > self.current_time:
                        next_arrival = min(next_arrival, p.arrival_time)
                
                if next_arrival == float('inf'):
                    break
                
                print(f"[Time {self.current_time}] CPU idle. Fast-forwarding to time {next_arrival}")
                self.current_time = next_arrival
                continue

            self.current_process = self.ready_queue.popleft()

            if self.current_process.remaining_time <= 0:
                continue

            print(f"\n[Time {self.current_time}] Process {self.current_process.id} gets the CPU.")

            if current_block_pid != self.current_process.id:
                if current_block_pid != -1:
                    gantt_chart.append(GanttBlock(current_block_pid, block_start_time, self.current_time))
                current_block_pid = self.current_process.id
                block_start_time = self.current_time

            ticks_run = 0
            while ticks_run < self.time_quantum and self.current_process.remaining_time > 0:
                self.current_process.remaining_time -= 1
                self.current_time += 1
                ticks_run += 1

                print(f"   -> Tick {self.current_time} | Process {self.current_process.id} remaining time: {self.current_process.remaining_time}")

                if self.current_process.remaining_time <= 0:
                    self.current_process.completion_time = self.current_time
                    print(f"[Time {self.current_time}] Process {self.current_process.id} FINISHED!")
                    
                    if current_block_pid == self.current_process.id:
                        gantt_chart.append(GanttBlock(current_block_pid, block_start_time, self.current_time))
                        current_block_pid = -1
                    
                    completed_count += 1
                    self.current_process = None
                    break

            if self.current_process is not None:
                print(f"[Time {self.current_time}] Process {self.current_process.id} paused. Moving to back of queue.")
                last_process = self.current_process

        print("\n--- All Processes Completed ---")

        # --- PERFECTLY ALIGNED GANTT CHART LOGIC ---
        if not gantt_chart:
            return

        print("\n========== GANTT CHART ==========")

        # 1. Top Border
        print(" ", end="")
        for block in gantt_chart:
            id_width = 2 if block.process_id >= 10 else 1
            print("-" * (id_width + 3) + " ", end="")
        print("\n|", end="")

        # 2. Process ID Row
        for block in gantt_chart:
            print(f" P{block.process_id} |", end="")
        print("\n ", end="")

        # 3. Bottom Border
        for block in gantt_chart:
            id_width = 2 if block.process_id >= 10 else 1
            print("-" * (id_width + 3) + " ", end="")
        print()

        # 4. Timeline (Timestamps)
        last_time_str = str(gantt_chart[0].start_time)
        print(last_time_str, end="")

        for block in gantt_chart:
            id_width = 2 if block.process_id >= 10 else 1
            block_width = id_width + 4
            
            spaces = block_width - len(last_time_str)
            if spaces < 1:
                spaces = 1
                
            print(" " * spaces + str(block.end_time), end="")
            last_time_str = str(block.end_time)
            
        print("\n\n================================\n")

# test
if __name__ == "__main__":
    rr = RoundRobin(time_quantum=3)
    rr.add_process(1, 10, 0)
    rr.add_process(2, 4, 0)
    rr.add_process(3, 5, 0)
    rr.execute()