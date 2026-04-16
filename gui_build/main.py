import tkinter as tk
from tkinter import ttk, messagebox
from schedulers import SchedulerEngine

class CPUSchedulerGUI:
    def __init__(self, root):
        self.root = root
        self.root.title("CPU Scheduler Simulator")
        self.root.geometry("900x700")
        
        self.engine = None
        self.is_live = False
        self.colors = ["#FF9999", "#99CCFF", "#99FF99", "#FFCC99", "#CC99FF", "#FFFF99", "#FF99CC"]
        self.process_colors = {}
        
        self.setup_ui()

    def setup_ui(self):
        # --- Top Control Panel ---
        control_frame = ttk.LabelFrame(self.root, text="Configuration")
        control_frame.pack(fill="x", padx=10, pady=5)

        ttk.Label(control_frame, text="Algorithm:").grid(row=0, column=0, padx=5, pady=5)
        self.algo_var = tk.StringVar(value="FCFS")
        self.algo_dropdown = ttk.Combobox(control_frame, textvariable=self.algo_var, state="readonly", width=25)
        self.algo_dropdown['values'] = ("FCFS", "SJF (Non-Preemptive)", "SJF (Preemptive)", 
                                        "Priority (Non-Preemptive)", "Priority (Preemptive)", "Round Robin")
        self.algo_dropdown.grid(row=0, column=1, padx=5, pady=5)
        self.algo_dropdown.bind("<<ComboboxSelected>>", self.toggle_inputs)

        self.lbl_quantum = ttk.Label(control_frame, text="Time Quantum:")
        self.entry_quantum = ttk.Entry(control_frame, width=10)
        
        self.lbl_priority = ttk.Label(control_frame, text="Priority:")
        self.entry_priority = ttk.Entry(control_frame, width=10)

        # --- Process Entry Panel ---
        entry_frame = ttk.LabelFrame(self.root, text="Add Process")
        entry_frame.pack(fill="x", padx=10, pady=5)

        ttk.Label(entry_frame, text="PID:").grid(row=0, column=0, padx=5)
        self.entry_pid = ttk.Entry(entry_frame, width=10)
        self.entry_pid.grid(row=0, column=1, padx=5)

        ttk.Label(entry_frame, text="Arrival Time:").grid(row=0, column=2, padx=5)
        self.entry_at = ttk.Entry(entry_frame, width=10)
        self.entry_at.grid(row=0, column=3, padx=5)

        ttk.Label(entry_frame, text="Burst Time:").grid(row=0, column=4, padx=5)
        self.entry_bt = ttk.Entry(entry_frame, width=10)
        self.entry_bt.grid(row=0, column=5, padx=5)

        self.btn_add = ttk.Button(entry_frame, text="Add Process", command=self.add_process_ui)
        self.btn_add.grid(row=0, column=8, padx=10)

        self.btn_reset = ttk.Button(entry_frame, text="Reset All", command=self.reset_all)
        self.btn_reset.grid(row=0, column=9, padx=10)

        # --- Simulation Controls ---
        sim_frame = ttk.Frame(self.root)
        sim_frame.pack(fill="x", padx=10, pady=5)
        
        self.live_var = tk.BooleanVar(value=True)
        ttk.Checkbutton(sim_frame, text="Live Scheduling (1s per unit)", variable=self.live_var).pack(side="left", padx=5)
        
        ttk.Button(sim_frame, text="Start Simulation", command=self.start_simulation).pack(side="left", padx=5)

        # --- Remaining Burst Time Table ---
        table_frame = ttk.LabelFrame(self.root, text="Live Process Table (Remaining Burst Time)")
        table_frame.pack(fill="both", expand=True, padx=10, pady=5)
        
        columns = ("PID", "Arrival", "Burst", "Priority", "Remaining", "Status")
        self.tree = ttk.Treeview(table_frame, columns=columns, show="headings", height=6)
        for col in columns:
            self.tree.heading(col, text=col)
            self.tree.column(col, width=100, anchor="center")
        self.tree.pack(fill="both", expand=True, padx=5, pady=5)

        # --- Gantt Chart ---
        gantt_frame = ttk.LabelFrame(self.root, text="Live Gantt Chart")
        gantt_frame.pack(fill="x", padx=10, pady=5)
        self.canvas = tk.Canvas(gantt_frame, height=100, bg="white")
        self.canvas.pack(fill="x", padx=5, pady=5)

        # --- Stats ---
        stats_frame = ttk.Frame(self.root)
        stats_frame.pack(fill="x", padx=10, pady=5)
        self.lbl_stats = ttk.Label(stats_frame, text="Average WT: 0.00  |  Average TAT: 0.00", font=("Arial", 12, "bold"))
        self.lbl_stats.pack()

    def toggle_inputs(self, event=None):
        algo = self.algo_var.get()
        # Hide unused inputs 
        self.lbl_quantum.grid_forget()
        self.entry_quantum.grid_forget()
        self.lbl_priority.grid_forget()
        self.entry_priority.grid_forget()

        if algo == "Round Robin":
            self.lbl_quantum.grid(row=0, column=2, padx=5, pady=5)
            self.entry_quantum.grid(row=0, column=3, padx=5, pady=5)
        elif "Priority" in algo:
            self.lbl_priority.grid(row=0, column=6, padx=5)
            self.entry_priority.grid(row=0, column=7, padx=5)

    def add_process_ui(self):
        pid = self.entry_pid.get()
        at = self.entry_at.get()
        bt = self.entry_bt.get()
        pr = self.entry_priority.get() if "Priority" in self.algo_var.get() else "0"
        
        if not (pid and at and bt):
            messagebox.showerror("Error", "Please fill PID, Arrival, and Burst times.")
            return

        try:
            priority_val = int(pr) if pr else 0
            if self.engine:  # Add dynamically while running [cite: 10]
                self.engine.add_process(pid, int(at), int(bt), priority_val)
            
            # Add to treeview
            self.tree.insert("", "end", values=(pid, at, bt, priority_val, bt, "Ready/Waiting"))
            
            # Clear entries
            self.entry_pid.delete(0, 'end')
            self.entry_at.delete(0, 'end')
            self.entry_bt.delete(0, 'end')
            self.entry_priority.delete(0, 'end')
            
        except ValueError:
            messagebox.showerror("Error", "Numerical values required for times.")

    def reset_all(self):
        for item in self.tree.get_children():
            self.tree.delete(item)
        self.process_colors.clear()
        self.engine = None
        self.is_live = False
        self.canvas.delete("all")
        self.lbl_stats.config(text="Average WT: 0.00  |  Average TAT: 0.00")

    def start_simulation(self):
        items = self.tree.get_children()
        if not items:
            messagebox.showerror("Error", "Add at least one process.")
            return

        algo = self.algo_var.get()
        quantum = int(self.entry_quantum.get()) if algo == "Round Robin" and self.entry_quantum.get() else 1
        
        self.engine = SchedulerEngine(algo, quantum)
        self.process_colors.clear()
        
        for idx, item in enumerate(items):
            vals = self.tree.item(item, 'values')
            self.engine.add_process(vals[0], int(vals[1]), int(vals[2]), int(vals[3]))
            self.process_colors[vals[0]] = self.colors[idx % len(self.colors)]

        self.canvas.delete("all")
        self.is_live = self.live_var.get()
        
        if self.is_live:
            self.run_tick() # Live mode [cite: 13, 14]
        else:
            self.run_all_fast() # Fast mode [cite: 14]

    def update_table(self):
        for item in self.tree.get_children():
            self.tree.delete(item)
            
        for p in self.engine.processes:
            status = "Completed" if p.remaining_time == 0 else "Running/Ready"
            self.tree.insert("", "end", values=(p.pid, p.arrival_time, p.burst_time, p.priority, p.remaining_time, status))

    def calculate_stats(self):
        n = len(self.engine.processes)
        if n == 0: return
        total_wt = sum(p.waiting_time for p in self.engine.processes)
        total_tat = sum(p.turnaround_time for p in self.engine.processes)
        self.lbl_stats.config(text=f"Average WT: {total_wt/n:.2f}  |  Average TAT: {total_tat/n:.2f}")

    def draw_gantt_block(self, start_time, pid):
        if pid is None: return
        x1 = start_time * 30
        x2 = (start_time + 1) * 30
        color = self.process_colors.get(pid, "#DDDDDD")
        
        self.canvas.create_rectangle(x1, 20, x2, 80, fill=color, outline="black")
        self.canvas.create_text((x1 + x2) / 2, 50, text=f"P{pid}", font=("Arial", 10, "bold"))
        self.canvas.create_text(x1, 90, text=str(start_time), anchor="n")
        self.canvas.create_text(x2, 90, text=str(start_time + 1), anchor="n")

    def run_tick(self):
        if self.engine.is_finished():
            self.calculate_stats() 
            messagebox.showinfo("Done", "Simulation Complete!")
            return

        current_time = self.engine.current_time
        executing_pid = self.engine.tick()

        self.update_table() 
        if executing_pid:
            self.draw_gantt_block(current_time, executing_pid)

        # Schedule next tick mapping 1 unit to 1000ms (1 second) [cite: 13]
        self.root.after(1000, self.run_tick)

    def run_all_fast(self):
        while not self.engine.is_finished():
            current_time = self.engine.current_time
            executing_pid = self.engine.tick()
            if executing_pid:
                self.draw_gantt_block(current_time, executing_pid)
        
        self.update_table()
        self.calculate_stats()

if __name__ == "__main__":
    root = tk.Tk()
    app = CPUSchedulerGUI(root)
    root.mainloop()