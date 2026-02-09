#pragma once
#include <deque>
#include "order.hpp"

class PriceLevel
{
private:
    std::deque<Order> orders; // FIFO queue

public:
    void add(const Order &order)
    {
        orders.push_back(order);
    }

    Order &front()
    {
        return orders.front();
    }

    void pop()
    {
        orders.pop_front();
    }

    bool empty() const
    {
        return orders.empty();
    }

    int totalQty() const
    {
        int sum = 0;
        for (const auto &o : orders)
            sum += o.qty;
        return sum;
    }
};
