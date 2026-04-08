#include <iostream>

#include "../include/helper.h"
#include "../include/orderbook.h"

int main() {
    Orderbook ob(true);
    while (true) {
        int action;
        std::cout << "Options\n————————————————————\n|1. Print Orderbook|\n|2. Submit order   |\n "
                     "————————————————————\nChoice: ";
        std::cin >> action;
        std::cout << "\n";

        if (action == 1) {
            ob.print();
        } else if (action == 2) {
            int order_input_type;
            int quantity;
            int side_input;
            double price;

            std::cout << "Enter order type:\n0. Market order\n1. Limit order\nSelection: ";
            std::cin >> order_input_type;
            OrderType order_type = static_cast<OrderType>(order_input_type);

            std::cout << "\nEnter side:\n0. Buy\n1. Sell\nSelection: ";
            std::cin >> side_input;
            Side side = static_cast<Side>(side_input);

            std::cout << "\nEnter order quantity: ";
            std::cin >> quantity;

            if (order_type == OrderType::market) {
                std::cout << "\nSubmitting market " << ((side == Side::buy) ? "buy" : "sell")
                          << " order for " << quantity << " units.." << " \n";

                u_int64_t start_time = unix_time();
                std::pair<int, double> fill = ob.handle_order(order_type, quantity, side);
                u_int64_t end_time = unix_time();
                print_fill(fill, quantity, start_time, end_time);
            } else if (order_type == OrderType::limit) {
                std::cout << "\nEnter limit price: ";
                std::cin >> price;

                std::cout << "\nSubmitting limit " << ((side == Side::buy) ? "buy" : "sell")
                          << " order for " << quantity << " units @ Rs. " << price << "\n";

                u_int64_t start_time = unix_time();
                std::pair<int, double> fill = ob.handle_order(order_type, quantity, side, price);
                u_int64_t end_time = unix_time();
                print_fill(fill, quantity, start_time, end_time);
            }
        } else {
            std::cout << "Invalid Option! Please try again \n";
        }
    }
    return 0;
}