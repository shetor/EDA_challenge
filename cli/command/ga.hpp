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
#include <chrono>

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
                if (str == "lut_opt;"){
                    uint32_t cut_size = 6u;
                    uint32_t priority_size = 10u;
                    uint32_t iFlowIter = 1;
                    uint32_t iAreaIter = 2;
                    bool zero_gain = false;
                    bool verbose = false;
                    iFPGA::aig_network aig = store<iFPGA::aig_network>().current();
                    iFPGA::aig_with_choice awc(aig);
                    iFPGA::mapping_view<iFPGA::aig_with_choice, true, false> mapped_aig(awc);
                    iFPGA::klut_mapping_params params;
                    params.cut_enumeration_ps.cut_size = cut_size;
                    params.cut_enumeration_ps.cut_limit = priority_size;
                    params.uFlowIters = iFlowIter;
                    params.uAreaIters = iAreaIter;
                    params.bZeroGain = zero_gain;
                    params.verbose = verbose;
                    iFPGA_NAMESPACE::klut_mapping<decltype(mapped_aig), true>(mapped_aig, params);
                    const auto kluts = *iFPGA_NAMESPACE::choice_to_klut<iFPGA_NAMESPACE::klut_network>( mapped_aig );
                    iFPGA::aig_network res = iFPGA::convert_klut_to_aig( kluts );
                    store<iFPGA::aig_network>().current() = res;
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
            auto start = std::chrono::steady_clock::now();
            iFPGA::aig_network initial_aig = store<iFPGA::aig_network>().current();
            std::shared_ptr < iFPGA::storage < iFPGA::fixed_node < 2, 2 >,
                    iFPGA::aig_storage_data >> initial_aig_storage =
                            store<iFPGA::aig_network>().current()._storage;
            iFPGA::aig_network stage2_initial_aig;
            std::shared_ptr < iFPGA::storage < iFPGA::fixed_node < 2, 2 >, iFPGA::aig_storage_data
                    >> stage2_initial_aig_storage;

            iFPGA::klut_mapping_params param_mapping;
            param_mapping.cut_enumeration_ps.cut_size = 6u;
            param_mapping.cut_enumeration_ps.cut_limit = 10u;
            param_mapping.uFlowIters = 1;
            param_mapping.uAreaIters = 2;
            param_mapping.verbose = false;
            iFPGA::aig_with_choice restore_awc_1(initial_aig);
            iFPGA::mapping_view<iFPGA::aig_with_choice, true, false> restore_mapped_aig_1(restore_awc_1);
            iFPGA_NAMESPACE::klut_mapping<decltype(restore_mapped_aig_1), true>(restore_mapped_aig_1, param_mapping);
            const auto initial_kluts_1 = *iFPGA_NAMESPACE::choice_to_klut<iFPGA_NAMESPACE::klut_network>(
                    restore_mapped_aig_1);
            int node_num = initial_aig.num_gates();

            //判断case大小来决定运行时间
            int limited_second_1;
            int limited_second_2;
            if (node_num <= 1000) {
                limited_second_1 = 30;
                limited_second_2 = 58;
            } else if (node_num > 1000 && node_num <= 10000) {
                limited_second_1 = 300;
                limited_second_2 = 590;
            } else {
                limited_second_1 = 1800;
                limited_second_2 = 3590;
            }
            std::chrono::duration<int, std::ratio<1>> limited_time_1(limited_second_1);
            std::chrono::duration<int, std::ratio<1>> limited_time_2(limited_second_2);

            ////map_fpga
            if (store<iFPGA::klut_network>().empty()) {
                store<iFPGA::klut_network>().extend();
            }
            iFPGA::aig_with_choice restore_awc_2(initial_aig);
            iFPGA::mapping_view<iFPGA::aig_with_choice, true, false> restore_mapped_aig_2(restore_awc_2);
            iFPGA_NAMESPACE::klut_mapping<decltype(restore_mapped_aig_2), true>(restore_mapped_aig_2, param_mapping);
            const auto initial_kluts_2 = *iFPGA_NAMESPACE::choice_to_klut<iFPGA_NAMESPACE::klut_network>(
                    restore_mapped_aig_2);
            store<iFPGA::klut_network>().current() = initial_kluts_2;

            iFPGA::klut_network initial_klut = store<iFPGA::klut_network>().current()._storage;
            iFPGA::depth_view <iFPGA::klut_network> initial_dklut(initial_klut);
            double no_opt_area = initial_klut.num_gates();
            double no_opt_delay = initial_dklut.depth();
            double sum_normal_fitness = 0;

            ////复原
            store<iFPGA::aig_network>().current() = initial_aig;
            store<iFPGA::aig_network>().current()._storage = initial_aig_storage;
//            iFPGA::aig_with_choice initial_awc(store<iFPGA::aig_network>().current());
//            iFPGA::mapping_view<iFPGA::aig_with_choice, true, false> mapped_aig(initial_awc);

//            iFPGA_NAMESPACE::klut_mapping<decltype(mapped_aig), true>(mapped_aig, param_mapping);
//            const auto initial_kluts = *iFPGA_NAMESPACE::choice_to_klut<iFPGA_NAMESPACE::klut_network>( mapped_aig );
//            store<iFPGA::klut_network>().current() = initial_kluts;

            std::unordered_map <std::string, fit_area_delay> seq_to_db_map{};
            std::unordered_map <std::string, fit_area_delay> seq_to_db_map_2{};
            std::unordered_map <std::string, fit_area_delay> better_seq_to_db_map{};
            std::unordered_map <std::string, fit_area_delay> better_seq_to_db_map_2{};
            std::vector <std::string> current_population_v;
            std::vector <std::string> current_population_v_2;
            std::vector <std::string> next_population_v;
            std::vector <std::string> next_population_v_2;
            bool continue_not_opt_flag = true;
            uint64_t algo_num = 5;
            uint64_t algo_num_of_stage_2 = 10;
            uint64_t sequence_num = 10;
            std::vector <std::string> strings = {"balance;", "rewrite;", "rewrite -z;", "rewrite -l;", "refactor;",
                                                 "refactor -z;", "refactor -v;", "refactor -l;"};
            std::vector <std::string> add_lut_strings = {"balance;", "rewrite;", "rewrite -z;", "rewrite -l;", "refactor;",
                                                 "refactor -z;", "refactor -v;", "refactor -l;","lut_opt;"};
            std::vector <std::string> macro_1 = {"balance;", "rewrite;", "rewrite -z;", "balance;", "rewrite -z;",
                                                 "balance;"};
            std::vector <std::string> macro_2 = {
                    "balance;", "rewrite;", "refactor;", "balance;", "rewrite;", "rewrite -z;", "balance;",
                    "refactor -z;", "rewrite -z;", "balance;"};
            std::vector <std::string> macro_3 = {
                    "balance;", "rewrite;", "balance;", "rewrite;", "rewrite -z;", "balance;", "rewrite -z;",
                    "balance;"};
            std::vector <std::string> macro_4 = {"balance;", "rewrite -l;", "rewrite -z -l;", "rewrite -z -l;",
                                                 "balance;"};
            std::vector <std::string> macro_5 = {
                    "balance;", "rewrite -l;", "refactor -l;", "balance;", "rewrite -z -l;", "balance;", "refactor -z;",
                    "rewrite -z;", "balance;"};

            std::string best_seq{};
            std::string best_seq_of_2{};
            ////自适应
            double K1 = 0.6;
            double K2 = 0.5;   ////K2>K3
            double K3 = 0.4;
            double K4 = 0.5;
            double K5 = 0.4;  ////K5>K6
            double K6 = 0.3;
            double mutation_probability = 0;
            double cross_probability = 0;

            int count = 0;
            ////生成初始序列,返回seq_to_db_map
            for (uint64_t i = 0; i < sequence_num; ++i) {
                if (seq_to_db_map.size()==3){
                    double first_area = seq_to_db_map.begin()->second.area;
                    double first_delay = seq_to_db_map.begin()->second.delay;
                    for (const auto &dbMap: seq_to_db_map) {
                        if (dbMap.second.area!=first_area||dbMap.second.delay!=first_delay){
                            continue_not_opt_flag = false;
                            break;
                        }
                    }
                }
                std::vector <std::string> algo_sequence;
                if (continue_not_opt_flag ){
                    algo_sequence = get_random_add_lut_sequence(add_lut_strings, algo_num);
                }
                else{
                    algo_sequence = get_random_sequence(strings, algo_num);
                }
                run_algo_seq(algo_sequence);
                ////turn vector to string
                std::string combined_algo_seq_string = std::accumulate(algo_sequence.begin(), algo_sequence.end(),
                                                                       std::string());
                current_population_v.push_back(combined_algo_seq_string);
//                std::cout<<"Combined String:"<<combined_algo_seq_string<<std::endl;
                iFPGA::klut_network klut = store<iFPGA::klut_network>().current()._storage;
                iFPGA::depth_view <iFPGA::klut_network> dklut(klut);
                double current_area = klut.num_gates();
                double current_delay = dklut.depth();
                double fitness = fitness_func(current_area, current_delay, no_opt_area, no_opt_delay);

                fit_area_delay tmp_fit_area_delay;
                tmp_fit_area_delay.fitness = fitness;
                tmp_fit_area_delay.area = current_area;
                tmp_fit_area_delay.delay = current_delay;
                seq_to_db_map.emplace(combined_algo_seq_string, tmp_fit_area_delay);

                ////复原为原来一开始输入文件的那个结构
                store<iFPGA::aig_network>().current() = initial_aig;
                store<iFPGA::aig_network>().current()._storage = initial_aig_storage;
                iFPGA::aig_with_choice restore_awc_3(initial_aig);
                iFPGA::mapping_view<iFPGA::aig_with_choice, true, false> restore_mapped_aig_3(restore_awc_3);
                iFPGA_NAMESPACE::klut_mapping<decltype(restore_mapped_aig_3), true>(restore_mapped_aig_3,
                                                                                    param_mapping);
                const auto initial_kluts_3 = *iFPGA_NAMESPACE::choice_to_klut<iFPGA_NAMESPACE::klut_network>(
                        restore_mapped_aig_3);
                store<iFPGA::klut_network>().current() = initial_kluts_3;
                iFPGA::klut_network initial_klut_1 = store<iFPGA::klut_network>().current()._storage;
                iFPGA::depth_view <iFPGA::klut_network> initial_dklut_1(initial_klut_1);
            }

            ////迭代GA 第一次
            while (1) {
                sum_normal_fitness = 0.0;
                ////归一前的最大最小fitness
                auto initial_find_max_fitness = std::max_element(seq_to_db_map.begin(), seq_to_db_map.end(),
                                                                 [](const auto &pair1, const auto &pair2) {
                                                                     return pair1.second.fitness < pair2.second.fitness;
                                                                 });
                auto initial_find_min_fitness = std::min_element(seq_to_db_map.begin(), seq_to_db_map.end(),
                                                                 [](const auto &pair1, const auto &pair2) {
                                                                     return pair1.second.fitness <
                                                                            pair2.second.fitness;;
                                                                 });
                double initial_max_fitness = initial_find_max_fitness->second.fitness;
                double initial_min_fitness = initial_find_min_fitness->second.fitness;
//                std::cout << "initial_max_fitness: " << initial_max_fitness << std::endl;
//                std::cout << "initial_min_fitness: " << initial_min_fitness << std::endl;
                ////归一
                for (auto &item: seq_to_db_map) {
                    double current_fit = item.second.fitness;
//                    std::cout<<"current_fit:"<<current_fit<<std::endl;
//                    std::cout<<"initial_min_fitness:"<<initial_min_fitness<<std::endl;
//                    std::cout<<"initial_max_fitness:"<<initial_max_fitness<<std::endl;
                    double norm_fitness =
                            (current_fit - initial_min_fitness) / (initial_max_fitness - initial_min_fitness);
                    item.second.norm_fitness = norm_fitness;
                    sum_normal_fitness += norm_fitness;
                }
//                std::cout<<"sum_normal_fitness: "<<sum_normal_fitness<<std::endl;
                ////计算所有fitness_prob，存进seq_map的prob
                for (const auto &dbMap: seq_to_db_map) {
                    double norm_fitness = seq_to_db_map.find(dbMap.first)->second.norm_fitness;
                    double fit_prob = norm_fitness / sum_normal_fitness;
//                    std::cout<<"norm_fitness: "<<norm_fitness<<std::endl;
//                    std::cout<<"sum_mormal_fitness: "<<sum_normal_fitness<<std::endl;
//                    std::cout<<"sum_mormal_fitness_prob: "<<fit_prob<<std::endl;
                    seq_to_db_map.find(dbMap.first)->second.fit_prob = fit_prob;
                }
                ////归一后的最大最小fitness
                auto norm_find_max_fitness = std::max_element(seq_to_db_map.begin(), seq_to_db_map.end(), [](const auto& pair1, const auto& pair2){
                    return pair1.second.fitness < pair2.second.fitness;
                });
                auto norm_find_min_fitness = std::min_element(seq_to_db_map.begin(), seq_to_db_map.end(), [](const auto& pair1, const auto& pair2){
                    return pair1.second.fitness < pair2.second.fitness;;
                });

                double max_fitness = norm_find_max_fitness->second.norm_fitness;
                double min_fitness = norm_find_min_fitness->second.norm_fitness;
                //归一化后测试
//                for (const auto &item: seq_to_db_map) {
//                    std::cout<<"after  fit:"<<item.second.fitness<<std::endl;
//                    std::cout<<"after norm fit:"<<item.second.norm_fitness<<std::endl;
//                }

                for (const auto &populationV: current_population_v) {
                    std::cout << "current_population:" << populationV << std::endl;
                }
                std::cout << "seq_to_db_map.size:" << seq_to_db_map.size() << std::endl;
                for (const auto &dbMap: seq_to_db_map) {
                    std::cout << "seq:" << dbMap.first << std::endl;
                    std::cout << "seq_area:" << dbMap.second.area << std::endl;
                    std::cout << "seq_delay:" << dbMap.second.delay << std::endl;
                    std::cout << "seq_fitness:" << dbMap.second.fitness << std::endl;
                    std::cout << "seq_fitness_prob:" << dbMap.second.fit_prob << std::endl;
                }
                ////初始化sum_fitness
                sum_normal_fitness = 0;
                double best_fitness = 0.0;
                //// 调用 find_top_better_strings 去找到fitness高的前3个，只有算子序列，无QoR
                std::vector <std::string> top_better_algo_sequences = find_top_better_strings(seq_to_db_map);
                better_seq_to_db_map.clear();
                for (const auto &sequence: top_better_algo_sequences) {
                    fit_area_delay tmp_fit_area_delay;
                    tmp_fit_area_delay = seq_to_db_map.find(sequence)->second;
                    better_seq_to_db_map.emplace(sequence, tmp_fit_area_delay);
                    next_population_v.push_back(sequence);
                }
                ////得到交叉或者变异之后的child
                for (uint64_t i = 0; i < (current_population_v.size() - top_better_algo_sequences.size()); ++i) {
                    std::string string_father = ga_select(seq_to_db_map, current_population_v);
                    std::vector <std::string> vector_father = string_to_vector(string_father);
                    std::string string_mother = ga_select(seq_to_db_map, current_population_v);
                    std::vector <std::string> vector_mother = string_to_vector(string_mother);
                    std::cout << "father1:" << string_father << std::endl;
                    std::cout << "mother1:" << string_mother << std::endl;
                    ////自适应
                    double father_fitness = 0;
                    double mother_fitness = 0;
                    for (const auto &item: seq_to_db_map) {
                        if (string_father == item.first) {
                            father_fitness = item.second.fitness;
                        }
                    }
//                    std::cout << "father_fitness:" << father_fitness << std::endl;
                    for (const auto &item: seq_to_db_map) {
                        if (string_mother == item.first) {
                            mother_fitness = item.second.fitness;
                        }
                    }
//                    std::cout << "mother_fitness:" << mother_fitness << std::endl;
                    double max_fitness_of_parents = std::max(father_fitness, mother_fitness);
                    if (max_fitness_of_parents != min_fitness && max_fitness_of_parents != max_fitness) {
                        cross_probability = K1 * ((max_fitness - max_fitness_of_parents) / (max_fitness - min_fitness));
                        mutation_probability =
                                K4 * ((max_fitness - max_fitness_of_parents) / (max_fitness - min_fitness));
                    } else if (max_fitness_of_parents == min_fitness) {
                        cross_probability = K2;
                        mutation_probability = K5;
                    } else if (max_fitness_of_parents == max_fitness) {
                        cross_probability = K3;
                        mutation_probability = K6;
                    }
                    std::cout << "cross_probability:" << cross_probability << std::endl;
                    std::cout << "mutation_probability:" << mutation_probability << std::endl;

                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_real_distribution<> dis(0.0, 1.0);
                    double random_num = dis(gen);
                    std::string child = "";
                    vector_father.pop_back();
                    vector_mother.pop_back();
                    bool is_not_father_equal_to_macro =
                            is_not_vector_equal(vector_father, macro_1) &&
                            is_not_vector_equal(vector_father, macro_2) &&
                            is_not_vector_equal(vector_father, macro_3) &&
                            is_not_vector_equal(vector_father, macro_4) &&
                            is_not_vector_equal(vector_father, macro_5);
                    bool is_not_mother_equal_to_macro =
                            is_not_vector_equal(vector_mother, macro_1) &&
                            is_not_vector_equal(vector_mother, macro_2) &&
                            is_not_vector_equal(vector_mother, macro_3) &&
                            is_not_vector_equal(vector_mother, macro_4) &&
                            is_not_vector_equal(vector_mother, macro_5);
                    vector_father.push_back("map_fpga;");
                    vector_mother.push_back("map_fpga;");


                    std::cout << "is_not_father_equal_to_macro:" << is_not_father_equal_to_macro << std::endl;
                    std::cout << "is_not_mother_equal_to_macro:" << is_not_mother_equal_to_macro << std::endl;
                    if (is_not_father_equal_to_macro && is_not_mother_equal_to_macro &&
                        random_num < cross_probability) {
                        child = crossover_op(vector_father, vector_mother);
                    } else {
                        std::vector <std::string> child_temp;
                        if (continue_not_opt_flag){
                            child_temp = get_random_add_lut_sequence(add_lut_strings, algo_num);
                        }
                        else{
                            child_temp = get_random_sequence(strings, algo_num);
                        }
                        std::string combined_algo_seq_string = std::accumulate(child_temp.begin(),
                                                                               child_temp.end(),
                                                                               std::string());
                        child = combined_algo_seq_string;
                        std::cout << "child_2:" << child << std::endl;
                    }
                    if (random_num < mutation_probability) {
                        std::cout << "mutation" << std::endl;
                        std::vector <std::string> v_child = string_to_vector(child);
                        v_child.pop_back();
                        std::vector <std::string> mutation_child = mutation(v_child);
//                        mutation_child.push_back("map_fpga;");
                        std::string combined_algo_seq_string = std::accumulate(mutation_child.begin(),
                                                                               mutation_child.end(),
                                                                               std::string());
                        child = combined_algo_seq_string;
                    }
                    std::cout << "child1:" << child << std::endl;
                    if (child == string_father || child == string_mother) {
                        std::cout << "child == parents" << std::endl;
                        std::vector <std::string> algo_sequence;
                        if (continue_not_opt_flag){
                            algo_sequence = get_random_add_lut_sequence(add_lut_strings, algo_num);
                        }else{
                            algo_sequence = get_random_sequence(strings, algo_num);

                        }


                        ////turn vector to string
                        std::string same_pare_child = std::accumulate(algo_sequence.begin(),
                                                                      algo_sequence.end(),
                                                                      std::string());
                        next_population_v.push_back(same_pare_child);

                    } else {
                        next_population_v.push_back(child);
                    }
                }


                // 清空 seq_to_db_map
                sum_normal_fitness = 0;
                seq_to_db_map.clear();
                std::cout << "seq_map_clear_size:" << seq_to_db_map.size() << std::endl;
                for (const auto &dbMap: better_seq_to_db_map) {
                    seq_to_db_map.emplace(dbMap.first, dbMap.second);
                }
                for (const auto &next_population_string: next_population_v) {
                    if (better_seq_to_db_map.count(next_population_string) > 0) {
                        continue;
                    }
                    std::vector <std::string> v_tmp_item = string_to_vector(next_population_string);
                    run_algo_seq(v_tmp_item);
                    iFPGA::klut_network klut = store<iFPGA::klut_network>().current()._storage;
                    iFPGA::depth_view <iFPGA::klut_network> dklut(klut);
                    double current_area = klut.num_gates();
                    double current_delay = dklut.depth();
                    double fitness = fitness_func(current_area, current_delay, no_opt_area, no_opt_delay);
                    ////复原为原来一开始输入文件的那个结构
                    store<iFPGA::aig_network>().current() = initial_aig;
                    store<iFPGA::aig_network>().current()._storage = initial_aig_storage;
                    store<iFPGA::aig_network>().current() = initial_aig;
                    store<iFPGA::aig_network>().current()._storage = initial_aig_storage;
                    iFPGA::aig_with_choice restore_awc_4(initial_aig);
                    iFPGA::mapping_view<iFPGA::aig_with_choice, true, false> restore_mapped_aig_4(restore_awc_4);
                    iFPGA_NAMESPACE::klut_mapping<decltype(restore_mapped_aig_4), true>(restore_mapped_aig_4,
                                                                                        param_mapping);
                    const auto initial_kluts_4 = *iFPGA_NAMESPACE::choice_to_klut<iFPGA_NAMESPACE::klut_network>(
                            restore_mapped_aig_4);
                    store<iFPGA::klut_network>().current() = initial_kluts_4;

                    fit_area_delay tmp_fit_area_delay;
                    tmp_fit_area_delay.fitness = fitness;
                    tmp_fit_area_delay.area = current_area;
                    tmp_fit_area_delay.delay = current_delay;
                    ///找第一阶段fitness最大值
                    for (const auto &dbMap: better_seq_to_db_map) {
                        std::string current_seq = dbMap.first;
                        double current_fitness = dbMap.second.fitness;
                        if (current_fitness >= best_fitness) {
                            best_seq = current_seq;
                            best_fitness = current_fitness;
                        }
                    }
                    if (fitness > best_fitness) {
                        best_seq = next_population_string;
                        best_fitness = fitness;
                    }

                    seq_to_db_map.emplace(next_population_string, tmp_fit_area_delay);
                }
                //// 输出最佳序列和最佳适应度
                std::cout << "Best Sequence: " << best_seq << std::endl;
                std::cout << "Best Fitness: " << best_fitness << std::endl;
                std::cout << "best_area:" << seq_to_db_map.find(best_seq)->second.area << std::endl;
                std::cout << "best_delay:" << seq_to_db_map.find(best_seq)->second.delay << std::endl;
                std::cout << "best_fitness:" << seq_to_db_map.find(best_seq)->second.fitness << std::endl;

                current_population_v.clear();  // 清空 current_population_v
                current_population_v = next_population_v;
                next_population_v.clear();


                count += 1;
                std::cout << "count: " << count << std::endl;
//                end_time_1 = clock();
                auto end = std::chrono::steady_clock::now();
                std::chrono::duration<double> run_time_1 = end - start;
                std::cout << "run_time: " << run_time_1.count() << std::endl;
                if (run_time_1.count() >= limited_time_1.count()) {
                    break;
                }
            }
            ////第二个阶段初始种群
            int count2 = 0;
            sum_normal_fitness = 0;

            std::vector <std::string> v_best_seq_1 = string_to_vector(best_seq);
            v_best_seq_1.pop_back();
            std::cout << "best_seq_of_1:" << best_seq << std::endl;
            run_algo_seq(v_best_seq_1);
            stage2_initial_aig = store<iFPGA::aig_network>().current();
            stage2_initial_aig_storage = store<iFPGA::aig_network>().current()._storage;
            for (uint64_t i = 0; i < sequence_num; ++i) {
                std::vector <std::string> algo_sequence_of_2;
                if (continue_not_opt_flag){
                    algo_sequence_of_2 = get_random_add_lut_sequence(add_lut_strings, algo_num_of_stage_2);
                }
                else{
                    algo_sequence_of_2 = get_random_sequence(strings, algo_num_of_stage_2);
                }
                run_algo_seq(algo_sequence_of_2);
                ////turn vector to string
                std::string combined_algo_seq_string = std::accumulate(algo_sequence_of_2.begin(),
                                                                       algo_sequence_of_2.end(),
                                                                       std::string());
                current_population_v_2.push_back(combined_algo_seq_string);

                iFPGA::klut_network klut = store<iFPGA::klut_network>().current()._storage;
                iFPGA::depth_view <iFPGA::klut_network> dklut(klut);
                double current_area = klut.num_gates();
                double current_delay = dklut.depth();

                double fitness = fitness_func(current_area, current_delay, no_opt_area, no_opt_delay);
                fit_area_delay tmp_fit_area_delay;
                tmp_fit_area_delay.fitness = fitness;
                tmp_fit_area_delay.area = current_area;
                tmp_fit_area_delay.delay = current_delay;
                seq_to_db_map_2.emplace(combined_algo_seq_string, tmp_fit_area_delay);
                ////复原
                store<iFPGA::aig_network>().current() = stage2_initial_aig;
                store<iFPGA::aig_network>().current()._storage = stage2_initial_aig_storage;

                iFPGA::aig_with_choice restore_awc_5(stage2_initial_aig);
                iFPGA::mapping_view<iFPGA::aig_with_choice, true, false> restore_mapped_aig_5(restore_awc_5);
                iFPGA_NAMESPACE::klut_mapping<decltype(restore_mapped_aig_5), true>(restore_mapped_aig_5,
                                                                                    param_mapping);
                const auto initial_kluts_5 = *iFPGA_NAMESPACE::choice_to_klut<iFPGA_NAMESPACE::klut_network>(
                        restore_mapped_aig_5);
                store<iFPGA::klut_network>().current() = initial_kluts_5;

            }
            ////GA第二阶
            while (1) {
                ////初始化sum_fitness
                sum_normal_fitness = 0;
                double best_fitness_of_2 = 0.0;
//                std::string best_seq{};
                ////归一前最大最小fitness
                auto initial_find_max_fitness_2 = std::max_element(seq_to_db_map_2.begin(), seq_to_db_map_2.end(),
                                                                   [](const auto &pair1, const auto &pair2) {
                                                                       return pair1.second.fitness <
                                                                              pair2.second.fitness;
                                                                   });
                auto initial_find_min_fitness_2 = std::min_element(seq_to_db_map_2.begin(), seq_to_db_map_2.end(),
                                                                   [](const auto &pair1, const auto &pair2) {
                                                                       return pair1.second.fitness <
                                                                              pair2.second.fitness;;
                                                                   });
                double initial_max_fitness_2 = initial_find_max_fitness_2->second.fitness;
                double initial_min_fitness_2 = initial_find_min_fitness_2->second.fitness;
                ////归一化fitness
                for (auto &item: seq_to_db_map_2) {
                    double current_fit = item.second.fitness;
                    double norm_fitness =
                            (current_fit - initial_min_fitness_2) / (initial_max_fitness_2 - initial_min_fitness_2);
                    item.second.norm_fitness = norm_fitness;
                    sum_normal_fitness += norm_fitness;
                }
                for (const auto &toDbMap2: seq_to_db_map_2) {
                    double norm_fitness = seq_to_db_map_2.find(toDbMap2.first)->second.norm_fitness;
                    double fit_prob = norm_fitness / sum_normal_fitness;
                    seq_to_db_map_2.find(toDbMap2.first)->second.fit_prob = fit_prob;
                }
                ////归一后最大最小fitness
                auto norm_find_max_fitness_2 = std::max_element(seq_to_db_map_2.begin(), seq_to_db_map_2.end(),
                                                                [](const auto &pair1, const auto &pair2) {
                                                                    return pair1.second.fitness < pair2.second.fitness;
                                                                });
                auto norm_find_min_fitness_2 = std::min_element(seq_to_db_map_2.begin(), seq_to_db_map_2.end(),
                                                                [](const auto &pair1, const auto &pair2) {
                                                                    return pair1.second.fitness < pair2.second.fitness;;
                                                                });
                double max_fitness_2 = norm_find_max_fitness_2->second.fitness;
                double min_fitness_2 = norm_find_min_fitness_2->second.fitness;

                for (const auto &toDbMap2: seq_to_db_map_2) {
                    std::cout << "seq_2_string:" << toDbMap2.first << std::endl;
                    std::cout << "seq_2_area:" << toDbMap2.second.area << std::endl;
                    std::cout << "seq_2_delay:" << toDbMap2.second.delay << std::endl;
                    std::cout << "seq_2_fitness:" << toDbMap2.second.fitness << std::endl;
                    std::cout << "seq_2_fitness_norm:" << toDbMap2.second.norm_fitness << std::endl;
                }
                //// 调用 find_top_better_strings 去找到fitness前3
                std::vector <std::string> top_better_algo_sequences_2 = find_top_better_strings(seq_to_db_map_2);
                better_seq_to_db_map_2.clear();
                for (const auto &sequence: top_better_algo_sequences_2) {
                    fit_area_delay tmp_fit_area_delay;
                    tmp_fit_area_delay = seq_to_db_map_2.find(sequence)->second;
                    better_seq_to_db_map_2.emplace(sequence, tmp_fit_area_delay);
                    next_population_v_2.push_back(sequence);
                }

                ////得到交叉或者变异之后的child
                for (uint64_t i = 0; i < (current_population_v_2.size() - top_better_algo_sequences_2.size()); ++i) {

                    std::string string_father = ga_select(seq_to_db_map_2, current_population_v_2);
                    std::vector <std::string> vector_father = string_to_vector(string_father);
                    std::string string_mother = ga_select(seq_to_db_map_2, current_population_v_2);
                    std::vector <std::string> vector_mother = string_to_vector(string_mother);
                    std::cout << "father2:" << string_father << std::endl;
                    std::cout << "mother2:" << string_mother << std::endl;

                    ////自适应
                    double father_fitness_2 = 0;
                    double mother_fitness_2 = 0;
                    for (const auto &item: seq_to_db_map_2) {
                        if (string_father == item.first) {
                            father_fitness_2 = item.second.fitness;
                        }
                    }
//                    std::cout << "father_fitness_2:" << father_fitness_2 << std::endl;
                    for (const auto &item: seq_to_db_map_2) {
                        if (string_mother == item.first) {
                            mother_fitness_2 = item.second.fitness;
                        }
                    }
//                    std::cout << "mother_fitness_2:" << mother_fitness_2 << std::endl;
                    double max_fitness_of_parents = std::max(father_fitness_2, mother_fitness_2);
                    if (max_fitness_of_parents != min_fitness_2 && max_fitness_of_parents != max_fitness_2) {
                        cross_probability =
                                K1 * ((max_fitness_2 - max_fitness_of_parents) / (max_fitness_2 - min_fitness_2));
                        mutation_probability =
                                K4 * ((max_fitness_2 - max_fitness_of_parents) / (max_fitness_2 - min_fitness_2));
                    } else if (max_fitness_of_parents == min_fitness_2) {
                        cross_probability = K2;
                        mutation_probability = K5;
                    } else if (max_fitness_of_parents == max_fitness_2) {
                        cross_probability = K3;
                        mutation_probability = K6;
                    }

//                    std::cout << "cross_probability:" << cross_probability << std::endl;
//                    std::cout << "mutation_probability:" << mutation_probability << std::endl;
                    std::random_device rd;
                    std::mt19937 gen(rd());
                    std::uniform_real_distribution<> dis(0.0, 1.0);
                    double random_num = dis(gen);
                    std::string child = "";

                    vector_father.pop_back();
                    vector_mother.pop_back();
                    bool is_not_father_equal_to_macro =
                            is_not_vector_equal(vector_father, macro_1) &&
                            is_not_vector_equal(vector_father, macro_2) &&
                            is_not_vector_equal(vector_father, macro_3) &&
                            is_not_vector_equal(vector_father, macro_4) &&
                            is_not_vector_equal(vector_father, macro_5);
                    bool is_not_mother_equal_to_macro =
                            is_not_vector_equal(vector_mother, macro_1) &&
                            is_not_vector_equal(vector_mother, macro_2) &&
                            is_not_vector_equal(vector_mother, macro_3) &&
                            is_not_vector_equal(vector_mother, macro_4) &&
                            is_not_vector_equal(vector_mother, macro_5);
                    vector_father.push_back("map_fpga;");
                    vector_mother.push_back("map_fpga;");

                    if (is_not_father_equal_to_macro && is_not_mother_equal_to_macro &&
                        random_num < cross_probability) {
                        child = crossover_op(vector_father, vector_mother);
                    } else {
                        std::vector <std::string> child_temp;
                        if (continue_not_opt_flag){
                            child_temp = get_random_add_lut_sequence(add_lut_strings, algo_num_of_stage_2);
                        }
                        else{
                            child_temp = get_random_sequence(strings, algo_num_of_stage_2);
                        }
                        std::string combined_algo_seq_string = std::accumulate(child_temp.begin(),
                                                                               child_temp.end(),
                                                                               std::string());
                        child = combined_algo_seq_string;
                    }
                    if (random_num < mutation_probability) {
//                        std::cout << "mutation" << std::endl;
                        std::vector <std::string> v_child = string_to_vector(child);
                        v_child.pop_back();
                        std::vector <std::string> mutation_child = mutation(v_child);
//                        mutation_child.push_back("map_fpga");
//                        std::vector<std::string> algo_sequence = get_random_sequence(strings, algo_num);

                        std::string combined_algo_seq_string = std::accumulate(mutation_child.begin(),
                                                                               mutation_child.end(),
                                                                               std::string());
                        child = combined_algo_seq_string;
                    }
//                    std::cout << "child2:" << child << std::endl;
                    ////存入下一次种群,得到一个完整的next_seq_to_de_map
                    if (child == string_father || child == string_mother) {
//                        std::cout<<"child == pareants"<<std::endl;
                        std::vector <std::string> algo_sequence;
                        if (continue_not_opt_flag){
                            algo_sequence = get_random_add_lut_sequence(add_lut_strings, algo_num_of_stage_2);
                        }
                        else{
                            algo_sequence = get_random_sequence(strings, algo_num_of_stage_2);
                        }
                        run_algo_seq(algo_sequence);
                        ////turn vector to string
                        std::string same_pare_child = std::accumulate(algo_sequence.begin(),
                                                                      algo_sequence.end(),
                                                                      std::string());
                        next_population_v_2.push_back(same_pare_child);
                    } else {
//                        std::cout << "child != pareants" << std::endl;
                        next_population_v_2.push_back(child);

                    }
                }

                // 清空 seq_to_db_map_2
                seq_to_db_map_2.clear();
                for (const auto &toDbMap2: better_seq_to_db_map_2) {
                    seq_to_db_map_2.emplace(toDbMap2.first, toDbMap2.second);
                }
                for (const auto &next_population_string_2: next_population_v_2) {
                    if (better_seq_to_db_map_2.count(next_population_string_2) > 0) {
                        continue;
                    }
                    std::vector <std::string> v_tmp_item = string_to_vector(next_population_string_2);
                    run_algo_seq(v_tmp_item);
                    iFPGA::klut_network klut = store<iFPGA::klut_network>().current()._storage;
                    iFPGA::depth_view <iFPGA::klut_network> dklut(klut);
                    double current_area = klut.num_gates();
                    double current_delay = dklut.depth();
                    double fitness = fitness_func(current_area, current_delay, no_opt_area, no_opt_delay);
                    //复原为原来一开始输入文件的那个结构
                    store<iFPGA::aig_network>().current() = stage2_initial_aig;
                    store<iFPGA::aig_network>().current()._storage = stage2_initial_aig_storage;

                    iFPGA::aig_with_choice restore_awc_6(stage2_initial_aig);
                    iFPGA::mapping_view<iFPGA::aig_with_choice, true, false> restore_mapped_aig_6(restore_awc_6);
                    iFPGA_NAMESPACE::klut_mapping<decltype(restore_mapped_aig_6), true>(restore_mapped_aig_6,
                                                                                        param_mapping);
                    const auto initial_kluts_6 = *iFPGA_NAMESPACE::choice_to_klut<iFPGA_NAMESPACE::klut_network>(
                            restore_mapped_aig_6);
                    store<iFPGA::klut_network>().current() = initial_kluts_6;

                    iFPGA::klut_network state1_initial_klut = store<iFPGA::klut_network>().current()._storage;
                    iFPGA::depth_view <iFPGA::klut_network> state1_initial_dklut(state1_initial_klut);
//                    double state1_current_area = state1_initial_klut.num_gates();
//                    double state1_current_delay = state1_initial_dklut.depth();
//                    std::cout << "test_after_state_initial" << std::endl;
//                    std::cout << "best_area: " << state1_current_area << std::endl;
//                    std::cout << "best_delay: " << state1_current_delay << std::endl;

                    ///找第一阶段fitness最大值
                    for (const auto &dbMap: better_seq_to_db_map_2) {
                        std::string current_seq = dbMap.first;
                        double current_fitness = dbMap.second.fitness;
                        if (current_fitness > best_fitness_of_2) {
                            best_seq_of_2 = current_seq;
                            best_fitness_of_2 = current_fitness;
                        }
                    }
                    if (fitness > best_fitness_of_2) {
                        best_seq_of_2 = next_population_string_2;
                        best_fitness_of_2 = fitness;
                    }

                    fit_area_delay tmp_fit_area_delay;
                    tmp_fit_area_delay.fitness = fitness;
                    tmp_fit_area_delay.area = current_area;
                    tmp_fit_area_delay.delay = current_delay;
                    seq_to_db_map_2.emplace(next_population_string_2, tmp_fit_area_delay);
                }

                //// 输出最佳序列和最佳适应度
                std::cout << "Best Sequence: " << best_seq_of_2 << std::endl;
                std::cout << "Best Fitness: " << best_fitness_of_2 << std::endl;
                std::cout << "best_area:" << seq_to_db_map_2.find(best_seq_of_2)->second.area << std::endl;
                std::cout << "best_delay:" << seq_to_db_map_2.find(best_seq_of_2)->second.delay << std::endl;
                std::cout << "best_fitness:" << seq_to_db_map_2.find(best_seq_of_2)->second.fitness << std::endl;

                current_population_v_2.clear();  // 清空 current_population_v
                current_population_v_2 = next_population_v_2;
                next_population_v_2.clear();

                count2 += 1;
                std::cout << "count2: " << count2 << std::endl;
                auto end_2 = std::chrono::steady_clock::now();
                std::chrono::duration<double> run_time_2 = end_2 - start;
                std::cout << "run_time: " << run_time_2.count() << std::endl;
                if (run_time_2.count() >= limited_time_2.count()) {
                    break;
                }
            }

            store<iFPGA::aig_network>().current() = initial_aig;
            store<iFPGA::aig_network>().current()._storage = initial_aig_storage;

            iFPGA::aig_with_choice restore_awc_7(stage2_initial_aig);
            iFPGA::mapping_view<iFPGA::aig_with_choice, true, false> restore_mapped_aig_7(restore_awc_7);
            iFPGA_NAMESPACE::klut_mapping<decltype(restore_mapped_aig_7), true>(restore_mapped_aig_7, param_mapping);
            const auto initial_kluts_7 = *iFPGA_NAMESPACE::choice_to_klut<iFPGA_NAMESPACE::klut_network>(
                    restore_mapped_aig_7);
            store<iFPGA::klut_network>().current() = initial_kluts_7;

            std::vector <std::string> vector_best_seq_of_1 = string_to_vector(best_seq);
            std::vector <std::string> vector_best_seq_of_2 = string_to_vector(best_seq_of_2);
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
            iFPGA::klut_network klut = store<iFPGA::klut_network>().current()._storage;
            iFPGA::depth_view <iFPGA::klut_network> dklut(klut);
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
            std::cout << " " << std::endl;

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