#include "../include/order_book.hpp"
#include <algorithm>
#include <climits>

using namespace std;

// ======================================================
// LIMIT ORDER
// ======================================================
void OrderBook::addLimit(Order order)
{
    // BUY
    if (order.side == Side::Buy)
    {
        auto it = asks.find(order.price);

        // match same price sells
        if (it != asks.end())
        {
            auto &level = it->second;

            while (!level.empty() && order.qty > 0)
            {
                Order &sell = level.front();

                int traded = min(order.qty, sell.qty);

                order.qty -= traded;
                sell.qty -= traded;

                if (sell.qty == 0)
                    level.pop();
            }
        }

        if (order.qty > 0)
            bids[order.price].add(order);
    }

    // SELL
    else
    {
        auto it = bids.find(order.price);

        if (it != bids.end())
        {
            auto &level = it->second;

            while (!level.empty() && order.qty > 0)
            {
                Order &buy = level.front();

                int traded = min(order.qty, buy.qty);

                order.qty -= traded;
                buy.qty -= traded;

                if (buy.qty == 0)
                    level.pop();
            }
        }

        if (order.qty > 0)
            asks[order.price].add(order);
    }
}

// ======================================================
// MARKET ORDER
// ======================================================
void OrderBook::addMarket(Order order)
{
    if (order.side == Side::Buy)
    {
        while (order.qty > 0)
        {
            int price = bestAsk();
            if (price == -1)
                break;

            auto &level = asks[price];

            while (!level.empty() && order.qty > 0)
            {
                Order &sell = level.front();

                int traded = min(order.qty, sell.qty);

                order.qty -= traded;
                sell.qty -= traded;

                if (sell.qty == 0)
                    level.pop();
            }
        }
    }
    else
    {
        while (order.qty > 0)
        {
            int price = bestBid();
            if (price == -1)
                break;

            auto &level = bids[price];

            while (!level.empty() && order.qty > 0)
            {
                Order &buy = level.front();

                int traded = min(order.qty, buy.qty);

                order.qty -= traded;
                buy.qty -= traded;

                if (buy.qty == 0)
                    level.pop();
            }
        }
    }
}

// ======================================================
// HELPERS
// ======================================================
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
