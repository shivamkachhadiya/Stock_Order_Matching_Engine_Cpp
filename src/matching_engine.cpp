#include "../include/matching_engine.hpp"
#include <thread>
using namespace std;
MatchingEngine::MatchingEngine()
{
    worker = thread(&MatchingEngine::process, this);
}

// =====================================================
// LOCK-FREE SUBMIT (no mutex, no blocking)
// =====================================================
void MatchingEngine::submit(Order o)
{
    {
        lock_guard<mutex> lock(mtx);
        queue.push(o);
    }
    cv.notify_one();
}

// =====================================================
// MATCHING THREAD
// =====================================================
void MatchingEngine::process()
{
    while (running)
    {
        unique_lock<mutex> lock(mtx);

        cv.wait(lock, [&]
                { return !queue.empty() || !running; });

        while (!queue.empty())
        {
            Order o = queue.front();
            queue.pop();
            lock.unlock();

            book.addLimit(o);

            lock.lock();
        }
    }
}

// =====================================================
// STOP ENGINE
// =====================================================
void MatchingEngine::stop()
{
    running = false;
    cv.notify_all();

    if (worker.joinable())
        worker.join();
}

// =====================================================
// HELPERS
// =====================================================
int MatchingEngine::bestBid() const
{
    return book.bestBid();
}

int MatchingEngine::bestAsk() const
{
    return book.bestAsk();
}

map<int, int> MatchingEngine::getBidLevels() const
{
    map<int, int> levels;

    for (int price = book.bestBid(); price >= 0; --price)
    {
        int qty = book.totalBidQty(price);
        if (qty > 0)
            levels[price] = qty;
        else
            break;
    }

    return levels;
}

map<int, int> MatchingEngine::getAskLevels() const
{
    map<int, int> levels;

    for (int price = book.bestAsk(); price <= 1e6; ++price)
    {
        int qty = book.totalAskQty(price);
        if (qty > 0)
            levels[price] = qty;
        else
            break;
    }

    return levels;
}

size_t MatchingEngine::tradeCount() const
{
    return book.tradeCount();
}
