#pragma once
#include <cstdint>

struct Trade
{
    uint64_t buyId;
    uint64_t sellId;
    int price;
    int qty;
};
