#pragma once
#include "order_book.hpp"
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <map>
#include "lock_free_queue.hpp"
#include <atomic>

class MatchingEngine
{
private:
    OrderBook book;
    queue<Order> queue;
    mutex mtx;
    condition_variable cv;
    // LockFreeQueue<Order, 1 << 20> queue; // 1M capacity

    atomic<bool> running{true};
    thread worker;

    void process();

public:
    MatchingEngine();
    void submit(Order o);
    void stop();
    size_t tradeCount() const;

    // Helper functions
    int bestBid() const;
    int bestAsk() const;
    map<int, int> getBidLevels() const;
    map<int, int> getAskLevels() const;
};
