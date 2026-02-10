#pragma once
#include <map>
#include <unordered_map>
#include <vector>
#include "Order.hpp"
#include "price_level.hpp"
#include "trade.hpp"
using namespace std;
class OrderBook
{
private:
    map<int, PriceLevel> bids; // key = price
    map<int, PriceLevel> asks;
    vector<Trade> trades;
    unordered_map<uint64_t, pair<Side, int>> orderIndex;

public:
    void addLimit(Order order);
    void addMarket(Order order);
    bool cancel(uint64_t orderId);

    int totalBidQty(int price) const;
    int totalAskQty(int price) const;

    int bestBid() const;
    int bestAsk() const;

    const vector<Trade> &getTrades() const;
    void printBook() const;

    size_t tradeCount() const;
};
