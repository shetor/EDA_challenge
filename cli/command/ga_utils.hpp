#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

struct fit_area_delay {
    double fitness = 0;
    double area = 0;
    double delay = 0;
    double fit_prob = 0;
    double norm_fitness = 0;
};

std::vector <std::string> get_random_add_lut_sequence(const std::vector <std::string> &strings, int algo_num) {
    std::vector <std::string> sequences;
    std::vector<int> counts(strings.size(), 0);
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (int i = 0; i < algo_num; i++) {
        int index = std::uniform_int_distribution<int>(0, (int)strings.size() - 1)(gen);
        counts[index]++;
    }
    for (int i = 0; i < (int)strings.size(); i++) {
        const std::string& str = strings[i];
        int count = counts[i];
        sequences.insert(sequences.end(), count, str);
    }
    std::shuffle(sequences.begin(), sequences.end(), gen);
    sequences.emplace_back("map_fpga;");
    return sequences;
}

////随机生成初始序列
std::vector <std::string> get_random_sequence(const std::vector <std::string> &strings, int algo_num,
                                              std::vector <std::vector<std::string>> &available_macros) {
    std::vector<int> counts(strings.size(), 0);
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<> dis(0.0, 1.0);
    double random_num = dis(gen);
//    std::cout<<"random num:"<<random_num<<std::endl;
    for (int i = 0; i < algo_num; i++) {
        int index = std::uniform_int_distribution<int>(0, (int)strings.size() - 1)(gen);
        counts[index]++;
    }

    std::vector <std::string> sequences;
    if (random_num >= 0.5) {
        for (int i = 0; i < (int)strings.size(); i++) {
            const std::string& str = strings[i];
            int count = counts[i];
            sequences.insert(sequences.end(), count, str);
        }

        std::shuffle(sequences.begin(), sequences.end(), gen);
    } else {
//        if (random_num>=0.4){sequences = macro_1;}
//        if (random_num>=0.3 && random_num<0.4){sequences = macro_2;}
//        if (random_num>=0.2 && random_num<0.3){sequences = macro_3;}
//        if (random_num>=0.1 && random_num<0.2){sequences = macro_4;}
//        if (random_num>=0.0 && random_num<0.1){sequences = macro_5;}
        if (!available_macros.empty()){
            int index = std::uniform_int_distribution<int>(0,(int)available_macros.size()-1)(gen);
            sequences = available_macros[index];
            available_macros.erase(available_macros.begin()+index);
        }else{
            for (int i = 0; i < algo_num; i++) {
                int index = std::uniform_int_distribution<int>(0, (int)strings.size() - 1)(gen);
                counts[index]++;
            }
            for (int i = 0; i < (int)strings.size(); i++) {
                const std::string& str = strings[i];
                int count = counts[i];
                sequences.insert(sequences.end(), count, str);
            }
            std::shuffle(sequences.begin(), sequences.end(), gen);
        }
    }
    sequences.emplace_back("map_fpga;");
    return sequences;
}

////判断父母是否等于macro
bool is_not_vector_equal(const std::vector <std::string> &v1, const std::vector <std::string> &v2) {
    if (v1.size() != v2.size()) { return true; }
    for (size_t i = 0; i < v1.size(); i++) {
        if (v1[i] != v2[i]) { return true; }
    }
    return false;
}
bool is_not_vector_in_list(const std::vector<std::string> &vec,const std::vector<std::vector<std::string>>& list){
    for (const auto &macro: list) {
        if (!is_not_vector_equal(vec,macro)){
            return false;
        }
    }
    return true;
}

double reward_func(double current_area, double current_delay, double no_opt_area, double no_opt_delay) {
    double qor = 0.4 * (double) (current_area / no_opt_area) + 0.6 * (double) (current_delay / no_opt_delay);
    return qor;
}

////计算适应度
double fitness_func(double current_area, double current_delay, double no_opt_area, double no_opt_delay) {
    double fitness_value = 1 - pow(reward_func(current_area, current_delay, no_opt_area, no_opt_delay), 2);
    return fitness_value;
}

////选取适应度高的几个序列
std::vector <std::string>
find_top_better_strings(const std::unordered_map <std::string, fit_area_delay> &seq_to_fitness_map) {
    std::vector <std::string> sorted_strings;  // 存储按 fitness 排序的字符串
    for (const auto &entry: seq_to_fitness_map) {
        const std::string &str = entry.first;
        sorted_strings.push_back(str);
        std::sort(sorted_strings.begin(), sorted_strings.end(), [&](const std::string &a, const std::string &b) {
            return seq_to_fitness_map.find(a)->second.fitness >= seq_to_fitness_map.find(b)->second.fitness;
        });
    }
    std::vector <std::string> better_seq;
    int count = 0;
    for (const auto &item: sorted_strings) {
        better_seq.push_back(item);
        count++;
        if (count == 3) { break; }
    }
    return better_seq;
}

////轮盘赌选择
std::string ga_select(std::unordered_map <std::string, fit_area_delay> &seq_to_db_map,
                      const std::vector <std::string> &current_population_v) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::string select_sequences;
    // 生成一个0到总概率之间的随机数
    std::uniform_real_distribution<> dis(0.0, 1.0);
    double random_num = dis(gen);
    bool not_all_zero_flag = false;
    for (const auto &item: seq_to_db_map) {
        if (item.second.fit_prob != 0) {
            not_all_zero_flag = true;
            break;
        }
    }
    if (!not_all_zero_flag) {
        for (auto &item: seq_to_db_map) {
//            std::cout << "come in" << std::endl;
            item.second.fit_prob = 1.0 / double(current_population_v.size());
//            std::cout << "in fitness:" << item.second.fit_prob << std::endl;
        }
    }
//    std::cout<<"ga_select random num:"<<random_num<<std::endl;
    double cumulative_probability = 0.0;
    for (const auto &string: current_population_v) {
        cumulative_probability += seq_to_db_map.find(string)->second.fit_prob;
//        std::cout << "out fitness:" << seq_to_db_map.find(string)->second.fit_prob << std::endl;
        if (cumulative_probability >= random_num) {
            select_sequences = string; // 返回与选中概率匹配的字符串
            break;
        }
    }
//    std::cout<<"select_sequence:"<<select_sequences<<std::endl;
    return select_sequences;
}

////将以；为间隔的算子字符串转换为以vector type
std::vector <std::string> string_to_vector(const std::string &input_string) {
    std::vector <std::string> result;
    std::istringstream iss(input_string);
    std::string token;
    while (std::getline(iss, token, ';')) {
        result.push_back(token + ';');
    }
    return result;
}

////交叉
std::string crossover_op(std::vector <std::string> seq_1, std::vector <std::string> seq_2) {
    std::vector <std::string> after_cross_x;
    int seq_num = std::min((int)seq_1.size(), (int)seq_2.size());
    int position_1 = 0;
    int position_2 = 0;
    if (seq_num == 0) {
        std::cout << "Error! The cross sequences is 0!" << std::endl;
    } else if (seq_num == 1) {
        position_1 = position_2 = 1;
    } else {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, seq_num);
        position_1 = dis(gen);
        position_2 = dis(gen);
    }
    for (int i = 0; i < seq_num; ++i) {
        if (std::min(position_1, position_2) <= i && i < std::max(position_1, position_2)) {
            after_cross_x.push_back(seq_2[i]);
        } else {
            after_cross_x.push_back(seq_1[i]);
        }
    }

    std::string string_after_cross_x = std::accumulate(after_cross_x.begin(), after_cross_x.end(), std::string());

    return string_after_cross_x;
}

////变异
std::vector <std::string> mutation(const std::vector <std::string>& sequence) {
    std::vector <std::string> mutated_sequence = sequence;
//
//    for (const auto &item: mutated_sequence) {
//        std::cout<<"before mutate seq:"<<item<<std::endl;
//    }
    int seq_num = (int)mutated_sequence.size();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_of_position_num(1, seq_num);
    std::uniform_int_distribution<> dis_of_position(0, seq_num - 1);

    int position_num = dis_of_position_num(gen);
//    std::cout<<"position_num:"<<position_num<<std::endl;

    int position = dis_of_position(gen);
//    std::cout<<"position1:"<<position<<std::endl;
    std::vector <std::int32_t> used_position;

    for (int i = 1; i <= position_num - 1; ++i) {
        std::vector <std::string> operations = {"balance;", "rewrite;", "rewrite -z;", "rewrite -v;", "refactor;",
                                                "refactor -z;", "refactor -v;"};
        mutated_sequence[position] = operations[dis_of_position(gen) % operations.size()];

        do {
            position = dis_of_position(gen);
//            std::cout<<"position:"<<position<<std::endl;
        } while (std::find(used_position.begin(), used_position.end(), position) != used_position.end());
        used_position.push_back(position);
//        std::cout<<"position"<<count<<":"<<position<<std::endl;
    }
    mutated_sequence.emplace_back("map_fpga;");
//    for (const auto &item: mutated_sequence) {
//        std::cout<<"after mutate seq:"<<item<<std::endl;
//    }
//    int count = 1;
//    for (const auto &item: used_position) {
//        std::cout<<"position"<<count<<":"<<item<<std::endl;
//        count++;
//    }
    return mutated_sequence;
}


