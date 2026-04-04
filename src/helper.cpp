#include "helper.h"

void print_fill(std::pair<int, double> fill, int quantity, u_int64_t start_time, u_int64_t end_time)
{
    std::cout<<"Filled "<<fill.first<<"/"<<quantity<<" units @ Rs."<<fill.second/fill.first<<" average price. Time taken: "
    <<(end_time - start_time)<<" nano seconds "<<std::endl;
}