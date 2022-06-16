//
// Created by mikhail on 6/16/22.
//

#include "mc_pico_chain.h"
#include <iostream>
#include <fstream>
#include "input_chain.h"

InputChain::InputChain(const std::string &input_file_list,
                       const std::string &input_tree_name) : chain_(new TChain(input_tree_name.c_str())) {
  this->AddFiles(input_file_list);
}

void InputChain::AddFiles(const std::string& file_list) {
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

bool InputChain::NextEvent() {
  assert(chain_);
  current_event_++;
  if(current_event_ >= chain_->GetEntries())
    return false;
  chain_->GetEntry(current_event_);
  return true;
}

