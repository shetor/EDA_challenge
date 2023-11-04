#include "algorithm/cache.hpp"
#include "cli/command/io/read_aiger.hpp"
#include "cli/command/io/write_aiger.hpp"
#include "cli/command/io/write_fpga.hpp"
#include "cli/command/io/write_verilog.hpp"
#include "cli/command/io/write_dot.hpp"

#include "cli/command/rewrite.hpp"
#include "cli/command/refactor.hpp"
#include "cli/command/balance.hpp"
#include "cli/command/lut_opt.hpp"
#include "cli/command/map_fpga.hpp"
#include "cli/command/cleanup.hpp"
#include "cli/command/print_stats.hpp"
#include "cli/command/history.hpp"
ALICE_MAIN(imap)