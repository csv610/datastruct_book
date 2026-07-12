#include <iostream>

int main() {
    interval_tree scheduler;

    // Insert meetings: [start, end] in hours
    scheduler.insert({9,  10});
    scheduler.insert({10, 12});
    scheduler.insert({11, 13});
    scheduler.insert({14, 16});
    scheduler.insert({15, 17});
    scheduler.insert({19, 20});

    // Query: find all meetings overlapping [11, 15]
    auto conflicts = scheduler.search({11, 15});

    for (const auto& m : conflicts) {
        std::cout << "Overlaps: ["
                  << m.low << ", " << m.high << "]\n";
    }
    // Output:
    //   Overlaps: [10, 12]
    //   Overlaps: [14, 16]
    //   Overlaps: [11, 13]
    //   Overlaps: [15, 17]
}
