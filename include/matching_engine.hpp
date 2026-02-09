#pragma once
#include "order_book.hpp"

class MatchingEngine
{
private:
    OrderBook book;

public:
    void addLimit(Order order);
    void addMarket(Order order);

    int bestBid() const { return book.bestBid(); }
    int bestAsk() const { return book.bestAsk(); }

    int totalBidQty(int price) const { return book.totalBidQty(price); }
    int totalAskQty(int price) const { return book.totalAskQty(price); }
};
