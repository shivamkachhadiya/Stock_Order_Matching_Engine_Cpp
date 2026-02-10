#include "../include/order_book.hpp"
#include <algorithm>
#include <iostream>
#include <climits>
#include <deque>

using namespace std;

// Add Limit Order
void OrderBook::addLimit(Order order)
{
    if (order.side == Side::Buy)
    {
        auto it = asks.find(order.price);
        if (it != asks.end())
        {
            auto &level = it->second;
            while (!level.empty() && order.qty > 0)
            {
                Order &sell = level.front();
                int traded = min(order.qty, sell.qty);
                trades.push_back({order.id, sell.id, order.price, traded});
                order.qty -= traded;
                sell.qty -= traded;
                if (sell.qty == 0)
                    level.pop();
            }
        }
        if (order.qty > 0)
        {
            bids[order.price].add(order);
            orderIndex[order.id] = {Side::Buy, order.price};
        }
    }
    else
    { // Sell
        auto it = bids.find(order.price);
        if (it != bids.end())
        {
            auto &level = it->second;
            while (!level.empty() && order.qty > 0)
            {
                Order &buy = level.front();
                int traded = min(order.qty, buy.qty);
                trades.push_back({buy.id, order.id, order.price, traded});
                order.qty -= traded;
                buy.qty -= traded;
                if (buy.qty == 0)
                    level.pop();
            }
        }
        if (order.qty > 0)
        {
            asks[order.price].add(order);
            orderIndex[order.id] = {Side::Sell, order.price};
        }
    }
}

// Market Order
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
                trades.push_back({order.id, sell.id, price, traded});
                order.qty -= traded;
                sell.qty -= traded;
                if (sell.qty == 0)
                    level.pop();
            }
        }
    }
    else
    { // Sell
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
                trades.push_back({buy.id, order.id, price, traded});
                order.qty -= traded;
                buy.qty -= traded;
                if (buy.qty == 0)
                    level.pop();
            }
        }
    }
}

// Cancel Order
bool OrderBook::cancel(uint64_t orderId)
{
    auto it = orderIndex.find(orderId);
    if (it == orderIndex.end())
        return false;
    Side side = it->second.first;
    int price = it->second.second;

    auto &bookSide = (side == Side::Buy) ? bids : asks;
    auto levelIt = bookSide.find(price);
    if (levelIt == bookSide.end())
        return false;

    auto &level = levelIt->second;
    deque<Order> temp;
    while (!level.empty())
    {
        Order o = level.front();
        level.pop();
        if (o.id != orderId)
            temp.push_back(o);
    }
    for (auto &o : temp)
        level.add(o);
    orderIndex.erase(orderId);
    return true;
}

// Total quantities
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

// Best prices
int OrderBook::bestBid() const
{
    if (bids.empty())
        return -1;
    return bids.rbegin()->first;
}
int OrderBook::bestAsk() const
{
    if (asks.empty())
        return -1;
    return asks.begin()->first;
}

// Get trades
const vector<Trade> &OrderBook::getTrades() const { return trades; }

// Print Book
void OrderBook::printBook() const
{
    cout << "\n======= ORDER BOOK =======\nASKS:\n";
    for (const auto &[price, level] : asks)
        if (!level.empty())
            cout << price << " | " << level.totalQty() << "\n";
    cout << "BIDS:\n";
    for (const auto &[price, level] : bids)
        if (!level.empty())
            cout << price << " | " << level.totalQty() << "\n";
}

size_t OrderBook::tradeCount() const
{
    return trades.size();
}