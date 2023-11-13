#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>


struct fit_area_delay{
    double fitness=0;
    double area =0;
    double delay = 0;
    double fit_prob = 0;
};
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
    return sequences;
}
double reward_func(double current_area, double current_delay, double no_opt_area, double no_opt_delay) {
    double qor = 0.4 * (double) (current_area / no_opt_area) + 0.6 * (double) (current_delay / no_opt_delay);
    return qor;
}
double fitness_func(double current_area, double current_delay, double no_opt_area, double no_opt_delay) {
    double fitness_value = 1 / reward_func(current_area, current_delay, no_opt_area, no_opt_delay);
    return fitness_value;
}

std::vector<std::string> find_top_half_strings(const std::unordered_map<std::string,fit_area_delay>& seq_to_fitness_map) {
    std::vector<std::string> sorted_strings;  // 存储按 fitness 排序的字符串
    for (const auto& entry : seq_to_fitness_map) {
        const std::string& str = entry.first;


        sorted_strings.push_back(str);
        std::sort(sorted_strings.begin(), sorted_strings.end(), [&](const std::string& a, const std::string& b) {
            return seq_to_fitness_map.find(a)->second.fitness >= seq_to_fitness_map.find(b)->second.fitness;
        });

        // 保持 vector 的大小为前一半
        if (sorted_strings.size() > seq_to_fitness_map.size() / 2) {
            sorted_strings.pop_back();
        }
    }

    return sorted_strings;
}
////轮盘赌选择
std::string ga_select(std::unordered_map<std::string, fit_area_delay> half_seq_to_db_map) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::string select_sequences;
    // 生成一个0到总概率之间的随机数
    std::uniform_real_distribution<> dis(0.0, 1.0);
    double random_num = dis(gen);

    double cumulative_probability = 0.0;
    for (const auto& pair : half_seq_to_db_map) {
        cumulative_probability += pair.second.fit_prob;
        if (cumulative_probability >= random_num) {
            select_sequences = pair.first; // 返回与选中概率匹配的字符串
            break;
        }
    }
    return select_sequences;
}
////将以；为间隔的算子字符串转换为以vector type
std::vector<std::string> string_to_vector(const std::string& input_string) {
    std::vector<std::string> result;
    std::istringstream iss(input_string);
    std::string token;
    while (std::getline(iss, token, ';')) {
        result.push_back(token+';');
    }
    return result;
}
////交叉
std::string crossover_op(std::vector<std::string> seq_1,std::vector<std::string> seq_2){
    std::vector<std::string>after_cross_x;
    int seq_num = seq_1.size();
    int position_1 = 0;
    int position_2 = 0;
    if (seq_num == 0){
        std::cout<<"Error! The cross sequences is 0!"<<std::endl;
    }
    else if(seq_num ==1){
        position_1 = position_2 =1;
    }
    else{
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, seq_num);
        position_1 = dis(gen);
        position_2 = dis(gen);
    }
    for (int i = 0; i < seq_num; ++i) {
        if (std::min(position_1,position_2)<=i && i< std::max(position_1,position_2)){
            after_cross_x.push_back(seq_2[i]);
        }
        else{
            after_cross_x.push_back(seq_1[i]);
        }
    }

    std::string string_after_cross_x = std::accumulate(after_cross_x.begin(),after_cross_x.end(),std::string());

    return string_after_cross_x;
}
////变异
std::string mutation(std::vector<std::string> sequence){
    std::vector<std::string> mutated_sequence = sequence;
    int seq_num = mutated_sequence.size();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, seq_num);
    int position = dis(gen);
    std::vector <std::string> operations = {"balance;", "rewrite;",  "rewrite -z;", "rewrite -v;", "refactor;",
                                         "refactor -z;", "refactor -v;"};
    mutated_sequence[position] = operations[dis(gen)%operations.size()];
    std::string string_mutated_sequence = std::accumulate(mutated_sequence.begin(),mutated_sequence.end(),std::string());

    return string_mutated_sequence;

}