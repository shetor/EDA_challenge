#pragma once
#include "alice/alice.hpp"
#include "include/database/network/aig_network.hpp"
#include "include/database/network/klut_network.hpp"
#include "include/database/views/depth_view.hpp"

namespace alice 
{

class print_stats_command : public command 
{
public:
    explicit print_stats_command(const environment::ptr& env) : command(env, "print the stats for current Network!") 
    {
        add_option("--type, -t", type, "0 for AIG, 1 for FPGA, 2 for ASIC, [default = 0]");
        
    }
    // int get_num_pis() const { return num_pis_; }
    // int get_num_pos() const { return num_pos_; }
    // int get_num_gates() const { return num_gates_; }
    // int get_depth() const { return depth_; }
    rules validity_rules() const { return {}; }
    
protected:
    void execute()
    {
        if( type == 0 ) {
            if( store<iFPGA::aig_network>().empty() ) {
                printf("WARN: there is no any stored AIG file, please refer to the command \"read_aiger\"\n");
                return;
            }
            iFPGA::aig_network aig = store<iFPGA::aig_network>().current()._storage;
            iFPGA::depth_view<iFPGA::aig_network> daig(aig);
            // // 设置成员变量的值
            // num_pis_ = aig.num_pis();
            // num_pos_ = aig.num_pos();
            // num_gates_ = aig.num_gates();
            // depth_ = daig.depth();
            printf("Stats of AIG: pis=%d, pos=%d, area=%d, depth=%d\n", aig.num_pis(), aig.num_pos(), aig.num_gates(), daig.depth());
        }
        else if (type == 1) {
            if( store<iFPGA::klut_network>().empty() ) {
                printf("WARN: there is no any FPGA mapping result, please refer to the command \"map_fpga\"\n");
                return;
            }
            iFPGA::klut_network klut = store<iFPGA::klut_network>().current()._storage;
            iFPGA::depth_view<iFPGA::klut_network> dklut(klut);
            printf("Stats of FPGA: pis=%d, pos=%d, area=%d, depth=%d\n", klut.num_pis(), klut.num_pos(), klut.num_gates(), dklut.depth());
        }
        else if(type == 2) {
            printf("FAIL, comming soon!\n");
            return;
        }
        else {
            printf("FAIL, not supported type!\n");
            return;
        }
    }
private:
    int type = 0;
    // int num_pis_ = 0;
    // int num_pos_ = 0;
    // int num_gates_ = 0;
    // int depth_ = 0;
};
ALICE_ADD_COMMAND(print_stats, "Utils");
};