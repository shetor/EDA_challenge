// ***************************************************************************************
// Copyright (c) 2023-2025 Peng Cheng Laboratory
// Copyright (c) 2023-2025 Shanghai Anlogic Infotech Co.,Ltd.
// Copyright (c) 2023-2025 Peking University
//
// iMAP-FPGA is licensed under Mulan PSL v2.
// You can use this software according to the terms and conditions of the Mulan PSL v2.
// You may obtain a copy of Mulan PSL v2 at:
// http://license.coscl.org.cn/MulanPSL2
//
// THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
// EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
// MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
//
// See the Mulan PSL v2 for more details.
// ***************************************************************************************

#pragma once

#include <limits.h>

#include <algorithm>
#include <queue>
#include <set>
#include <unordered_set>
#include <vector>

#include "algorithms/cleanup.hpp"
#include "algorithms/ref_deref.hpp"
#include "detail/sop_refactoring.hpp"
#include "kitty/constructors.hpp"
#include "kitty/isop.hpp"
#include "network/aig_network.hpp"
#include "utils/ifpga_namespaces.hpp"
#include "views/topo_view.hpp"

#include "detail/sop_refactoring.hpp"

iFPGA_NAMESPACE_HEADER_START

struct refactor_params
{
  /*! \brief Maximum number of leaves in reconvergence_driven_cut. */
  uint32_t max_leaves_num{10};
  /*! \brief Maximum number of nodes in cone. */
  uint32_t max_cone_size{16};
  /*! \brief whether to allow depth growth*/
  bool preserve_depth = false;
  /*! \brief whether allow replace sub_ntk with zero gain */
  bool allow_zero_gain = false;
  bool verbose = false;
};

namespace detail
{

  template <typename Ntk, class RefactoringFn, typename NodeCostFn>
  class refactor_impl
  {
  public:
    refactor_impl(Ntk const& ntk,
                  RefactoringFn const& refactoring_fn,
                  refactor_params const& params,
                  NodeCostFn const& node_cost_fn)
        : _ntk(ntk),
          _refactoring_fn(refactoring_fn),
          _params(params),
          _node_cost_fn(node_cost_fn)
    {
      init();
    }

    /**
     * @brief the main algorithm procedure
     */
    Ntk run()
    {
      topo_view<Ntk>(_ntk).foreach_gate([&](auto const &n) {
        // skip the nodes with many fanouts and nodes with no fanout
        if (_ntk.is_dead(n) || _ntk.fanout_size(n) > 1000)
          return;

        // 1、compute a reconvergence-driven cut
        compute_reconvergence_driven_cut(n);

        // 2、collect MFFC nodes
        collect_cone_nodes(n);

        // 3、local refactor the cone with the constrained size
        if (_visited_nodes.size() > 1 && _visited_nodes.size() <= _params.max_cone_size){
          local_isop_refactor(n);
        }
      });

      // clean up the dangling nodes caused by the padded subgraphs
      return cleanup_dangling<Ntk>(_ntk);
    }

  private:
    void init()
    {
      set_nodes_refered_value();
      collect_fanouts();
      compute_depth();
      compute_reverse_depth();
    }

    /**
     * @brief initizlize the node's refered value by fan-out size
     * 
     *  the refered value means how many times used by its fan-out nodes
     */
    void set_nodes_refered_value()
    {
      _ntk.clear_values();
      _ntk.foreach_node([&](auto const &n) { 
        _ntk.set_value(n, _ntk.fanout_size(n)); 
      });
    }

    /**
     * @brief compute depth for each node
     *    (depth is seemed as the unit delay model)
     *    depth(n) = max(depth(n), depth( childred(n) ) + 1 )
     */
    void compute_depth()
    {
      // set depth of constant
      _depth_map.insert( std::make_pair(0, 0) );
      
      // set the depth of PIs
      _ntk.foreach_pi([&](auto const& pi){
        _depth_map.insert( std::make_pair(pi, 0) );
      });

      // compute the depth of internal node
      _ntk.foreach_gate([&](auto const& n) {
        assert( _depth_map.find(n) == _depth_map.end() );
        uint32_t dmax = 0u;
        _ntk.foreach_fanin( n, [&](auto const& c){
          auto nc = _ntk.get_node(c);
          assert( _depth_map.find(nc) != _depth_map.end() ); 
          dmax = std::max(dmax, _depth_map[nc] + 1);
        });
        _depth_map.insert( std::make_pair(n, dmax) );
      });

      // compute the max depth of _ntk
      _ntk.foreach_po([&](auto const& po){
        auto npo = _ntk.get_node(po);
        _depth_max = std::max(_depth_max, _depth_map[npo]);
      });
    }

    /**
     * @brief Computes reverse levels for each node (length of the farthest path from n to POs)
     */
    void compute_reverse_depth()
    {
      std::vector<node<Ntk>> rtopo_nodes(_ntk.num_gates());
      uint32_t rindex = _ntk.num_gates();

      // collect reverse topo nodes
      _ntk.foreach_gate([&](auto const& n){
        rtopo_nodes[rindex-1] = n;
        rindex -= 1;
        _rdepth_map.insert( std::make_pair(n, 1) );
      });

      // init the rdepth of constant
      _rdepth_map.insert( std::make_pair(0, 1) );
      
      // init the rdepth of PIs
      _ntk.foreach_pi([&](auto const& pi){
        _rdepth_map.insert( std::make_pair(pi, 1) );
      });

      
      // compute the reverse depth of internal nodes
      for(auto n : rtopo_nodes) {
        _ntk.foreach_fanin(n, [&](auto const& c){
          auto nc = _ntk.get_node(c);
          _rdepth_map[nc] = std::max(_rdepth_map[nc], _rdepth_map[n] + 1);
        });
      }
    }

    /**
     * @brief collect the fanout nodes for each node
     */
    void collect_fanouts()
    {
      _ntk.foreach_node([&](auto const& n) { 
        _ntk.foreach_fanin(n, [&](auto const& c) {
          auto nc = _ntk.get_node(c);
          auto iter = _fanouts_map.find(nc);
          if( iter != _fanouts_map.end() ) {
            iter->second.insert( n );
          } else {
            std::unordered_set<node<Ntk>> new_set;
            new_set.insert( n );
            _fanouts_map.insert( std::make_pair(nc, new_set ) );
          } 
        }); 
      });
    }

    /**
     * @brief Finds a fanin-limited, reconvergence-driven cut for the node
     */
    void compute_reconvergence_driven_cut(node<Ntk> root)
    {
      // init
      _ntk.clear_visited();
      _visited_nodes.clear();
      _leaves_nodes.clear();

      // record visited nodes and leaves nodes
      _visited_nodes.push_back(root);
      _ntk.set_visited(root, 1u);
      _ntk.foreach_fanin(root, [&](auto const &s, auto i)
                         {
      _ntk.set_visited( _ntk.get_node( s ), 1u );
      _visited_nodes.push_back( _ntk.get_node( s ) );
      _leaves_nodes.push_back( _ntk.get_node( s ) ); });

      while (one_level_build_cut())
        ;
      assert(_leaves_nodes.size() <= _params.max_leaves_num);
    }

    /**
     * @brief Builds reconvergence-driven cut by changing one leaf at a time
     */
    bool one_level_build_cut()
    {
      int best_cost, cur_cost;
      best_cost = INT_MAX;
      node<Ntk> best_fanin{0};
      int best_fanin_level = INT_MAX;

      for (auto leaf : _leaves_nodes)
      {
        cur_cost = compute_leaf_cost(leaf);
        int cur_level = _depth_map.find(leaf)->second;

        if (cur_cost < best_cost || (cur_cost == best_cost && cur_level > best_fanin_level))
        {
          best_cost = cur_cost;
          best_fanin = leaf;
          best_fanin_level = cur_level;
        }
        if (best_cost == 0)
        {
          break;
        }
      }
      if (best_fanin == 0)
      {
        return false;
      }

      assert(best_cost < 3);

      if (_leaves_nodes.size() - 1 + best_cost > _params.max_leaves_num)
      {
        return false;
      }
      // remove the new root node from the leaves
      for (auto iter = _leaves_nodes.begin(); iter != _leaves_nodes.end(); iter++)
      {
        if (*iter == best_fanin)
        {
          _leaves_nodes.erase(iter);
          break;
        }
      }

      // add the children of node to the fanins
      _ntk.foreach_fanin(best_fanin, [&](auto const &s, auto i)
                         {
      if ( !_ntk.visited( _ntk.get_node( s ) ) ) {
        _ntk.set_visited( _ntk.get_node( s ), 1 );
        _visited_nodes.push_back( _ntk.get_node( s ) );
        _leaves_nodes.push_back( _ntk.get_node( s ) );
      } });

      assert(_leaves_nodes.size() <= _params.max_leaves_num);
      return true;
    }

    /**
     * @brief Evaluate the cost of removing the node from the set of leaves
     */
    int compute_leaf_cost(node<Ntk> leaf)
    {
      int cost;
      // make sure the node is in the construction zone
      assert(_ntk.visited(leaf));
      if (_ntk.is_ci(leaf))
      {
        return INT_MAX;
      }
      // get the cost of the cone
      cost = !_ntk.visited(_ntk.get_node(_ntk.get_child0(leaf))) + !_ntk.visited(_ntk.get_node(_ntk.get_child1(leaf)));
      // always accept if the number of leaves does not increase
      if (cost < 2)
        return cost;
      // skip nodes with many fanouts
      if (_ntk.fanout_size(leaf) > Ntk::max_fanin_size)
        return INT_MAX;
      // return the number of nodes that will be on the leaves if this node is removed
      return cost;
    }

    /**
     * @brief Get the nodes contained in the cut
     */
    void collect_cone_nodes(node<Ntk> root)
    {
      std::set<node<Ntk>> cone_nodes;

      // add cut leaves into cone nodes
      for (auto leaf : _leaves_nodes)
      {
        cone_nodes.insert(_ntk.index_to_node(leaf));
      }

      // collect the nodes in the DFS order
      _visited_nodes.clear();
      dfs_collect_cone_nodes(root, cone_nodes);
    }

    /**
     * @brief Marks the TFI cone
     */
    void dfs_collect_cone_nodes(node<Ntk> root, std::set<node<Ntk>> &cone_nodes)
    {
      if (cone_nodes.find(root) != cone_nodes.end() || _ntk.is_pi(root))
      {
        return;
      }

      _ntk.foreach_fanin(root, [&](auto const &s, auto i)
                         { dfs_collect_cone_nodes(_ntk.get_node(s), cone_nodes); });
      cone_nodes.insert(root);
      _visited_nodes.push_back(root);
    }

    /**
     * @brief compute truth table of the cone
     */
    kitty::dynamic_truth_table compute_cone_tt(node<Ntk> root)
    {
      std::unordered_map<node<Ntk>, kitty::dynamic_truth_table> tts_map;
      uint32_t nvars_tt = _leaves_nodes.size();
      // init leaves
      for (int i = 0; i < pow(2, nvars_tt); ++i)
      {
        for (uint k = 0u; k < nvars_tt; ++k)
        {
          if ((i >> k) & 1)
          {
            if (tts_map.find(_leaves_nodes[k]) == tts_map.end())
            {
              kitty::dynamic_truth_table tmp_tt(nvars_tt);
              kitty::set_bit(tmp_tt, i);
              tts_map.insert(std::make_pair(_leaves_nodes[k], tmp_tt));
            }
            else
            {
              kitty::set_bit(tts_map[_leaves_nodes[k]], i);
            }
          }
        }
      }
      // topological compute truth table of nodes in cut
      for (auto cur_node : _visited_nodes)
      {
        if (_ntk.is_pi(cur_node))
        {
          continue;
        }

        auto child0 = _ntk.get_child0(cur_node);
        auto child1 = _ntk.get_child1(cur_node);

        assert(tts_map.find(_ntk.get_node(child0)) != tts_map.end());
        assert(tts_map.find(_ntk.get_node(child1)) != tts_map.end());

        auto tt0 = tts_map[_ntk.get_node(child0)];
        auto tt1 = tts_map[_ntk.get_node(child1)];
        assert(tt0.num_vars() == tt1.num_vars());

        kitty::dynamic_truth_table tt(nvars_tt);
        if (child0.complement)
        {
          if (child1.complement)
          {
            tt = kitty::binary_and(~tt0, ~tt1);
          }
          else
          {
            tt = kitty::binary_and(~tt0, tt1);
          }
        }
        else
        {
          if (child1.complement)
          {
            tt = kitty::binary_and(tt0, ~tt1);
          }
          else
          {
            tt = kitty::binary_and(tt0, tt1);
          }
        }
        tts_map.insert(std::make_pair(cur_node, tt));
      }
      // return truth table of root node
      return tts_map.find(root)->second;
    }

    /**
     * @brief perform the local refactor based on isop optimization
     */
    void local_isop_refactor(node<Ntk> root)
    {
      auto tt = compute_cone_tt(root);

      // skip the constant truth table
      if ( kitty::is_const0(tt) ) {
        return;
      }

      // collect cut
      std::vector<signal<Ntk>> cut;
      for (auto leaf : _leaves_nodes) {
        cut.push_back(_ntk.make_signal(leaf));
      }    

      const auto on_signal = [&](auto const &f_new){
        // compute gain
        // compute old sub ntk's cost(reconvergence driven cut size)
        auto num_nodes_save = deref_node_recursive<Ntk, NodeCostFn>(_ntk, root);
        auto num_nodes_added = ref_node_recursive<Ntk, NodeCostFn>(_ntk, _ntk.get_node(f_new));
        int gain = num_nodes_save - num_nodes_added;

        // compute depth
        assert( _rdepth_map.find(root) != _rdepth_map.end() );

        auto reuqire_depth = _depth_max + 1 - _rdepth_map[root];
        auto cur_depth = update_depth(_ntk.get_node(f_new));

        if ((gain > 0 || (_params.allow_zero_gain && gain == 0)) // area
            && ((cur_depth <= reuqire_depth) || !_params.preserve_depth) && root != _ntk.get_node(f_new))  // depth
        { 
          repalce_subgraph(root, f_new);
          update_fanouts_info(root, f_new);
          recursive_update_fanouts_depth(_ntk.get_node(f_new));
        }  else {
          deref_node_recursive<Ntk, NodeCostFn>(_ntk, _ntk.get_node(f_new));
          ref_node_recursive<Ntk, NodeCostFn>(_ntk, root);
        }

        return true;
      };

      // refactoring and replace sub ntk
      _refactoring_fn(_ntk, tt, cut.begin(), cut.end(), on_signal);
    }

    /**
     * @brief replace the root by the new signal, thus the original subgraph will be replaced by 
     * @param old_n old root
     * @param new_s new signal 
     */
    void repalce_subgraph(node<Ntk> old_n, signal<Ntk> new_s)
    {
      auto new_n = _ntk.get_node(new_s);

      // nodes relation repalce
      _ntk.substitute_node(old_n, new_s);

      // copy reference value
      _ntk.set_value(old_n, 0);
      _ntk.set_value(new_n, _ntk.fanout_size(new_n));
      for (auto leaf : _leaves_nodes)
      {
        _ntk.set_value(leaf, _ntk.fanout_size(leaf));
      }
    }

    /**
     * @brief update depth for nodes in new cone
     */
    uint32_t update_depth(node<Ntk> root)
    {
      std::unordered_set<node<Ntk>> leaves_set(_leaves_nodes.begin(), _leaves_nodes.end());
      return recursive_update_depth(root, leaves_set);
    }

    /**
     * @brief recursive compute depth for nodes in new cone
     */
    uint32_t recursive_update_depth(node<Ntk> n, std::unordered_set<node<Ntk>> &leaves_set)
    {
      if( leaves_set.find( n ) != leaves_set.end() ) {
        assert(_depth_map.find(n) != _depth_map.end() );
        return _depth_map[n];
      }
      
      uint32_t cur_depth = 0;
      // compute current max depth
      _ntk.foreach_fanin(n, [&](auto const &s, auto i) {
        auto child = _ntk.get_node( s );
        if ( leaves_set.find( child ) != leaves_set.end() ) {  // leaves node
          cur_depth = std::max( cur_depth, 1 + _depth_map.find( child )->second );
        } else {  // internal node
          cur_depth = std::max( cur_depth, 1 + recursive_update_depth( child, leaves_set ) );
        } 
      });

      // update depth info
      auto iter = _depth_map.find(n);
      if (iter == _depth_map.end()) { // newly node
        _depth_map.insert(std::make_pair(n, cur_depth));
      } else {
        iter->second = cur_depth;
      }

      return cur_depth;
    }

    /**
     * @brief update fanouts info for nodes in cone
     */
    void update_fanouts_info(node<Ntk> old_n, signal<Ntk> new_s)
    {
      // copy fanouts info of old node to new node
      auto old_iter = _fanouts_map.find(old_n);
      if (old_iter != _fanouts_map.end())
      {
        std::unordered_set<node<Ntk>> new_set(old_iter->second);
        _fanouts_map.insert(std::make_pair(_ntk.get_node(new_s), new_set));
        _fanouts_map.erase(old_iter);
      }

      for (auto n : _visited_nodes)
      {
        _ntk.foreach_fanin(n, [&](auto const &s, auto i)
                           {
        auto iter = _fanouts_map.find( _ntk.get_node( s ) );
        if ( iter != _fanouts_map.end() ) {
          iter->second.insert( n );
        } else {
          std::unordered_set<node<Ntk>> new_set;
          new_set.insert( n );
          _fanouts_map.insert( std::make_pair( _ntk.get_node( s ), new_set ) );
        } });
      }
    }

    /**
     * @brief recursive update fanouts depth
     * 
     * 这里貌似挺耗时的！
     */
    void recursive_update_fanouts_depth(node<Ntk> root)
    {
      auto iter_fanout = _fanouts_map.find(root);
      if (iter_fanout == _fanouts_map.end())
        return;

      auto fanouts_set = iter_fanout->second;
      auto root_dp = _depth_map.find(root)->second;
      
      for (auto fanout : fanouts_set) {
        auto iter_depth = _depth_map.find(fanout);
        if (iter_depth->second < root_dp + 1)  { //FIXME: BUG HERE!
          iter_depth->second = root_dp + 1;
          recursive_update_fanouts_depth(fanout);
        }
      }
    }

  private:
    Ntk             _ntk;
    RefactoringFn   _refactoring_fn;
    refactor_params _params;
    NodeCostFn      _node_cost_fn;

    std::vector<node<Ntk>> _visited_nodes;
    std::vector<node<Ntk>> _leaves_nodes;
    std::unordered_map<node<Ntk>, uint32_t> _depth_map;
    std::unordered_map<node<Ntk>, uint32_t> _rdepth_map;
    std::unordered_map<node<Ntk>, std::unordered_set<node<Ntk>>> _fanouts_map;
    uint32_t _depth_max = 0;
  }; // end class refactor_impl
};   // end namespace detail

template <typename Ntk = aig_network,
          class RefactoringFn = sop_factoring<Ntk>,
          typename NodeCostFn = unit_cost<Ntk>>
Ntk refactor(Ntk const&ntk, refactor_params const &params)
{
  RefactoringFn refactoring_fn;
  NodeCostFn cost_fn;
  const auto dest = detail::refactor_impl<Ntk, RefactoringFn, NodeCostFn>{ntk,
                                                                          refactoring_fn,
                                                                          params,
                                                                          cost_fn}.run();
  return dest;
}

iFPGA_NAMESPACE_HEADER_END
