#include "../include/matching_engine.hpp"

#include <thread>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include <atomic>

using namespace std;
using namespace std::chrono;

int main()
{
    MatchingEngine engine;

    const int THREADS = 10;
    const int ORDERS_PER_THREAD = 100000;
    const int TOTAL = THREADS * ORDERS_PER_THREAD;

    vector<thread> workers;

    atomic<int> buyCount{0};
    atomic<int> sellCount{0};

    auto start = high_resolution_clock::now();

    // Spawn producer threads
    for (int t = 0; t < THREADS; ++t)
    {
        workers.emplace_back([&, t]()
                             {
            // different seed every run (realistic randomness)
            mt19937 rng(
                high_resolution_clock::now().time_since_epoch().count() + t
            );

            uniform_int_distribution<> priceDist(95, 105);
            uniform_int_distribution<> sideDist(0, 1);

            for (int i = 0; i < ORDERS_PER_THREAD; ++i)
            {
                Side side = sideDist(rng) ? Side::Buy : Side::Sell;

                if (side == Side::Buy)
                    buyCount++;
                else
                    sellCount++;

                engine.submit({
                    static_cast<uint64_t>(t * ORDERS_PER_THREAD + i),
                    side,
                    priceDist(rng),
                    10,
                    static_cast<uint64_t>(
                        high_resolution_clock::now().time_since_epoch().count()
                    )
                });
            } });
    }

    // wait producers
    for (auto &th : workers)
        th.join();

    // wait matching thread to finish
    engine.stop();

    auto end = high_resolution_clock::now();

    // Stats
    auto ms = duration_cast<milliseconds>(end - start).count();

    int buys = buyCount.load();
    int sells = sellCount.load();
    int trades = engine.tradeCount();

    double throughput = (TOTAL * 1000.0) / ms;
    double fillRate = (100.0 * trades) / TOTAL;

    // Results
    cout << "\n===== STRESS TEST RESULTS =====\n";

    cout << "Total orders        : " << TOTAL << "\n";

    cout << "Buy orders          : " << buys
         << " (" << (100.0 * buys / TOTAL) << "%)\n";

    cout << "Sell orders         : " << sells
         << " (" << (100.0 * sells / TOTAL) << "%)\n";

    cout << "Trades executed     : " << trades << "\n";

    cout << "Fill rate           : " << fillRate << "%\n";

    cout << "Time taken          : " << ms << " ms\n";

    cout << "Throughput          : " << throughput << " orders/sec\n";

    cout << "BestBid             : " << engine.bestBid() << "\n";
    cout << "BestAsk             : " << engine.bestAsk() << "\n";
}
