#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

std::vector<std::string> get_random_sequence(const std::vector<std::string>& strings, int algo_num) {
    std::vector<int> counts(strings.size(), 0);
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < algo_num; i++) {
        int index = std::uniform_int_distribution<int>(0, strings.size() - 1)(gen);
        counts[index]++;
    }

    std::vector<std::string> sequences;

    for (int i = 0; i < strings.size(); i++) {
        std::string str = strings[i];
        int count = counts[i];
        sequences.insert(sequences.end(), count, str);
    }

    std::shuffle(sequences.begin(), sequences.end(), gen);
    sequences.push_back("map_fpga");
    return sequences;
}