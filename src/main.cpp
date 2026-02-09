#include "../include/order_book.hpp"
#include <iostream>

int main()
{
    OrderBook book;

    book.addLimit({1, Side::Buy, 100, 10, 0});
    book.addLimit({2, Side::Buy, 100, 5, 0});
    book.addLimit({3, Side::Sell, 105, 7, 0});
    book.addLimit({4, Side::Sell, 100, 4, 0});

    std::cout << "Bid@100 = " << book.totalBidQty(100) << "\n";
    std::cout << "Ask@105 = " << book.totalAskQty(105) << "\n";

    std::cout << "BestBid = " << book.bestBid() << "\n";
    std::cout << "BestAsk = " << book.bestAsk() << "\n";
}
