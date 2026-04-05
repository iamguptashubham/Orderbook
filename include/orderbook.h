#pragma once

#include <chrono>
#include <cstdlib>
#include <deque>
#include <map>
#include <memory>
#include <thread>
#include <unordered_map>

#include "enums.h"
#include "order.h"

class Orderbook {
   private:
    std::map<double, std::deque<std::unique_ptr<Order>>, std::greater<double>> m_bids;
    std::map<double, std::deque<std::unique_ptr<Order>>, std::less<double>> m_asks;

    // case for modify /delete
    std::unordered_map<uint64_t, std::pair<BookSide, double>> m_order_metadata;

   public:
    Orderbook(bool generate_dummies);

    void addOrder(int qty, double price, BookSide side);
    std::pair<int, double> handle_order(OrderType type, int order_qty, Side side, double price = 0);

    bool modifyOrder(uint64_t id, int new_qty);
    bool deleteOrder(uint64_t id);

    template <typename T>
    std::pair<int, double> fill_order(
        std::map<double, std::deque<std::unique_ptr<Order>>, T>& offers, const OrderType type,
        const Side side, int& order_qty, double price, int& unit_transacted, double& total_value);

    double best_quote(BookSide side);

    const auto& get_bids() {
        return m_bids;
    }
    const auto& get_asks() {
        return m_asks;
    };

    template <typename T>
    void print_leg(std::map<double, std::deque<std::unique_ptr<Order>>, T>& orders, BookSide side);

    void print();
};
