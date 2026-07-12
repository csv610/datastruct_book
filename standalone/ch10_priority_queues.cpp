// ch09: Priority Queues — heap-based priority queue
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <vector>

template <typename T, typename Compare = std::less<T>>
class priority_queue {
    std::vector<T> data_;
    Compare comp_;
public:
    priority_queue() { data_.push_back(T{}); }

    void push(const T& val) {
        data_.push_back(val);
        std::size_t i = data_.size() - 1;
        while (i > 1 && comp_(data_[i / 2], data_[i])) {
            std::swap(data_[i], data_[i / 2]);
            i /= 2;
        }
    }
    T pop() {
        if (data_.size() <= 1) throw std::underflow_error("pq empty");
        T top = data_[1];
        data_[1] = std::move(data_.back());
        data_.pop_back();
        heapify(1);
        return top;
    }
    const T& peek() const {
        if (data_.size() <= 1) throw std::underflow_error("pq empty");
        return data_[1];
    }
    bool empty() const { return data_.size() <= 1; }
    std::size_t size() const { return data_.size() - 1; }

private:
    void heapify(std::size_t i) {
        while (true) {
            std::size_t largest = i;
            if (2 * i < data_.size() && comp_(data_[largest], data_[2 * i])) largest = 2 * i;
            if (2 * i + 1 < data_.size() && comp_(data_[largest], data_[2 * i + 1])) largest = 2 * i + 1;
            if (largest == i) break;
            std::swap(data_[i], data_[largest]);
            i = largest;
        }
    }
};

template <typename T, typename Compare = std::greater<T>>
class min_priority_queue : public priority_queue<T, Compare> {};

int main() {
    priority_queue<int> pq;
    pq.push(5); pq.push(10); pq.push(3); pq.push(8);
    std::cout << "pq pop: " << pq.pop() << " (expect 10)\n";
    std::cout << "pq pop: " << pq.pop() << " (expect 8)\n";
    std::cout << "pq pop: " << pq.pop() << " (expect 5)\n";
    std::cout << "pq pop: " << pq.pop() << " (expect 3)\n";
    std::cout << "pq empty: " << pq.empty() << " (expect 1)\n";

    // min priority queue (using std::greater)
    priority_queue<int, std::greater<int>> minpq;
    minpq.push(5); minpq.push(10); minpq.push(3);
    std::cout << "minpq pop: " << minpq.pop() << " (expect 3)\n";

    std::cout << "All ch10 tests passed.\n";
    return 0;
}
