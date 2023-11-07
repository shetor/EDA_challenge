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
#include <typeinfo>
namespace alice {

    class ga_command : public command {
    public:
        explicit ga_command(const environment::ptr &env) : command(env, "GA!") {
        }

        rules validity_rules() const { return {}; }

    protected:
        void execute() {
            std::vector <std::string> strings = {"balance", "rewrite", "rewrite -z", "rewrite -v", "refactor",
                                                 "refactor -z", "refactor -v"};
            int algo_num = 10;
            std::vector <std::string> algo_sequence = get_random_sequence(strings, algo_num);
            for (const std::string &str: algo_sequence) {
                if (str == "balance") {
                    store<iFPGA::aig_network>().current() = iFPGA::balance_and(store<iFPGA::aig_network>().current());
                }
                if (str == "rewrite") {
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
                if (str == "rewrite -v") {
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
                if (str == "rewrite -z") {
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
                if (str == "refactor") {
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
                if (str == "refactor -z") {
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
                if (str == "refactor -v") {
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
                if (str == "map_fpga") {
                    if( store<iFPGA::klut_network>().empty() ) {
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
            iFPGA::aig_network aig = store<iFPGA::aig_network>().current()._storage;
            iFPGA::depth_view <iFPGA::aig_network> daig(aig);
            printf("Stats of AIG: pis=%d, pos=%d, area=%d, depth=%d\n", aig.num_pis(), aig.num_pos(),
                   aig.num_gates(),
                   daig.depth());
            iFPGA::klut_network klut = store<iFPGA::klut_network>().current()._storage;
            iFPGA::depth_view <iFPGA::klut_network> dklut(klut);
            printf("Stats of FPGA: pis=%d, pos=%d, area=%d, depth=%d\n", klut.num_pis(), klut.num_pos(),
                   klut.num_gates(), dklut.depth());
        }

    private:

    };

    ALICE_ADD_COMMAND(ga,
    "Genetic Algorithm");
};