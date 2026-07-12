struct task {
    std::string name;
    int priority;       // higher = more urgent
    int deadline;       // time units
};

// Max-heap ordered by priority.
struct task_cmp {
    bool operator()(const task& a, const task& b) {
        return a.priority < b.priority;
    }
};

void schedule_tasks() {
    std::priority_queue<task, std::vector<task>,
                        task_cmp> scheduler;

    scheduler.push({"backup",      2, 100});
    scheduler.push({"critical_db", 5, 10});
    scheduler.push({"log_rotate",  1, 50});
    scheduler.push({"user_request",4, 5});

    while (!scheduler.empty()) {
        task t = scheduler.top();
        scheduler.pop();
        std::cout << "Running: " << t.name
                  << " (priority=" << t.priority
                  << ")\n";
    }
    // Output: critical_db, user_request, backup, log_rotate
}
