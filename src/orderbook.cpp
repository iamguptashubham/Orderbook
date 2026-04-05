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

template <typename T>
inline std::pair<int, double> Orderbook::fill_order(
    std::map<double, std::deque<std::unique_ptr<Order>>, T>& offers, const OrderType type,
    const Side side, int& order_qty, double price, int& unit_transacted, double& total_value) {
    // Iterate over price levels
    auto rit = offers.begin();
    while (rit != offers.end()) {
        const double price_level = rit->first;
        auto& orders = rit->second;

        // For a limit order, ensure the price level is acceptable
        // market order always acceptable price

        bool can_transact = true;
        if (type == OrderType::limit) {
            if (side == Side::buy && price_level > price) {
                can_transact = false;
            } else if (side == Side::sell && price_level < price) {
                can_transact = false;
            }
        }

        if (can_transact) {
            while (!orders.empty() && order_qty > 0) {
                auto& current_order = orders.front();
                const u_int64_t order_id = current_order->id;
                int current_qty = current_order->quantity;
                double current_price = current_order->price;

                if (current_qty > order_qty) {  // Partial Fills
                    unit_transacted += order_qty;
                    total_value += order_qty * current_price;
                    current_order->quantity = current_qty - order_qty;
                    order_qty = 0;
                    break;
                } else {  // Full fill
                    unit_transacted += current_qty;
                    total_value += current_qty * current_price;
                    order_qty -= current_qty;
                    orders.pop_front();

                    m_order_metadata.erase(order_id);
                }
            }

            // remove map entry if we wiped all the orders
            if (orders.empty()) {
                rit = offers.erase(rit);

            } else {
                if (order_qty > 0)
                    rit++;
                else
                    break;
            }
        } else {
            // Price will only get worse break;
            break;
        }
    }

    return std::make_pair(unit_transacted, total_value);
}

std::pair<int, double> Orderbook::handle_order(OrderType type, int order_qty, Side side,
                                               double price) {
    int units_transacted = 0;
    double total_value = 0;

    if (type == OrderType::market) {
        if (side == Side::sell) {
            return fill_order(m_bids, OrderType::market, Side::sell, order_qty, price,
                              units_transacted, total_value);
        } else if (side == Side::buy) {
            return fill_order(m_asks, OrderType::market, Side::sell, order_qty, price,
                              units_transacted, total_value);
        }
    } else if (type == OrderType::limit) {
        if (side == Side::buy) {
            if (best_quote(BookSide::ask) <= price) {
                auto fill = fill_order(m_asks, OrderType::limit, Side::buy, order_qty, price,
                                       units_transacted, total_value);
                if (order_qty > 0) {
                    addOrder(order_qty, price, BookSide::bid);
                }
                return fill;
            } else {
                addOrder(order_qty, price, BookSide::bid);
                return std::make_pair(units_transacted, total_value);
            }
        } else {
            if (best_quote(BookSide::bid) >= price) {
                auto fill = fill_order(m_bids, OrderType::limit, Side::sell, order_qty, price,
                                       units_transacted, total_value);
                if (order_qty > 0) {
                    addOrder(order_qty, price, BookSide::ask);
                }
                return fill;
            } else {
                addOrder(order_qty, price, BookSide::ask);
                return std::make_pair(units_transacted, total_value);
            }
        }
    } else {
        throw std::runtime_error("Invalid order type encountered");
    }
    return std::make_pair(units_transacted, total_value);
}

double Orderbook::best_quote(BookSide side) {
    if (side == BookSide::bid) {
        return m_bids.begin()->first;
    } else if (side == BookSide::ask) {
        return m_asks.begin()->first;
    }
    return 0.0;
}
