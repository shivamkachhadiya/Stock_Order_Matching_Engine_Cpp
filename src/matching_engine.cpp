#include "../include/matching_engine.hpp"
#include <algorithm>

using namespace std;

// ===========================================
// LIMIT ORDER
// ===========================================
void MatchingEngine::addLimit(Order order)
{
    if (order.side == Side::Buy)
    {
        if (book.hasAsk(order.price))
        {
            auto &level = book.askLevel(order.price);

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
            book.bidLevel(order.price).add(order);
    }
    else
    {
        if (book.hasBid(order.price))
        {
            auto &level = book.bidLevel(order.price);

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
            book.askLevel(order.price).add(order);
    }
}

// ===========================================
// MARKET ORDER
// ===========================================
void MatchingEngine::addMarket(Order order)
{
    if (order.side == Side::Buy)
    {
        while (order.qty > 0)
        {
            int price = book.bestAsk();
            if (price == -1)
                break;

            auto &level = book.askLevel(price);

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
            int price = book.bestBid();
            if (price == -1)
                break;

            auto &level = book.bidLevel(price);

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
