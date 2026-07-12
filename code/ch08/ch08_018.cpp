#include <vector>
#include <optional>
#include <cstdint>

class RobinHoodTable {
private:
    struct Slot {
        uint64_t key = 0;
        int      home = 0;    // ideal bucket index
        bool     occupied = false;
    };

    int                        capacity;
    int                        count = 0;
    std::vector<Slot>          slots;

    int probe_distance(int home, int pos) const {
        int d = pos - home;
        return (d >= 0) ? d : d + capacity;
    }

    int find_slot(uint64_t key) const {
        int home = key % capacity;
        for (int i = 0; i < capacity; i++) {
            int idx = (home + i) % capacity;
            if (!slots[idx].occupied) return idx;
            if (slots[idx].key == key) return idx;
            if (probe_distance(slots[idx].home, idx) < i)
                return -1;  // key cannot exist past this point
        }
        return -1;
    }

public:
    explicit RobinHoodTable(int cap) : capacity(cap), slots(cap) {}

    void insert(uint64_t key) {
        int home = key % capacity;
        uint64_t cur_key = key;
        int cur_home = home;

        for (int i = 0; i < capacity; i++) {
            int idx = (home + i) % capacity;
            if (!slots[idx].occupied) {
                slots[idx] = {cur_key, cur_home, true};
                count++;
                return;
            }
            if (slots[idx].key == cur_key) return; // already present

            // Robin Hood: if new element is "poorer," swap
            int existing_dist = probe_distance(slots[idx].home, idx);
            int new_dist = i;
            if (new_dist > existing_dist) {
                std::swap(cur_key, slots[idx].key);
                std::swap(cur_home, slots[idx].home);
                home = cur_home;  // continue probing for displaced element
                i = existing_dist; // resume from where displaced element was
            }
        }
    }

    std::optional<uint64_t> find(uint64_t key) const {
        int idx = find_slot(key);
        if (idx >= 0 && slots[idx].occupied && slots[idx].key == key)
            return slots[idx].key;
        return std::nullopt;
    }

    void erase(uint64_t key) {
        int idx = find_slot(key);
        if (idx < 0 || !slots[idx].occupied) return;
        slots[idx].occupied = false;
        count--;

        // Shift subsequent elements backward to fill the gap
        for (int i = 1; i < capacity; i++) {
            int next = (idx + i) % capacity;
            if (!slots[next].occupied) break;
            int ideal = slots[next].home;
            int dist = probe_distance(ideal, next);
            if (dist == 0) break;  // element is at home, nothing to shift
            slots[idx] = slots[next];
            slots[idx].occupied = true;
            slots[next].occupied = false;
            idx = next;
        }
    }

    int size() const { return count; }
};

// Usage
#include <iostream>
int main() {
    RobinHoodTable ht(101);
    ht.insert(42);
    ht.insert(99);
    ht.insert(150);
    auto r = ht.find(42);
    if (r) std::cout << "Found: " << *r << "\n";
    ht.erase(42);
}
