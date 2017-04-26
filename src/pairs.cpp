//
// Created by Snytkine, Dmitri (CORP) on 4/22/17.
//
#include <vector>
#include <algorithm>
#include <iostream>


int main() {

    // If you can't use c++11, then this is identical:
    struct {
        bool operator()(std::pair<int, int> lhs, std::pair<int, int> rhs) const {
            return lhs.second < rhs.second;
        }
    } Compare;


    std::vector<std::pair<int, int>> values = {
            {10, 100},
            {30, 120},
            {14, 110},
            {18, 200},
            {20, 230},
            {13, 49},
    };

    std::make_heap(values.begin(), values.end(), Compare);
    //std::sort(values.begin(), values.end(), Compare);


    for (auto &&i: values) {
        std::cout << "i: " << i.first << ":" << i.second << std::endl;
    }


}