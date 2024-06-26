#pragma once
#include "alice/alice.hpp"

namespace alice {

class history_command : public command {
public:
    explicit history_command(const environment::ptr &env) :
        command(env, "Perform history AIG operations, the history size is 5") {
        add_option("--replace, -R", index_replace_history, "Options of replace the previous optimized AIG by current AIG");
        add_option("--backup, -B", index_replace_current, "Options of replace the current AIG by the previous optimized AIG");
        add_flag("--clear, -c", cclear, "toggles of clear the history AIG files [default=no]");
        add_flag("--size, -s", csize, "toggles of show the history size of AIG files [default=no]");
        add_flag("--add, -a", cadd, "toggles of add the previous optimized AIG file [default=no]");
    }

    rules validity_rules() const {
        return {};
    }

protected:
    void execute() {
        if(!is_set("-R")) {
            index_replace_history = -1;
        }
        if(!is_set("-B")) {
            index_replace_current = -1;
        }

        if(is_set("-c")) {
            cclear = true;
        } else {
            cclear = false;
        }
        if(is_set("-s")) {
            csize = true;
        } else {
            csize = false;
        }
        if(is_set("-a")) {
            cadd = true;
        } else {
            cadd = false;
        }

        if (store<iFPGA::aig_network>().empty()) {
            printf("WARN: there is no any stored AIG file, please refer to the command \"read_aiger\"\n");
            return;
        }

        if (cadd && history_index + 1 <= max_size) {
            iFPGA::aig_network aig = store<iFPGA::aig_network>().current()._storage;
            store<iFPGA::aig_network>()[++history_index] = aig;
        }

        if (history_index + 1 > max_size) {
            printf("WARN: the max history size is %d, please refer to the command \"history -h\"\n", max_size);
            return;
        }

        // using current AIG to replace the indexed history AIG
        if (index_replace_history == -1) {
            // do nothing
        } else if (index_replace_history > history_index || index_replace_history < -1) {
            printf("WARN: the replace index is out of range, please refer to the command \"history -h\"\n");
        } else {
            iFPGA::aig_network aig = store<iFPGA::aig_network>().current()._storage;
            store<iFPGA::aig_network>()[index_replace_history] = aig; // replace the AIG file
        }

        if (index_replace_current < 0 || index_replace_current > 4) {
            // do nothing
        } else if (index_replace_current > history_index) {
            printf("WARN: the history index is out of range, please refer to the command \"history -h\"\n");
        } else {
            store<iFPGA::aig_network>()[5] = store<iFPGA::aig_network>()[index_replace_current];
        }

        if (cclear) {
            history_index = -1;
        }

        if (csize) {
            printf("INFO: the history size is %d\n", history_index + 1); // current aig are the latest version of optimized AIG, and the previous indexed AIGs are the history AIGs.
        }

        cclear = false;
        csize = false;
        cadd = false;
        index_replace_history = -1;
        index_replace_current = -1;
        return;
    }

private:
    int max_size = 5; // the history AIGs are indexed with {0,1,2,3,4}
    int history_index = -1;
    bool cclear = false;
    bool csize = false;
    bool cadd = false;
    int index_replace_history = -1;
    int index_replace_current = -1;
};
ALICE_ADD_COMMAND(history, "Logic optimization");
}; // namespace alice