#include "../include/order_book.hpp"
#include <climits>

PriceLevel &OrderBook::bidLevel(int price)
{
    return bids[price]; // create if not exist
}

PriceLevel &OrderBook::askLevel(int price)
{
    return asks[price];
}

bool OrderBook::hasBid(int price) const
{
    return bids.find(price) != bids.end();
}

bool OrderBook::hasAsk(int price) const
{
    return asks.find(price) != asks.end();
}

int OrderBook::totalBidQty(int price) const
{
    auto it = bids.find(price);
    return it == bids.end() ? 0 : it->second.totalQty();
}

int OrderBook::totalAskQty(int price) const
{
    auto it = asks.find(price);
    return it == asks.end() ? 0 : it->second.totalQty();
}

int OrderBook::bestBid() const
{
    int best = -1;

    for (const auto &[price, level] : bids)
        if (!level.empty() && price > best)
            best = price;

    return best;
}

int OrderBook::bestAsk() const
{
    int best = INT_MAX;

    for (const auto &[price, level] : asks)
        if (!level.empty() && price < best)
            best = price;

    return best == INT_MAX ? -1 : best;
}
