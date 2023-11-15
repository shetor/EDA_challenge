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
    std::vector<std::string> macro_1 = {"balance;rewrite;rewrite -z;balance;rewrite -z;balance;"};
    std::vector<std::string> macro_2 = {
            "balance;rewrite;refactor;balance;rewrite;rewrite -z;balance;refactor -z;rewrite -z;balance;"};
    std::vector<std::string> macro_3 = {"balance;rewrite;balance;rewrite;rewrite -z;balance;rewrite -z;balance;"};
    std::vector<std::string> macro_4 = {"balance;rewrite -l;rewrite -z -l;rewrite - -z -l;balance;"};
    std::vector<std::string> macro_5 = {
            "balance;rewrite -l;refactor -l;balance;rewrite -z -l;balance;refactor -z;rewrite -z;balance;"};
    std::vector<int> counts(strings.size(), 0);
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<> dis(0.0, 1.0);
    double random_num = dis(gen);
    std::cout<<"random num:"<<random_num<<std::endl;
    for (int i = 0; i < algo_num; i++) {
        int index = std::uniform_int_distribution<int>(0, strings.size() - 1)(gen);
        counts[index]++;
    }

    std::vector<std::string> sequences;
    if (random_num >= 0.5) {
        for (int i = 0; i < strings.size(); i++) {
            std::string str = strings[i];
            int count = counts[i];
            sequences.insert(sequences.end(), count, str);
        }

        std::shuffle(sequences.begin(), sequences.end(), gen);
    }else{
        if (random_num>=0.4){sequences = macro_1;std::cout<<"macro1"<<std::endl;}
        if (random_num>=0.3 && random_num<0.4){sequences = macro_2;std::cout<<"macro2"<<std::endl;}
        if (random_num>=0.2 && random_num<0.3){sequences = macro_3;std::cout<<"macro3"<<std::endl;}
        if (random_num>=0.1 && random_num<0.2){sequences = macro_4;std::cout<<"macro4"<<std::endl;}
        if (random_num>=0.0 && random_num<0.1){sequences = macro_5;std::cout<<"macro5"<<std::endl;}
    }
    return sequences;
}

bool is_vector_equal(const std::vector<std::string>& v1, const std::vector<std::string>& v2){
    if(v1.size() != v2.size()){return true;}
    for(size_t i = 0; i<v1.size(); i++){
        if(v1[i] != v2[i]){return true;}
    }
    return false;
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
        if (sorted_strings.size() > seq_to_fitness_map.size() / 3 ) {
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
    int seq_num = std::min(seq_1.size(),seq_2.size());
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


