struct event {
    double time;
    enum { ARRIVAL, DEPARTURE } type;
    int customer_id;
    int teller_id;

    bool operator>(const event& other) const {
        return time > other.time;
    }
};

void bank_simulation(int num_tellers, double arrival_rate, double duration) {
    // Min-heap of events
    std::priority_queue<event, std::vector<event>, std::greater<event>> events;
    std::vector<int> teller_available_time(num_tellers, 0);

    // First arrival
    events.push({0.0, event::ARRIVAL, 0, -1});

    int customer_count = 0;
    double total_wait = 0.0;

    while (!events.empty()) {
        event e = events.top(); events.pop();
        if (e.time > duration) break;

        if (e.type == event::ARRIVAL) {
            ++customer_count;
            // Schedule next arrival
            double interarrival = -std::log(1.0 - std::rand() / RAND_MAX) / arrival_rate;
            events.push({e.time + interarrival, event::ARRIVAL, customer_count, -1});

            // Find free teller
            int best_teller = 0;
            for (int t = 1; t < num_tellers; ++t) {
                if (teller_available_time[t] < teller_available_time[best_teller])
                    best_teller = t;
            }

            double start = std::max(e.time, double(teller_available_time[best_teller]));
            double wait = start - e.time;
            total_wait += wait;

            double service_time = 1.0 + std::rand() / RAND_MAX * 4.0;  // 1-5 min
            teller_available_time[best_teller] = start + service_time;
            events.push({start + service_time, event::DEPARTURE, customer_count, best_teller});
        }
    }

    std::cout << "Customers served: " << customer_count << '\n'
              << "Average wait: " << total_wait / customer_count << '\n';
}
