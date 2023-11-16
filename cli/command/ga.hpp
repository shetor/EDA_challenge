#pragma once

#include "alice/alice.hpp"
#include "include/database/network/aig_network.hpp"
#include "include/database/network/klut_network.hpp"
#include "include/database/views/depth_view.hpp"
#include "include/operations/optimization/and_balance.hpp"
#include "include/operations/optimization/refactor.hpp"
#include "include/operations/optimization/rewrite.hpp"
#include "ga_utils.hpp"
#include "include/database/views/mapping_view.hpp"
#include "include/operations/algorithms/aig_with_choice.hpp"
#include "include/operations/algorithms/choice_computation.hpp"
#include "include/operations/algorithms/choice_miter.hpp"
#include "include/operations/algorithms/klut_mapping.hpp"
#include "include/operations/algorithms/network_to_klut.hpp"
#include "include/operations/optimization/rewrite.hpp"
#include "include/operations/optimization/refactor.hpp"
#include "include/operations/optimization/and_balance.hpp"
#include <fstream>
#include <ctime>

namespace alice {

    class ga_command : public command {
    public:
        explicit ga_command(const environment::ptr &env) : command(env, "GA!") {
            add_option("--output_file_path, -O", outfile_path, "output the sequences to the output file path");

        }

        rules validity_rules() const { return {}; }

    protected:
        void run_algo_seq(std::vector <std::string> algo_sequence) {
            for (const std::string &str: algo_sequence) {
                if (str == "balance;") {
                    store<iFPGA::aig_network>().current() = iFPGA::balance_and(store<iFPGA::aig_network>().current());
                }
                if (str == "rewrite;") {
                    uint32_t cut_size = 4u;
                    uint32_t priority_size = 10u;
                    bool preserve_level = true;
                    bool zero_gain = false;
                    iFPGA::aig_network aig = store<iFPGA::aig_network>().current();
                    iFPGA::rewrite_params params;
                    params.b_preserve_depth = preserve_level;
                    params.b_use_zero_gain = zero_gain;
                    params.cut_enumeration_ps.cut_size = cut_size;
                    params.cut_enumeration_ps.cut_limit = priority_size;
                    aig = iFPGA::rewrite(aig, params);
                    store<iFPGA::aig_network>().current() = aig;

                }
                if (str == "rewrite -z;") {
                    uint32_t cut_size = 4u;
                    uint32_t priority_size = 10u;
                    bool preserve_level = true;
                    bool zero_gain = true;
                    iFPGA::aig_network aig = store<iFPGA::aig_network>().current();
                    iFPGA::rewrite_params params;
                    params.b_preserve_depth = preserve_level;
                    params.b_use_zero_gain = zero_gain;
                    params.cut_enumeration_ps.cut_size = cut_size;
                    params.cut_enumeration_ps.cut_limit = priority_size;
                    aig = iFPGA::rewrite(aig, params);
                    store<iFPGA::aig_network>().current() = aig;

                }
                if (str == "rewrite -l;") {
                    uint32_t cut_size = 4u;
                    uint32_t priority_size = 10u;
                    bool preserve_level = false;
                    bool zero_gain = false;
                    iFPGA::aig_network aig = store<iFPGA::aig_network>().current();
                    iFPGA::rewrite_params params;
                    params.b_preserve_depth = preserve_level;
                    params.b_use_zero_gain = zero_gain;
                    params.cut_enumeration_ps.cut_size = cut_size;
                    params.cut_enumeration_ps.cut_limit = priority_size;
                    aig = iFPGA::rewrite(aig, params);
                    store<iFPGA::aig_network>().current() = aig;

                }
                if (str == "rewrite -z -l;") {
                    uint32_t cut_size = 4u;
                    uint32_t priority_size = 10u;
                    bool preserve_level = false;
                    bool zero_gain = true;
                    iFPGA::aig_network aig = store<iFPGA::aig_network>().current();
                    iFPGA::rewrite_params params;
                    params.b_preserve_depth = preserve_level;
                    params.b_use_zero_gain = zero_gain;
                    params.cut_enumeration_ps.cut_size = cut_size;
                    params.cut_enumeration_ps.cut_limit = priority_size;
                    aig = iFPGA::rewrite(aig, params);
                    store<iFPGA::aig_network>().current() = aig;

                }

                if (str == "refactor;") {
                    uint32_t input_size = 10u;
                    uint32_t cone_size = 16u;
                    bool preserve_level = true;
                    bool zero_gain = false;
                    bool verbose = false;
                    iFPGA::aig_network aig = store<iFPGA::aig_network>().current();
                    iFPGA::refactor_params params;
                    params.allow_depth_up = preserve_level;
                    params.allow_zero_gain = zero_gain;
                    params.max_leaves_num = input_size;
                    params.max_cone_size = cone_size;
                    params.verbose = verbose;
                    aig = iFPGA::refactor(aig, params);

                    store<iFPGA::aig_network>().current() = aig;

                }
                if (str == "refactor -z;") {
                    uint32_t input_size = 10u;
                    uint32_t cone_size = 16u;
                    bool preserve_level = true;
                    bool zero_gain = true;
                    bool verbose = false;
                    iFPGA::aig_network aig = store<iFPGA::aig_network>().current();
                    iFPGA::refactor_params params;
                    params.allow_depth_up = preserve_level;
                    params.allow_zero_gain = zero_gain;
                    params.max_leaves_num = input_size;
                    params.max_cone_size = cone_size;
                    params.verbose = verbose;
                    aig = iFPGA::refactor(aig, params);

                    store<iFPGA::aig_network>().current() = aig;

                }
                if (str == "refactor -l;") {
                    uint32_t input_size = 10u;
                    uint32_t cone_size = 16u;
                    bool preserve_level = false;
                    bool zero_gain = false;
                    bool verbose = false;
                    iFPGA::aig_network aig = store<iFPGA::aig_network>().current();
                    iFPGA::refactor_params params;
                    params.allow_depth_up = preserve_level;
                    params.allow_zero_gain = zero_gain;
                    params.max_leaves_num = input_size;
                    params.max_cone_size = cone_size;
                    params.verbose = verbose;
                    aig = iFPGA::refactor(aig, params);

                    store<iFPGA::aig_network>().current() = aig;

                }
                if (str == "refactor -v;") {
                    uint32_t input_size = 10u;
                    uint32_t cone_size = 16u;
                    bool preserve_level = true;
                    bool zero_gain = false;
                    bool verbose = true;
                    iFPGA::aig_network aig = store<iFPGA::aig_network>().current();
                    iFPGA::refactor_params params;
                    params.allow_depth_up = preserve_level;
                    params.allow_zero_gain = zero_gain;
                    params.max_leaves_num = input_size;
                    params.max_cone_size = cone_size;
                    params.verbose = verbose;
                    aig = iFPGA::refactor(aig, params);

                    store<iFPGA::aig_network>().current() = aig;
                }
                if (str == "map_fpga;") {
                    if (store<iFPGA::klut_network>().empty()) {
                        store<iFPGA::klut_network>().extend();
                    }
                    uint32_t priority_size = 10u;
                    uint32_t cut_size = 6u;
                    uint32_t iFlowIter = 1;
                    uint32_t iAreaIter = 2;
                    bool verbose = false;
                    iFPGA::klut_mapping_params param_mapping;
                    param_mapping.cut_enumeration_ps.cut_size = cut_size;
                    param_mapping.cut_enumeration_ps.cut_limit = priority_size;
                    param_mapping.uFlowIters = iFlowIter;
                    param_mapping.uAreaIters = iAreaIter;
                    param_mapping.verbose = verbose;
                    iFPGA::aig_network aig = store<iFPGA::aig_network>().current();
                    iFPGA::aig_with_choice awc(aig);
                    iFPGA::mapping_view<iFPGA::aig_with_choice, true, false> mapped_aig(awc);


                    iFPGA_NAMESPACE::klut_mapping<decltype(mapped_aig), true>(mapped_aig, param_mapping);
                    const auto kluts = *iFPGA_NAMESPACE::choice_to_klut<iFPGA_NAMESPACE::klut_network>(mapped_aig);
                    store<iFPGA::klut_network>().current() = kluts;

                }
            }
        }
        void execute() {
            clock_t start_time, end_time_1, end_time_2;
            start_time = clock();
            double run_time;
            double run_time_of_stage2;
            iFPGA::aig_network initial_aig = store < iFPGA::aig_network > ().current();
//            int node_num = initial_aig.num_gates();
            int node_num = 5000;
            std::cout<<"node_num:"<<node_num<<std::endl;

            //判断case大小来决定运行时间
            double limited_time_1 ;
            double limited_time_2 ;
            if (node_num<=1000){
                limited_time_1 = 30.0;
                limited_time_2 = 57.0;
            } else if (1000<node_num && node_num<=10000){
                limited_time_1 = 150.0;
                limited_time_2 = 290.0;
            } else{
                limited_time_1 = 1800.0;
                limited_time_2 = 3590.0;
            }

            std::cout<<"limit time 1:"<<limited_time_1<<std::endl;
            std::cout<<"limit time 2:"<<limited_time_2<<std::endl;

            std::shared_ptr<iFPGA::storage<iFPGA::fixed_node<2, 2>, iFPGA::aig_storage_data>> initial_aig_storage =
                    store < iFPGA::aig_network > ().current()._storage;
            iFPGA::aig_network stage2_initial_aig;
            std::shared_ptr<iFPGA::storage<iFPGA::fixed_node<2, 2>, iFPGA::aig_storage_data>> stage2_initial_aig_storage;

            ////map_fpga
            if (store < iFPGA::klut_network > ().empty()) {
                store < iFPGA::klut_network > ().extend();
            }
            uint32_t priority_size = 10u;
            uint32_t cut_size = 6u;
            uint32_t iFlowIter = 1;
            uint32_t iAreaIter = 2;
            bool verbose = false;
            iFPGA::klut_mapping_params param_mapping;
            param_mapping.cut_enumeration_ps.cut_size = cut_size;
            param_mapping.cut_enumeration_ps.cut_limit = priority_size;
            param_mapping.uFlowIters = iFlowIter;
            param_mapping.uAreaIters = iAreaIter;
            param_mapping.verbose = verbose;
            iFPGA::aig_network aig = store < iFPGA::aig_network > ().current();
            iFPGA::aig_with_choice awc(aig);
            iFPGA::mapping_view<iFPGA::aig_with_choice, true, false> mapped_aig(awc);
            iFPGA_NAMESPACE::klut_mapping<decltype(mapped_aig), true>(mapped_aig, param_mapping);
            const auto initial_kluts = *iFPGA_NAMESPACE::choice_to_klut<iFPGA_NAMESPACE::klut_network>(mapped_aig);
            store < iFPGA::klut_network > ().current() = initial_kluts;
            iFPGA::klut_network initial_klut = store < iFPGA::klut_network > ().current()._storage;
            iFPGA::depth_view<iFPGA::klut_network> initial_dklut(initial_klut);

            double no_opt_area = initial_klut.num_gates();
            double no_opt_delay = initial_dklut.depth();
            double sum_fitness = 0;

            ////复原
            store < iFPGA::aig_network > ().current() = initial_aig;
            store < iFPGA::aig_network > ().current()._storage = initial_aig_storage;
            std::unordered_map<std::string, fit_area_delay> seq_to_db_map{};
            std::unordered_map<std::string, fit_area_delay> seq_to_db_map_2{};
            std::unordered_map<std::string, fit_area_delay> half_seq_to_db_map{};
            std::unordered_map<std::string, fit_area_delay> half_seq_to_db_map_2{};
            uint64_t algo_num = 5;
            uint64_t algo_num_of_stage_2 = 10;
            uint64_t sequence_num = 10;
            std::vector<std::string> strings = {"balance;", "rewrite;", "rewrite -z;", "rewrite -l;", "refactor;",
                                                "refactor -z;", "refactor -v;", "refactor -l;", "rewrite -z -l;"};
            std::vector<std::string> macro_1 = {"balance;","rewrite;","rewrite -z;","balance;","rewrite -z;","balance;"};
            std::vector<std::string> macro_2 = {
                    "balance;","rewrite;","refactor;","balance;","rewrite;","rewrite -z;","balance;","refactor -z;","rewrite -z;","balance;"};
            std::vector<std::string> macro_3 = {
                    "balance;","rewrite;","balance;","rewrite;","rewrite -z;","balance;","rewrite -z;","balance;"};
            std::vector<std::string> macro_4 = {"balance;","rewrite -l;","rewrite -z -l;","rewrite -z -l;","balance;"};
            std::vector<std::string> macro_5 = {
                    "balance;","rewrite -l;","refactor -l;","balance;","rewrite -z -l;","balance;","refactor -z;","rewrite -z;","balance;"};

            std::unordered_map<std::string, fit_area_delay> next_seq_to_db_map{};
            std::unordered_map<std::string, fit_area_delay> next_seq_to_db_map_2{};
            std::string best_seq{};
            std::string best_seq_of_2{};
            double mutation_probability = 0.4;
            double cross_probability = 0.6;

            int count = 0;
//            start_time = clock();
            ////生成初始序列,返回seq_to_db_map
            for (uint64_t i = 0; i < sequence_num; ++i) {
                std::vector<std::string> algo_sequence = get_random_sequence(strings, algo_num);
                algo_sequence.push_back("map_fpga;");
                run_algo_seq(algo_sequence);
                ////turn vector to string
                std::string combined_algo_seq_string = std::accumulate(algo_sequence.begin(), algo_sequence.end(),
                                                                       std::string());
//                std::cout<<"Combined String:"<<combined_algo_seq_string<<std::endl;
                iFPGA::klut_network klut = store < iFPGA::klut_network > ().current()._storage;
                iFPGA::depth_view<iFPGA::klut_network> dklut(klut);
                double current_area = klut.num_gates();
                double current_delay = dklut.depth();
                double fitness = fitness_func(current_area, current_delay, no_opt_area, no_opt_delay);

                fit_area_delay tmp_fit_area_delay;
                tmp_fit_area_delay.fitness = fitness;
                tmp_fit_area_delay.area = current_area;
                tmp_fit_area_delay.delay = current_delay;
                seq_to_db_map.emplace(combined_algo_seq_string, tmp_fit_area_delay);

                ////复原为原来一开始输入文件的那个结构
                store < iFPGA::aig_network > ().current() = initial_aig;
                store < iFPGA::aig_network > ().current()._storage = initial_aig_storage;

            }
            std::cout << "test_return initial" << std::endl;
            for (const auto &dbMap: seq_to_db_map) {
                std::cout << "seq_string: " << dbMap.first << std::endl;
                std::cout << "seq_string_area: " << dbMap.second.area << std::endl;
                std::cout << "seq_string_delay: " << dbMap.second.delay << std::endl;
            }

            ////迭代GA 第一次
            for (int i = 0; i < 10; ++i) {
                ////初始化sum_fitness
                sum_fitness = 0;
                double best_fitness = 0.0;
//                std::string best_seq{};
                //// 调用 find_top_half_strings 去找到fitness高的前一半，只有算子序列，无QoR
                std::vector<std::string> top_half_algo_sequences = find_top_half_strings(seq_to_db_map);
                ////找最好的序列
                for (const auto &seq: seq_to_db_map) {
                    std::string current_seq = seq.first;
                    double current_fitness = seq.second.fitness;
                    if (current_fitness > best_fitness) {
                        best_seq = current_seq;
                        best_fitness = current_fitness;
                    }
                }
                //// 输出最佳序列和最佳适应度
                std::cout << "Best Sequence: " << best_seq << std::endl;
                std::cout << "Best Fitness: " << best_fitness << std::endl;
                std::cout << "best_area:" << seq_to_db_map.find(best_seq)->second.area << std::endl;
                std::cout << "best_delay:" << seq_to_db_map.find(best_seq)->second.delay << std::endl;
                ////得到前一半seq,返回half_seq_to_db_map，这个map是有string以及QoR
                for (const auto &sequence: top_half_algo_sequences) {
//                    std::cout << "half_seq:" << sequence << std::endl;
                    double fitness = seq_to_db_map.find(sequence)->second.fitness;
//                    std::cout << "half_top_fitness:" << fitness << std::endl;
                    fit_area_delay tmp_db = seq_to_db_map.find(sequence)->second;
                    half_seq_to_db_map.emplace(sequence, tmp_db);///将前一半赋给这个map
                    sum_fitness += fitness;
                }
//                std::cout << "sum_fitness:" << sum_fitness << std::endl;
                ////计算所有fitness_prob，存进half_map的prob
                for (const auto &seqToDbMap: half_seq_to_db_map) {
//                    std::cout << "half_seq_db:" << seqToDbMap.first << std::endl;
                    double fitness = seq_to_db_map.find(seqToDbMap.first)->second.fitness;
                    double fit_prob = fitness / sum_fitness;
                    half_seq_to_db_map.find(seqToDbMap.first)->second.fit_prob = fit_prob;
                }
                ////将前一半seq存进下一个总群next_seq_to_db_map
                for (const auto &sequence: half_seq_to_db_map) {
                    next_seq_to_db_map.emplace(sequence.first, sequence.second);
                }
                ////得到交叉或者变异之后的child
                for (uint64_t i = 0; i < (seq_to_db_map.size() - half_seq_to_db_map.size()); ++i) {
                    std::string string_father = ga_select(half_seq_to_db_map);
                    std::vector<std::string> vector_father = string_to_vector(string_father);
                    std::string string_mother = ga_select(half_seq_to_db_map);
                    std::vector<std::string> vector_mother = string_to_vector(string_mother);
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_real_distribution<> dis(0.0, 1.0);
                    double random_num = dis(gen);
                    std::string child = "";
                    std::cout << "father1:" << string_father << std::endl;
                    std::cout << "mother1:" << string_mother << std::endl;

                    int isFather_Equal_to_macro =
                            is_vector_equal(vector_father, macro_1) && is_vector_equal(vector_father, macro_2) &&
                                    is_vector_equal(vector_father, macro_3) && is_vector_equal(vector_father, macro_4) &&
                                    is_vector_equal(vector_father, macro_5);
                    int isMother_Equal_to_macro =
                            is_vector_equal(vector_mother, macro_1) && is_vector_equal(vector_mother, macro_2) &&
                                    is_vector_equal(vector_mother, macro_3) && is_vector_equal(vector_mother, macro_4) &&
                                    is_vector_equal(vector_mother, macro_5);

                    if (isFather_Equal_to_macro && isMother_Equal_to_macro == 1) {

                        std::cout << "cross" << std::endl;
                        child = crossover_op(vector_father, vector_mother);
                        std::cout << "test1" << std::endl;

                    }
                    if (random_num < mutation_probability) {
                        std::cout << "mutation" << std::endl;
                        std::vector<std::string> v_child = string_to_vector(child);
                        std::vector<std::string> mutation_child = mutation(v_child);

//                        std::vector<std::string> algo_sequence = get_random_sequence(strings, algo_num);
                        mutation_child.push_back("map_fpga;");
                        std::string combined_algo_seq_string = std::accumulate(mutation_child.begin(),
                                                                               mutation_child.end(),
                                                                               std::string());
                        child = combined_algo_seq_string;
                    }
                    std::cout << "child1:" << child << std::endl;
                    ////存入下一次种群,得到一个完整的next_seq_to_de_map
                    if (child == string_father || child == string_mother) {
//                        std::cout<<"child == pareants"<<std::endl;
                        std::vector<std::string> algo_sequence = get_random_sequence(strings, algo_num);
                        algo_sequence.push_back("map_fpga;");
                        run_algo_seq(algo_sequence);
                        ////turn vector to string
                        std::string combined_algo_seq_string = std::accumulate(algo_sequence.begin(),
                                                                               algo_sequence.end(),
                                                                               std::string());
                        iFPGA::klut_network klut = store < iFPGA::klut_network > ().current()._storage;
                        iFPGA::depth_view<iFPGA::klut_network> dklut(klut);
                        double current_area = klut.num_gates();
                        double current_delay = dklut.depth();
                        double fitness = fitness_func(current_area, current_delay, no_opt_area, no_opt_delay);
                        child = combined_algo_seq_string;
                        ////复原为原来一开始输入文件的那个结构
                        store < iFPGA::aig_network > ().current() = initial_aig;
                        store < iFPGA::aig_network > ().current()._storage = initial_aig_storage;

                        fit_area_delay tmp_fit_area_delay;
                        tmp_fit_area_delay.fitness = fitness;
                        tmp_fit_area_delay.area = current_area;
                        tmp_fit_area_delay.delay = current_delay;
                        next_seq_to_db_map.emplace(child, tmp_fit_area_delay);
                    } else {
                        std::cout << "child != pareants" << std::endl;
                        std::vector<std::string> vector_child = string_to_vector(child);
                        run_algo_seq(vector_child);
                        iFPGA::klut_network klut = store < iFPGA::klut_network > ().current()._storage;
                        iFPGA::depth_view<iFPGA::klut_network> dklut(klut);
                        double current_area = klut.num_gates();
                        double current_delay = dklut.depth();
                        double fitness = fitness_func(current_area, current_delay, no_opt_area, no_opt_delay);
//                        std::cout << "current_area:" << current_area << std::endl;
//                        std::cout << "current_delay:" << current_delay << std::endl;
//                        std::cout << "fitness:" << fitness << std::endl;
                        ////复原为原来一开始输入文件的那个结构
                        store < iFPGA::aig_network > ().current() = initial_aig;
                        store < iFPGA::aig_network > ().current()._storage = initial_aig_storage;

                        fit_area_delay tmp_fit_area_delay;
                        tmp_fit_area_delay.fitness = fitness;
                        tmp_fit_area_delay.area = current_area;
                        tmp_fit_area_delay.delay = current_delay;
                        next_seq_to_db_map.emplace(child, tmp_fit_area_delay);
                    }
                }
                for (const auto &next_seq: next_seq_to_db_map) {
                    double fitness = next_seq_to_db_map.find(next_seq.first)->second.fitness;
                    sum_fitness += fitness;
                }
                ////算出prob存进next_seq_to_db_map
                for (const auto &next_seq: next_seq_to_db_map) {
                    double fit_prob = next_seq_to_db_map.find(next_seq.first)->second.fitness / sum_fitness;
                    next_seq_to_db_map.find(next_seq.first)->second.fit_prob = fit_prob;
                }
                for (const auto &dbMap: seq_to_db_map) {
                    std::cout << "seq_to_db:" << dbMap.first << std::endl;
                }
                std::cout << " " << std::endl;
                for (const auto &dbMap: next_seq_to_db_map) {
                    std::cout << "next_seq:" << dbMap.first << std::endl;
                }
                // 清空 seq_to_db_map
                seq_to_db_map.clear();
                // 将 next_seq_to_db_map_2 移动到 seq_to_db_map
                seq_to_db_map = std::move(next_seq_to_db_map);
                count += 1;
                std::cout << "count: " << count << std::endl;
                end_time_1 = clock();
                run_time = (double) (end_time_1 - start_time) / CLOCKS_PER_SEC;
                std::cout << "run_time: " << run_time << std::endl;
                if (run_time >= limited_time_1){break;}
            }



            ////第二个阶段初始种群
//            run_time = 0;
            int count2 = 0;
//            start_time = clock();
            for (uint64_t i = 0; i < sequence_num; ++i) {
                std::string best_seq_of_stage_1 = best_seq;
                std::cout << "best_seq_of_1:" << best_seq_of_stage_1 << std::endl;
                std::vector<std::string> vector_best_seq_of_1 = string_to_vector(best_seq_of_stage_1);
                run_algo_seq(vector_best_seq_of_1);
                stage2_initial_aig = store < iFPGA::aig_network > ().current();
                stage2_initial_aig_storage = store < iFPGA::aig_network > ().current()._storage;

                std::vector<std::string> algo_sequence_of_2 = get_random_sequence(strings, algo_num_of_stage_2);
                algo_sequence_of_2.push_back("map_fpga;");
                run_algo_seq(algo_sequence_of_2);
                ////turn vector to string
                std::string combined_algo_seq_string = std::accumulate(algo_sequence_of_2.begin(),
                                                                       algo_sequence_of_2.end(),
                                                                       std::string());

                iFPGA::klut_network klut = store < iFPGA::klut_network > ().current()._storage;
                iFPGA::depth_view<iFPGA::klut_network> dklut(klut);
                double current_area = klut.num_gates();
                double current_delay = dklut.depth();

                double fitness = fitness_func(current_area, current_delay, no_opt_area, no_opt_delay);
                fit_area_delay tmp_fit_area_delay;
                tmp_fit_area_delay.fitness = fitness;
                tmp_fit_area_delay.area = current_area;
                tmp_fit_area_delay.delay = current_delay;
                seq_to_db_map_2.emplace(combined_algo_seq_string, tmp_fit_area_delay);
                ////复原
                store < iFPGA::aig_network > ().current() = stage2_initial_aig;
                store < iFPGA::aig_network > ().current()._storage = stage2_initial_aig_storage;
            }
            for (const auto &item: seq_to_db_map_2) {
                std::cout << "seq2:" << item.first << std::endl;
                std::cout << "seq2 area:" << item.second.area << std::endl;
                std::cout << "seq2 delay:" << item.second.delay << std::endl;
            }
            ////GA第二阶
            for (int i = 0; i < 10; ++i) {
                ////初始化sum_fitness
                sum_fitness = 0;
                double best_fitness_of_2 = 0.0;
//                std::string best_seq{};
                //// 调用 find_top_half_strings 去找到fitness高的前一半，只有算子序列，无QoR
                std::vector<std::string> top_half_algo_sequences = find_top_half_strings(seq_to_db_map_2);
                ////找最好的序列
                for (const auto &seq: seq_to_db_map_2) {
                    std::string current_seq = seq.first;
                    double current_fitness = seq.second.fitness;
                    if (current_fitness > best_fitness_of_2) {
                        best_seq_of_2 = current_seq;
                        best_fitness_of_2 = current_fitness;
                    }
                }
                //// 输出最佳序列和最佳适应度
                std::cout << "Best Sequence2: " << best_seq_of_2 << std::endl;
                std::cout << "Best Fitness2: " << best_fitness_of_2 << std::endl;
                std::cout << "best_area2:" << seq_to_db_map_2.find(best_seq_of_2)->second.area << std::endl;
                std::cout << "best_delay2:" << seq_to_db_map_2.find(best_seq_of_2)->second.delay << std::endl;
                ////得到前一半seq,half_seq_to_db_map_2，这个map是有string以及QoR
                for (const auto &sequence: top_half_algo_sequences) {
//                    std::cout << "half_seq:" << sequence << std::endl;
                    double fitness = seq_to_db_map_2.find(sequence)->second.fitness;
//                    std::cout << "half_top_fitness:" << fitness << std::endl;
                    fit_area_delay tmp_db = seq_to_db_map_2.find(sequence)->second;
                    half_seq_to_db_map_2.emplace(sequence, tmp_db);///将前一半赋给这个map
                    sum_fitness += fitness;
                }
//                std::cout << "sum_fitness:" << sum_fitness << std::endl;
                ////计算所有fitness_prob，存进half_map的prob
                for (const auto &seqToDbMap: half_seq_to_db_map_2) {
//                    std::cout << "half_seq_db:" << seqToDbMap.first << std::endl;
                    double fitness = seq_to_db_map_2.find(seqToDbMap.first)->second.fitness;
                    double fit_prob = fitness / sum_fitness;
                    half_seq_to_db_map_2.find(seqToDbMap.first)->second.fit_prob = fit_prob;
                }
                ////将前一半seq存进下一个总群next_seq_to_db_map
                for (const auto &sequence: half_seq_to_db_map_2) {
                    next_seq_to_db_map_2.emplace(sequence.first, sequence.second);
                }

                ////得到交叉或者变异之后的child
                for (uint64_t i = 0; i < (seq_to_db_map_2.size() - half_seq_to_db_map_2.size()); ++i) {
                    std::string string_father = ga_select(half_seq_to_db_map_2);
                    std::vector<std::string> vector_father = string_to_vector(string_father);
                    std::string string_mother = ga_select(half_seq_to_db_map_2);
                    std::vector<std::string> vector_mother = string_to_vector(string_mother);
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_real_distribution<> dis(0.0, 1.0);
                    double random_num = dis(gen);
                    std::string child = "";
                    std::cout << "father2:" << string_father << std::endl;
                    std::cout << "mother2:" << string_mother << std::endl;

                    int isFather_Equal_to_macro =
                            is_vector_equal(vector_father, macro_1) && is_vector_equal(vector_father, macro_2) &&
                                    is_vector_equal(vector_father, macro_3) && is_vector_equal(vector_father, macro_4) &&
                                    is_vector_equal(vector_father, macro_5);
                    int isMother_Equal_to_macro =
                            is_vector_equal(vector_mother, macro_1) && is_vector_equal(vector_mother, macro_2) &&
                                    is_vector_equal(vector_mother, macro_3) && is_vector_equal(vector_mother, macro_4) &&
                                    is_vector_equal(vector_mother, macro_5);

                    if (isFather_Equal_to_macro && isMother_Equal_to_macro == true) {
                        std::cout << "cross" << std::endl;
                        child = crossover_op(vector_father, vector_mother);
                        std::cout << "test" << std::endl;
                    }
                    if (random_num < mutation_probability) {
                        std::cout << "mutation" << std::endl;
                        std::vector<std::string> v_child = string_to_vector(child);
                        std::vector<std::string> mutation_child = mutation(v_child);

//                        std::vector<std::string> algo_sequence = get_random_sequence(strings, algo_num);
                        mutation_child.push_back("map_fpga;");
                        std::string combined_algo_seq_string = std::accumulate(mutation_child.begin(),
                                                                               mutation_child.end(),
                                                                               std::string());
                        child = combined_algo_seq_string;
                    }
                    std::cout << "child2:" << child << std::endl;
                    ////存入下一次种群,得到一个完整的next_seq_to_de_map
                    if (child == string_father || child == string_mother) {
//                        std::cout<<"child == pareants"<<std::endl;
                        std::vector<std::string> algo_sequence = get_random_sequence(strings, algo_num_of_stage_2);
                        algo_sequence.push_back("map_fpga;");
                        run_algo_seq(algo_sequence);
                        ////turn vector to string
                        std::string combined_algo_seq_string = std::accumulate(algo_sequence.begin(),
                                                                               algo_sequence.end(),
                                                                               std::string());
                        iFPGA::klut_network klut = store < iFPGA::klut_network > ().current()._storage;
                        iFPGA::depth_view<iFPGA::klut_network> dklut(klut);
                        double current_area = klut.num_gates();
                        double current_delay = dklut.depth();
                        double fitness = fitness_func(current_area, current_delay, no_opt_area, no_opt_delay);
                        child = combined_algo_seq_string;
                        ////复原为原来一开始输入文件的那个结构
                        store < iFPGA::aig_network > ().current() = stage2_initial_aig;
                        store < iFPGA::aig_network > ().current()._storage = stage2_initial_aig_storage;

                        fit_area_delay tmp_fit_area_delay;
                        tmp_fit_area_delay.fitness = fitness;
                        tmp_fit_area_delay.area = current_area;
                        tmp_fit_area_delay.delay = current_delay;
                        next_seq_to_db_map_2.emplace(child, tmp_fit_area_delay);
                    } else {
                        std::cout << "child != pareants" << std::endl;
                        std::vector<std::string> vector_child = string_to_vector(child);
                        run_algo_seq(vector_child);
                        iFPGA::klut_network klut = store < iFPGA::klut_network > ().current()._storage;
                        iFPGA::depth_view<iFPGA::klut_network> dklut(klut);
                        double current_area = klut.num_gates();
                        double current_delay = dklut.depth();
                        double fitness = fitness_func(current_area, current_delay, no_opt_area, no_opt_delay);

                        ////复原为原来一开始输入文件的那个结构
                        store < iFPGA::aig_network > ().current() = stage2_initial_aig;
                        store < iFPGA::aig_network > ().current()._storage = stage2_initial_aig_storage;

                        fit_area_delay tmp_fit_area_delay;
                        tmp_fit_area_delay.fitness = fitness;
                        tmp_fit_area_delay.area = current_area;
                        tmp_fit_area_delay.delay = current_delay;
                        next_seq_to_db_map_2.emplace(child, tmp_fit_area_delay);
                    }
                }
                for (const auto &next_seq: next_seq_to_db_map_2) {
                    double fitness = next_seq_to_db_map_2.find(next_seq.first)->second.fitness;
                    sum_fitness += fitness;
                }
                ////算出prob存进next_seq_to_db_map
                for (const auto &next_seq: next_seq_to_db_map_2) {
                    double fit_prob = next_seq_to_db_map_2.find(next_seq.first)->second.fitness / sum_fitness;
                    next_seq_to_db_map_2.find(next_seq.first)->second.fit_prob = fit_prob;
                }
                for (const auto &dbMap: seq_to_db_map_2) {
                    std::cout << "seq_to_db:" << dbMap.first << std::endl;
                }
                std::cout << " " << std::endl;
                for (const auto &dbMap: next_seq_to_db_map_2) {
                    std::cout << "next_seq:" << dbMap.first << std::endl;
                }
                // 清空 seq_to_db_map_2
                seq_to_db_map_2.clear();
                // 将 next_seq_to_db_map_2 移动到 seq_to_db_map
                seq_to_db_map_2 = std::move(next_seq_to_db_map_2);

                count2 += 1;
                std::cout << "count2: " << count2 << std::endl;

                end_time_2 = clock();
                run_time_of_stage2 = (double) (end_time_2 - start_time) / CLOCKS_PER_SEC;
                std::cout << "run_time_of_stage2: " << run_time_of_stage2 << std::endl;
                if (run_time >= limited_time_2){break;}
            }

            store < iFPGA::aig_network > ().current() = initial_aig;
            std::vector<std::string> vector_best_seq_of_1 = string_to_vector(best_seq);
            std::vector<std::string> vector_best_seq_of_2 = string_to_vector(best_seq_of_2);
            vector_best_seq_of_1.pop_back();


            for (const auto &bestSeqOf1: vector_best_seq_of_1) {
                std::cout << "test1!!" << std::endl;
                std::cout << bestSeqOf1;
            }
            std::cout << "state_1_best_seq:" << best_seq << std::endl;
            std::cout << "state_2_best_seq: " << best_seq_of_2 << std::endl;
            for (const auto &item: vector_best_seq_of_2) {
                vector_best_seq_of_1.push_back(item);
            }
            run_algo_seq(vector_best_seq_of_1);
            iFPGA::klut_network klut = store < iFPGA::klut_network > ().current()._storage;
            iFPGA::depth_view<iFPGA::klut_network> dklut(klut);
            double current_area = klut.num_gates();
            double current_delay = dklut.depth();
            std::cout << "no_opt_area: " << no_opt_area << std::endl;
            std::cout << "no_opt_delay: " << no_opt_delay << std::endl;
            std::cout << "best_area: " << current_area << std::endl;
            std::cout << "best_delay: " << current_delay << std::endl;
            std::cout << "best_seq:" << std::endl;
            for (const auto &bestSeqOf1: vector_best_seq_of_1) {
                std::cout << bestSeqOf1;
            }


            std::ofstream output(outfile_path);
            if (output.is_open()) {
                for (const auto &bestSeqOf1: vector_best_seq_of_1) {
                    output << bestSeqOf1;
                }
                for (const auto &item: vector_best_seq_of_2) {
                    output << item;
                }
                output.close();
            }
        }

    private:
        std::string outfile_path = "";
    };

    ALICE_ADD_COMMAND(ga,
    "Genetic Algorithm");
};