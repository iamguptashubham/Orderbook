#pragma once

#include <cstdint>
#include "enums.h"
#include "helper.h"

struct Order {
    uint64_t id;
    int quantity;
    BookSide side;
    double price;
    uint64_t timestamp;

    Order(int q, double p, BookSide s, uint64_t t = unix_time()) :
    id(generate_unique_id()),
    quantity(q),
    price(p),
    side(s),
    timestamp(t){}
};