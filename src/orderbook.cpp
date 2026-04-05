#include "../include/orderbook.h"

#include "orderbook.h"

Orderbook::Orderbook(bool generate_dummies) {
    srand(12);

    if (generate_dummies) {
        // Add some dummy bid orders
        for (int i = 0; i < 3; i++) {
            double random_price = 90.0 + (rand() % 1001) / 100.0;
            int random_qty = rand() % 100 + 1;
            int random_qty2 = rand() % 100 + 1;

            addOrder(random_qty, random_price, BookSide::bid);
            std::this_thread::sleep_for(
                std::chrono::milliseconds(1));  // ensure different timestamps
            addOrder(random_qty2, random_price, BookSide::bid);
        }

        // Add some dummy ask orders
        for (int i = 0; i < 3; i++) {
            double random_price = 100.0 + (rand() % 1001) / 100.0;
            int random_qty = rand() % 100 + 1;
            int random_qty2 = rand() % 100 + 1;

            addOrder(random_qty, random_price, BookSide::ask);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            addOrder(random_qty2, random_price, BookSide::ask);
        }
    }
}

void Orderbook::addOrder(int qty, double price, BookSide side) {
    auto order = std::make_unique<Order>(qty, price, side);
    uint64_t order_id = order->id;
    if (side == BookSide::bid) {
        m_bids[price].push_back(std::move(order));
        m_order_metadata[order_id] = std::make_pair(side, price);
    } else {
        m_asks[price].push_back(std::move(order));
        m_order_metadata[order_id] = std::make_pair(side, price);
    }
}

std::pair<int, double> Orderbook::handle_order(OrderType type, int order_qty, Side side,
                                               double price) {
    return std::pair<int, double>();
}
