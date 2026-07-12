# Queues

## Introduction

> **Complete compilable implementations of the data structures in this chapter are in `code/`.**

## The Queue ADT

A **queue** follows FIFO (First In, First Out): insertions at the **rear**, deletions from the **front**.

### Operations

- **push(value)** — insert at rear
- **pop()** — remove and return the front element
- **front()** — return the front element
- **back()** — return the rear element
- **empty()**, **size()**

### Applications

- Task scheduling (print queues, thread pools)
- Breadth-first search (Chapter 12)
- Buffering (I/O, streaming)
- Message passing (queues in concurrent systems)

## Array-Based Queue (Circular Buffer)

A naive array queue would be inefficient: after many pushes and pops, front moves forward and space at the beginning is wasted. A **circular buffer** solves this by wrapping around.

```
Initial: [_, _, _, _]   front=0, rear=0
push 10: [10, _, _, _]  front=0, rear=1
push 20: [10, 20, _, _] front=0, rear=2
pop:     [_, 20, _, _]  front=1, rear=2   (returns 10)
push 30: [_, 20, 30, _] front=1, rear=3
push 40: [_, 20, 30, 40] front=1, rear=0  (wraps around!)
```

```cpp
template <typename T>
class circular_queue {
public:
    explicit circular_queue(size_t initial_capacity = 8)
        : data_(std::make_unique<T[]>(initial_capacity))
        , capacity_(initial_capacity), front_(0), size_(0) {}

    void push(const T& value) {
        if (size_ == capacity_) grow();
        size_t pos = (front_ + size_) % capacity_;
        data_[pos] = value;
        ++size_;
    }

    void push(T&& value) {
        if (size_ == capacity_) grow();
        size_t pos = (front_ + size_) % capacity_;
        data_[pos] = std::move(value);
        ++size_;
    }

    void pop() {
        if (empty()) throw std::underflow_error("queue is empty");
        front_ = (front_ + 1) % capacity_;
        --size_;
    }

    T& front() {
        if (empty()) throw std::underflow_error("queue is empty");
        return data_[front_];
    }
    const T& front() const {
        if (empty()) throw std::underflow_error("queue is empty");
        return data_[front_];
    }

    T& back() {
        if (empty()) throw std::underflow_error("queue is empty");
        return data_[(front_ + size_ - 1) % capacity_];
    }

    bool empty() const noexcept { return size_ == 0; }
    size_t size() const noexcept { return size_; }

private:
    void grow() {
        size_t new_capacity = capacity_ * 2;
        auto new_data = std::make_unique<T[]>(new_capacity);
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[(front_ + i) % capacity_]);
        }
        front_ = 0;
        capacity_ = new_capacity;
        data_ = std::move(new_data);
    }

    std::unique_ptr<T[]> data_;
    size_t capacity_, front_, size_;
};
```

All operations: O(1) amortized.

## Linked Queue

```cpp
template <typename T>
class linked_queue {
public:
    linked_queue() = default;

    void push(const T& value) {
        auto new_node = std::make_unique<node>(value);
        if (rear_) {
            rear_->next = std::move(new_node);
            rear_ = rear_->next.get();
        } else {
            front_ = std::move(new_node);
            rear_ = front_.get();
        }
        ++size_;
    }

    void pop() {
        if (empty()) throw std::underflow_error("queue is empty");
        front_ = std::move(front_->next);
        if (!front_) rear_ = nullptr;
        --size_;
    }

    T& front() {
        if (empty()) throw std::underflow_error("queue is empty");
        return front_->data;
    }

    bool empty() const noexcept { return size_ == 0; }
    size_t size() const noexcept { return size_; }

private:
    struct node {
        T data;
        std::unique_ptr<node> next;
        explicit node(const T& d) : data(d) {}
    };
    std::unique_ptr<node> front_;
    node* rear_ = nullptr;
    size_t size_ = 0;
};
```

## Applications

### Message Queue (Apache Kafka / AWS SQS)

Message queues decouple producers from consumers. In a simplified Kafka-like publish-subscribe system, a queue holds messages that multiple consumers can process:

```cpp
template <typename T>
class message_queue {
public:
    void publish(const T& message) {
        std::scoped_lock lock(mutex_);
        queue_.push(message);
        cv_.notify_one();
    }

    T consume(std::chrono::milliseconds timeout = std::chrono::seconds(30)) {
        std::unique_lock lock(mutex_);
        if (!cv_.wait_for(lock, timeout, [this] { return !queue_.empty(); })) {
            throw std::runtime_error("consume timeout — no messages available");
        }
        T msg = queue_.front();
        queue_.pop();
        return msg;
    }

    size_t size() const {
        std::scoped_lock lock(mutex_);
        return queue_.size();
    }

private:
    linked_queue<T> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cv_;
};
```

**Usage in production:** LinkedIn uses Apache Kafka to handle over 7 trillion messages per day. Each topic partition is an ordered, immutable queue. Twitter's event bus processes 500 million tweets/day through message queues. The queue abstraction provides backpressure: if consumers fall behind, the queue grows (or triggers auto-scaling).

### Web Crawler Frontier (BFS)

A web crawler maintains a frontier of URLs to visit. BFS (queue-based) ensures politeness — it visits pages in discovery order, typically used by search engines like Googlebot:

```cpp
class web_crawler {
public:
    web_crawler(std::string_view seed_url, size_t max_pages = 10000)
        : frontier_(), visited_(), max_pages_(max_pages) {
        frontier_.push(seed_url);
    }

    void crawl() {
        while (!frontier_.empty() && visited_.size() < max_pages_) {
            std::string url = frontier_.front(); frontier_.pop();
            if (visited_.count(url)) continue;
            visited_.insert(url);

            std::cout << "Crawling: " << url << " ("
                      << visited_.size() << "/" << max_pages_ << ")\n";

            auto links = fetch_links(url);  // HTTP GET + HTML parsing
            for (const auto& link : links) {
                if (!visited_.count(link) && same_domain(url, link)) {
                    frontier_.push(link);
                }
            }
        }
    }

    size_t pages_crawled() const { return visited_.size(); }

private:
    std::vector<std::string> fetch_links(std::string_view url) {
        // Simulate fetching — in production this uses libcurl or CPPREST
        return {};
    }

    bool same_domain(std::string_view a, std::string_view b) {
        auto pos_a = a.find("://"), pos_b = b.find("://");
        if (pos_a == std::string_view::npos) pos_a = 0;
        else pos_a += 3;
        if (pos_b == std::string_view::npos) pos_b = 0;
        else pos_b += 3;
        std::string_view domain_a = a.substr(pos_a, a.find('/', pos_a) - pos_a);
        std::string_view domain_b = b.substr(pos_b, b.find('/', pos_b) - pos_b);
        return domain_a == domain_b;
    }

    linked_queue<std::string> frontier_;
    std::unordered_set<std::string> visited_;
    size_t max_pages_;
};
```

**Politeness:** Production crawlers (Googlebot, Bingbot) add a delay between requests to the same domain — equivalent to rate-limited dequeue. The queue also respects `robots.txt`, which filters URLs before enqueuing.

### Thread Pool Task Queue

Modern web servers (NGINX, Apache, uWSGI) use a thread pool with a shared task queue. Worker threads consume tasks from the queue as they become available:

```cpp
class thread_pool {
public:
    explicit thread_pool(size_t num_threads = std::thread::hardware_concurrency()) {
        workers_.reserve(num_threads);
        for (size_t i = 0; i < num_threads; ++i) {
            workers_.emplace_back([this] { worker_loop(); });
        }
    }

    ~thread_pool() {
        {
            std::scoped_lock lock(mutex_);
            done_ = true;
        }
        cv_.notify_all();
        for (auto& t : workers_) t.join();
    }

    void enqueue(std::function<void()> task) {
        std::scoped_lock lock(mutex_);
        tasks_.push(std::move(task));
        cv_.notify_one();
    }

private:
    void worker_loop() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock lock(mutex_);
                cv_.wait(lock, [this] { return done_ || !tasks_.empty(); });
                if (done_ && tasks_.empty()) return;
                task = tasks_.front(); tasks_.pop();
            }
            task();  // execute outside the lock
        }
    }

    linked_queue<std::function<void()>> tasks_;
    std::vector<std::thread> workers_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool done_ = false;
};
```

**Usage:** NGINX uses this pattern for its thread pool module (ngx_thread_pool), processing hundreds of thousands of concurrent connections. uWSGI's async mode runs a thread pool with a queue of requests, each handled by a greenlet.

## Deque (Double-Ended Queue)

A deque allows push/pop at both ends. C++ provides `std::deque` — typically implemented as a sequence of fixed-size blocks.

```cpp
std::deque<int> dq;
dq.push_back(10);     // [10]
dq.push_front(5);     // [5, 10]
dq.push_back(20);     // [5, 10, 20]
dq.pop_front();       // [10, 20]
dq.pop_back();        // [10]
```

`std::deque` offers O(1) operations at both ends and O(1) random access. It is the default container for `std::stack` and `std::queue`.

## STL Connection: std::queue

```cpp
std::queue<int> q;                          // uses std::deque
std::queue<int, std::list<int>> ql;       // list-based

q.push(10);
q.push(20);
int x = q.front();  // 10
q.pop();            // removes 10
```

## Common Bugs and Pitfalls

- **Confusing front() and back()** — in FIFO, `front()` returns the oldest element (first to be removed) while `back()` returns the most recently added. The naming is reversed from a stack perspective and causes bugs in production.
- **Circular buffer index arithmetic** — `(front_ + size_) % capacity_` is correct for insertion, but `(front_ + 1) % capacity_` for pop is only correct when size_ > 0. Off-by-one errors here cause silent memory corruption.
- **Queue in concurrent code without synchronization** — our examples use `std::mutex` and `std::condition_variable`. A bare `std::queue` shared across threads is a data race. Always synchronize or use `tbb::concurrent_queue`.
- **Message queue backpressure** — if producers outpace consumers, the queue grows unbounded, consuming all memory. Production systems (Kafka, RabbitMQ) enforce limits: max queue size, TTL, or drop policies.
- **Linked queue memory fragmentation** — each enqueue allocates a node on the heap. For high-throughput systems, this causes cache misses and allocator contention. A circular buffer avoids per-element allocation.

## Summary

1. A **queue** is a FIFO structure — insertions at the rear, deletions from the front.
2. **Circular buffers** implement array-based queues with O(1) amortized operations.
3. **Linked queues** use a singly linked list with front and rear pointers.
4. A **deque** generalizes push/pop to both ends with O(1) operations.
5. Industrial applications include message brokers (Kafka, SQS), web crawling (Googlebot), and thread pools (NGINX, uWSGI).
6. `std::queue` and `std::deque` provide production-ready implementations.

## Exercises

### Drill

1. Trace the state of a circular-buffer queue (capacity = 4) after each operation:
   `enqueue(K), en(V), deq(), en(Q), en(W), en(E), deq(), en(R), en(T), deq()`
   Show the front and rear pointers after each step.

2. In a web server thread pool with 8 workers and a queue of 100 pending requests, what happens when:
   a) A 9th worker is requested but none is free?
   b) The queue grows to 10,000 requests?
   c) A worker crashes while holding a task?

3. A Kafka topic partition has 3 consumers in the same consumer group. Messages arrive as: [A, B, C, D, E]. Show which consumer receives each message. What happens if consumer 2 crashes after receiving message B?

4. What is the worst-case time complexity of:
   a) Circular-buffer enqueue/dequeue
   b) Linked-list enqueue/dequeue
   c) Deque push_front/push_back
   How does each perform under cache misses?

5. A message queue has a maximum size of 1000. Producers enqueue 100 msg/s. Consumers process at 80 msg/s. After how many seconds does the queue fill up? What strategies can prevent this?

### Application

6. **Kafka-like partitioned message queue.** Implement a `partitioned_queue` that distributes messages across N partitions using round-robin or key-based hashing. Each partition is an independent queue. Support `publish(key, message)` and `consume(partition_id)`. Simulate 3 consumers, 4 partitions, and 10,000 messages. Measure the load balance across consumers.

7. **Sliding window rate limiter (deque).** Cloudflare's rate limiting uses a sliding window counter. Implement a rate limiter using a deque of timestamps. Each request adds the current time; requests older than 1 second are popped from the front. The window size is the deque length. Test with a burst of 100 requests in 100ms — how many pass if the limit is 10 req/s?

8. **Web crawler politeness queue.** Extend the BFS web crawler to respect `robots.txt` crawl delays. Each domain has a "next allowed crawl time." Maintain a priority queue of (next_crawl_time, url). Only dequeue URLs whose next_crawl_time has passed. Test on a list of 1000 URLs across 50 domains. Show that no domain is crawled more than once per 5 seconds.

9. **Thread pool with work stealing.** Implement a thread pool where each worker has its own task deque (instead of a shared queue). When a worker's deque is empty, it steals from another worker's deque (from the opposite end — the back). Compare throughput with the shared-queue approach for 10⁶ fine-grained tasks. Explain why work stealing outperforms on cache-bound workloads.

10. **NGINX-style event queue.** NGINX processes events through a queue: accept connections, read requests, write responses. Implement an event-driven server using a single-threaded event loop with a queue of (fd, event_type) pairs. Support EPOLLIN and EPOLLOUT. Benchmark against a thread-per-connection model for 10,000 concurrent connections.

11. **Consumer group rebalancing.** In Kafka, when a consumer joins or leaves, the group rebalances — partitions are reassigned. Implement a simple rebalancer: given N partitions and M consumers, assign partitions to consumers using range or sticky strategy. Simulate adding/removing consumers and show the reassignment. Measure the "movement" (fraction of partitions that change owners).

### Research

12. **(Blockmap deque)** `std::deque` uses a sequence of fixed-size blocks. What is the advantage over a single circular buffer? Implement a simplified blockmap deque with block size 64. Measure random-access performance vs `std::deque` and `std::vector`. When does the deque outperform vector for random access?

13. **(Chase-Lev work-stealing deque)** The Chase-Lev deque is the standard for work-stealing thread pools. Implement it with atomic operations. Compare its throughput with a mutex-guarded deque on 4, 8, and 16 threads. Why is the Chase-Lev design faster for the "owner" thread?

14. **(Disruptor)** Read about the LMAX Disruptor — a lock-free inter-thread queue that achieves 100M ops/sec. Implement a simplified single-producer, single-consumer ring buffer without locks. Compare its throughput with a mutex-guarded `std::queue` and a `tbb::concurrent_queue`. What design choices make the Disruptor faster?

## References and Further Reading

- Donald E. Knuth, *The Art of Computer Programming*, Volume 1: Fundamental Algorithms. Section 2.2.2.
- Thomas H. Cormen et al., *Introduction to Algorithms*, 4th Edition. Sections 10.1, 22.2.
- Robert Sedgewick and Kevin Wayne, *Algorithms*, 4th Edition. Sections 1.3, 4.1.
- David Chase and Yossi Lev, "Dynamic Circular Work-Stealing Deque" — SPAA 2005.
