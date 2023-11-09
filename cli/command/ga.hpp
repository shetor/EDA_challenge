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

#include <ctime>

namespace alice {

    class ga_command : public command {
    public:
        explicit ga_command(const environment::ptr &env) : command(env, "GA!") {
        }

        rules validity_rules() const { return {}; }

    protected:
        void run_algo_seq(std::vector <std::string> algo_sequence) {
            for (const std::string &str: algo_sequence) {
                if (str == " balance ") {
                    store<iFPGA::aig_network>().current() = iFPGA::balance_and(store<iFPGA::aig_network>().current());
                }
                if (str == " rewrite ") {
                    uint32_t cut_size = 4u;
                    uint32_t priority_size = 10u;
                    bool preserve_level = true;
                    bool zero_gain = false;
                    bool verbose = false;
                    iFPGA::aig_network aig = store<iFPGA::aig_network>().current();
                    iFPGA::rewrite_params params;
                    params.b_preserve_depth = preserve_level;
                    params.b_use_zero_gain = zero_gain;
                    params.cut_enumeration_ps.cut_size = cut_size;
                    params.cut_enumeration_ps.cut_limit = priority_size;
                    aig = iFPGA::rewrite(aig, params);
                    store<iFPGA::aig_network>().current() = aig;

                }
                if (str == " rewrite -v ") {
                    uint32_t cut_size = 4u;
                    uint32_t priority_size = 10u;
                    bool preserve_level = true;
                    bool zero_gain = false;
                    bool verbose = true;
                    iFPGA::aig_network aig = store<iFPGA::aig_network>().current();
                    iFPGA::rewrite_params params;
                    params.b_preserve_depth = preserve_level;
                    params.b_use_zero_gain = zero_gain;
                    params.cut_enumeration_ps.cut_size = cut_size;
                    params.cut_enumeration_ps.cut_limit = priority_size;
                    aig = iFPGA::rewrite(aig, params);
                    store<iFPGA::aig_network>().current() = aig;

                }
                if (str == " rewrite -z ") {
                    uint32_t cut_size = 4u;
                    uint32_t priority_size = 10u;
                    bool preserve_level = true;
                    bool zero_gain = true;
                    bool verbose = false;
                    iFPGA::aig_network aig = store<iFPGA::aig_network>().current();
                    iFPGA::rewrite_params params;
                    params.b_preserve_depth = preserve_level;
                    params.b_use_zero_gain = zero_gain;
                    params.cut_enumeration_ps.cut_size = cut_size;
                    params.cut_enumeration_ps.cut_limit = priority_size;
                    aig = iFPGA::rewrite(aig, params);
                    store<iFPGA::aig_network>().current() = aig;

                }
                if (str == " refactor ") {
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
                if (str == " refactor -z ") {
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
                if (str == " refactor -v ") {
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
                if (str == " map_fpga ") {
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
            clock_t start_time, end_time;
            start_time = clock();
            iFPGA::aig_network initial_aig = store<iFPGA::aig_network>().current();
            iFPGA::aig_network initial_aig_storage = initial_aig._storage;
            iFPGA::depth_view <iFPGA::aig_network> initial_daig(initial_aig_storage);
            double no_opt_area = initial_aig_storage.num_gates();
            double no_opt_delay = initial_daig.depth();
            double sum_fitness;
            std::vector<double> v_qor;
            std::vector<double> v_halp_top_fitness;
            std::unordered_map<std::string,fit_area_delay>seq_to_db_map;
            std::unordered_map<std::string,fit_area_delay>half_seq_to_db_map;

            uint64_t algo_num = 10;
            uint64_t sequence_num = 10;
            std::vector<int> v_area;
            std::vector<int> v_depth;
            std::vector <std::string> strings = {" balance ", " rewrite ",  " rewrite -z ", " rewrite -v ", " refactor ",
                                                 " refactor -z ", " refactor -v "};
            ////生成初始序列
            for (int i = 0; i < sequence_num; ++i) {
                std::vector <std::string> algo_sequence = get_random_sequence(strings, algo_num);
                run_algo_seq(algo_sequence);
                ////turn vector to string
                std::string combined_algo_seq_string = std::accumulate(algo_sequence.begin(),algo_sequence.end(),std::string());
                std::cout<<"Combined String:"<<combined_algo_seq_string<<std::endl;
                iFPGA::aig_network aig = store<iFPGA::aig_network>().current()._storage;
                iFPGA::depth_view <iFPGA::aig_network> daig(aig);
                double current_area = aig.num_gates();
                double current_delay = daig.depth();
                double qor = reward_func(current_area, current_delay, no_opt_area, no_opt_delay);
                v_qor.push_back(qor);
                ////test
//                std::cout<<"qor:"<<qor<<std::endl;
//                std::cout<<"current_area: "<<current_area<<std::endl;
//                std::cout<<"current_delay: "<<current_delay<<std::endl;
//                std::cout<<"no_opt_area:"<<no_opt_area<<std::endl;
//                std::cout<<"no_opt_delay:"<<no_opt_delay<<std::endl;
                double fitness = fitness_func(current_area, current_delay, no_opt_area, no_opt_delay);
                std::cout<<"fitness:"<<fitness<<std::endl;
                v_area.push_back(aig.num_gates());
                v_depth.push_back(daig.depth());
                fit_area_delay tmp_fit_area_delay;
                tmp_fit_area_delay.fitness = fitness;
                tmp_fit_area_delay.area = current_area;
                tmp_fit_area_delay.delay = current_delay;
                seq_to_db_map.emplace(combined_algo_seq_string,tmp_fit_area_delay);
                std::cout << "Stats of AIG: pis=" << aig.num_pis() << ", pos=" << aig.num_pos() << ", area="
                          << current_area << ", depth=" << current_delay << std::endl;
            }
            //// 调用 find_top_half_strings 去找到fitness高的前一半
            std::vector<std::string> top_half_algo_sequences = find_top_half_strings(seq_to_db_map);
            ////            test

            ////得到前一半fitness，返回一个fitness_vector
            for (const auto &sequence: top_half_algo_sequences) {
                double fitness = seq_to_db_map.find(sequence)->second.fitness;
//                std::cout<<"half_top_fitness:"<<fitness<<std::endl;
                fit_area_delay tmp_db = seq_to_db_map.find(sequence)->second;
                half_seq_to_db_map.emplace(sequence,tmp_db);///将前一半赋给这个map
                v_halp_top_fitness.push_back(fitness);
                sum_fitness+=fitness;
            }
            ////计算所有fitness_prob，存进half_map的prob
            for (const auto &seqToDbMap: half_seq_to_db_map) {
                double fitness = seq_to_db_map.find(seqToDbMap.first)->second.fitness;
                double fit_prob = fitness/sum_fitness;
                half_seq_to_db_map.find(seqToDbMap.first)->second.fit_prob = fit_prob;

                ////test
//                std::cout<<"sequence: "<<seqToDbMap.first<<std::endl;
//                std::cout<<"fitness: "<<fitness<<std::endl;
//                std::cout<<"area: "<<half_seq_to_db_map.find(seqToDbMap.first)->second.area<<std::endl;
//                std::cout<<"delay: "<<half_seq_to_db_map.find(seqToDbMap.first)->second.delay<<std::endl;
//                std::cout<<"fit_prob: "<<half_seq_to_db_map.find(seqToDbMap.first)->second.fit_prob<<std::endl;

            }









//
//            iFPGA::klut_network klut = store<iFPGA::klut_network>().current()._storage;
//            iFPGA::depth_view <iFPGA::klut_network> dklut(klut);
//            printf("Stats of FPGA: pis=%d, pos=%d, area=%d, depth=%d\n", klut.num_pis(), klut.num_pos(),
//                   klut.num_gates(), dklut.depth());
            end_time = clock();
            std::cout << "run_time: " << (double) (end_time - start_time) / CLOCKS_PER_SEC << std::endl;
        }

    private:
    };

    ALICE_ADD_COMMAND(ga,
    "Genetic Algorithm");
};