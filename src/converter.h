//
// Created by mikhail on 6/14/22.
//

#ifndef MCPICO2AT_SRC_CONVERTER_H_
#define MCPICO2AT_SRC_CONVERTER_H_

#include "mc_pico_chain.h"
#include "out_tree_manager.h"

class Converter {
public:
  Converter( const std::string& input_file_list, const std::string& out_file_name, const std::string& out_tree_name );
  ~Converter() = default;
  void Run();
private:
  McPicoChain in_chain_;
  OutTreeManager out_tree_;
};

#endif // MCPICO2AT_SRC_CONVERTER_H_
