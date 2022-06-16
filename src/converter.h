//
// Created by mikhail on 6/14/22.
//

#ifndef MCPICO2AT_SRC_CONVERTER_H_
#define MCPICO2AT_SRC_CONVERTER_H_

#include "input_chain.h"
#include "unigen_chain.h"
#include "mc_pico_chain.h"
#include "out_tree_manager.h"

class Converter {
public:
  Converter(const std::string& out_file_name, const std::string& out_tree_name);
  ~Converter() = default;
  void SampleReactionPlane(bool sample_reaction_plane=true) { sample_reaction_plane_ = sample_reaction_plane; }
  void BoostToLab(bool boost_to_lab=true) { boost_to_lab_ = boost_to_lab; }
  void SetInChain(InputChain *in_chain) { in_chain_ = in_chain; }
  void SetCollidingSystem(const std::string &colliding_system, double energy) {
    colliding_system_ = colliding_system;
    beta_cm_ = sqrt( 1 - 4*0.938*0.938 / energy / energy);
    gama_cm_ = 1.0 / sqrt( 1.0 - beta_cm_*beta_cm_ );
    out_tree_.WriteDataHeader( colliding_system, energy );
  }
  void Run();
private:
  bool sample_reaction_plane_{false};
  bool boost_to_lab_{false};
  std::string colliding_system_;
  double beta_cm_;
  double gama_cm_;
  InputChain* in_chain_{nullptr};
  OutTreeManager out_tree_;
};

#endif // MCPICO2AT_SRC_CONVERTER_H_
