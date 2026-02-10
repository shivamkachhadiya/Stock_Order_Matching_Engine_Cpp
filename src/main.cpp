#include "../include/order_book.hpp"
#include "../include/matching_engine.hpp"

#include <iostream>
#include <chrono>
#include <iomanip>
#include <random>
#include <thread>
#include <vector>

using namespace std;
using namespace chrono;

int main()
{
    MatchingEngine engine;

    engine.submit({1, Side::Buy, 100, 10, 0});
    engine.submit({2, Side::Sell, 100, 5, 0});
    engine.submit({3, Side::Buy, 101, 15, 0});

    this_thread::sleep_for(chrono::milliseconds(100));

    engine.stop();

    cout << "BestBid: " << engine.bestBid() << "\n";
    cout << "BestAsk: " << engine.bestAsk() << "\n";

    return 0;
}