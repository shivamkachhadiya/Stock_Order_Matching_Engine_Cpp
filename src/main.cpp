#include "../include/matching_engine.hpp"
#include <iostream>

int main()
{
    MatchingEngine engine;

    engine.addLimit({1, Side::Buy, 100, 10, 0});
    engine.addLimit({2, Side::Buy, 100, 5, 0});
    engine.addLimit({3, Side::Sell, 105, 7, 0});
    engine.addLimit({4, Side::Sell, 100, 4, 0});

    std::cout << engine.totalBidQty(100) << "\n";
    std::cout << engine.totalAskQty(105) << "\n";

    std::cout << "BestBid: " << engine.bestBid() << "\n";
    std::cout << "BestAsk: " << engine.bestAsk() << "\n";
}
