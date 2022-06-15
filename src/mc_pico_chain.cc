//
// Created by mikhail on 6/14/22.
//

#include <fstream>
#include <iostream>
#include "mc_pico_chain.h"

void McPicoChain::Init() {
  chain_ = new TChain( "mctree" );
  chain_->SetBranchAddress( "bimp", &impact_parameter_ );
  chain_->SetBranchAddress( "phi2", &reaction_plain_ );
  chain_->SetBranchAddress( "nh", &n_particles_ );
  chain_->SetBranchAddress( "momx", px_ );
  chain_->SetBranchAddress( "momy", py_ );
  chain_->SetBranchAddress( "momz", pz_ );
  chain_->SetBranchAddress( "pdg", pdg_code_ );
}
void McPicoChain::AddFiles(const std::string& file_list) {
  if( !chain_ )
    this->Init();
  std::cout << "Reading file from filelist " << file_list << std::endl;
  std::ifstream infile(file_list);
  std::string input_root_file;
  if( !infile.is_open() ){
    throw std::runtime_error( "Filelist "+file_list+" cannot be open" );
  }
  int i=0;
  while( infile >> input_root_file ){
    auto success = chain_->AddFile( input_root_file.c_str() );
    if( success != 1 )
      throw std::runtime_error( "File "+ input_root_file +" cannot be open for reading" );
    std::cout << i << ". " << input_root_file << " added\n";
    i++;
  }
}
bool McPicoChain::NextEvent() {
  assert(chain_);
  current_event_++;
  if(current_event_ >= chain_->GetEntries())
    return false;
  chain_->GetEntry(current_event_);
  return true;
}
