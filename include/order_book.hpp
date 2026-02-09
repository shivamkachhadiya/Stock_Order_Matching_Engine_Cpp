#pragma once
#include <unordered_map>
#include "price_level.hpp"

class OrderBook
{
private:
    std::unordered_map<int, PriceLevel> bids;
    std::unordered_map<int, PriceLevel> asks;

public:
    void addLimit(Order order);
    void addMarket(Order order);

    int totalBidQty(int price) const;
    int totalAskQty(int price) const;

    int bestBid() const;
    int bestAsk() const;
};
