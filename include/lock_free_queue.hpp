#pragma once
#include <atomic>
#include <cstddef>
using namespace std;
template <typename T, size_t SIZE>
class LockFreeQueue
{
private:
    T buffer[SIZE];

    atomic<size_t> head{0};
    atomic<size_t> tail{0};

public:
    bool push(const T &item)
    {
        size_t t = tail.load(memory_order_relaxed);
        size_t next = (t + 1) % SIZE;

        if (next == head.load(memory_order_acquire))
            return false; // full

        buffer[t] = item;

        tail.store(next, memory_order_release);
        return true;
    }

    bool pop(T &item)
    {
        size_t h = head.load(memory_order_relaxed);

        if (h == tail.load(memory_order_acquire))
            return false; // empty

        item = buffer[h];

        head.store((h + 1) % SIZE, memory_order_release);
        return true;
    }
};
