#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

std::vector <std::string> get_random_sequence(const std::vector <std::string> &strings, int algo_num) {
    std::vector<int> counts(strings.size(), 0);
    std::random_device rd;
    std::mt19937 gen(rd());

    for (int i = 0; i < algo_num; i++) {
        int index = std::uniform_int_distribution<int>(0, strings.size() - 1)(gen);
        counts[index]++;
    }

    std::vector <std::string> sequences;

    for (int i = 0; i < strings.size(); i++) {
        std::string str = strings[i];
        int count = counts[i];
        sequences.insert(sequences.end(), count, str);
    }

    std::shuffle(sequences.begin(), sequences.end(), gen);
    sequences.push_back("map_fpga");
    return sequences;
}

double reward_func(double current_area, double current_delay, double no_opt_area, double no_opt_delay) {
    double qor = 0.4 * (double)(current_area / no_opt_area) + 0.6 * (double )(current_delay / no_opt_delay);
    return qor;
}

double fitness_func(double current_area, double current_delay, double no_opt_area, double no_opt_delay) {
    double fitness_value = 1 / reward_func(current_area, current_delay, no_opt_area, no_opt_delay);
    return fitness_value;
}