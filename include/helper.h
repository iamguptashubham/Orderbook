#pragma once

#include <chrono>
#include <functional>
#include <iostream>

inline uint64_t unix_time() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(
               std::chrono::high_resolution_clock::now().time_since_epoch())
        .count();
}

inline uint64_t generate_unique_id() {
    static uint64_t s_next_id{0};
    return ++s_next_id;
}

void print_fill(std::pair<int, double> fill, int quantity, u_int64_t start_time,
                u_int64_t end_time);

// Defer helper to run a function at scope exit
struct Defer {
    std::function<void()> f;
    Defer(std::function<void()> f_) : f(f_) {}
    ~Defer() {
        f();
    }
};

// Macro to profile scope in nanoseconds
#define PROFILE_SCOPE(name_str)                                                                  \
    auto start_time = std::chrono::high_resolution_clock::now();                                 \
    auto end_scope = [&]() {                                                                     \
        auto end_time = std::chrono::high_resolution_clock::now();                               \
        auto duration =                                                                          \
            std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count(); \
        std::cout << name_str << " took " << duration << "ns\n";                                 \
    };                                                                                           \
    Defer end_defer(end_scope);
