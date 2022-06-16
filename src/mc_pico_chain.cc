//
// Created by mikhail on 6/14/22.
//

#include <fstream>
#include <iostream>
#include "mc_pico_chain.h"
McPicoChain::McPicoChain(const std::string &input_file_list,
                         const std::string &input_tree_name)
    : InputChain(input_file_list, input_tree_name) {
  chain_->SetBranchAddress( "bimp", &impact_parameter_ );
  chain_->SetBranchAddress( "phi2", &reaction_plain_ );
  chain_->SetBranchAddress( "nh", &n_particles_ );
  chain_->SetBranchAddress( "momx", px_ );
  chain_->SetBranchAddress( "momy", py_ );
  chain_->SetBranchAddress( "momz", pz_ );
  chain_->SetBranchAddress( "pdg", pdg_code_ );
}