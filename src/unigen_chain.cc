//
// Created by mikhail on 6/16/22.
//

#include "unigen_chain.h"
UnigenChain::UnigenChain(const std::string &input_file_list,
                         const std::string &input_tree_name)
    : InputChain(input_file_list, input_tree_name) {
  chain_->SetBranchAddress( "event", &event_ );
}
