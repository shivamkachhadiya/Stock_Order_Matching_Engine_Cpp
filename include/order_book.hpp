#pragma once

#include <unordered_map>
#include "price_level.hpp"

class OrderBook
{
private:
    std::unordered_map<int, PriceLevel> bids;
    std::unordered_map<int, PriceLevel> asks;

public:
    // ======================
    // STORAGE ONLY
    // ======================

    PriceLevel &bidLevel(int price);
    PriceLevel &askLevel(int price);

    bool hasBid(int price) const;
    bool hasAsk(int price) const;

    int totalBidQty(int price) const;
    int totalAskQty(int price) const;

    int bestBid() const;
    int bestAsk() const;
};
