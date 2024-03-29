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
    sqrt_snn_ = energy;
    beta_cm_ = sqrt( 1 - 4*0.938*0.938 / energy / energy);
    gama_cm_ = 1.0 / sqrt( 1.0 - beta_cm_*beta_cm_ );
    out_tree_.WriteDataHeader( colliding_system, energy );
  }
  void Run();
private:
  std::vector<int> auau_mult_edges_{ 0, 240, 172, 120, 80, 48, 24, 8, 0 };
  std::vector<float> auau_b_edges_{ 0, 3.888, 5.67, 6.966, 8.1, 9.072, 10.044, 10.854, 11.664, 12.474, 16.2 };
  std::vector<float> b_edges_;
  std::vector<float> mult_edges_;
  std::vector<float> centrality_percentage_{0, 5, 10, 15, 20, 25, 30, 35, 40, 50, 60, 70, 80, 90, 100};
  ;
  bool sample_reaction_plane_{false};
  bool boost_to_lab_{false};
  std::string colliding_system_;
  double sqrt_snn_;
  double beta_cm_;
  double gama_cm_;
  double nucleus_radius_;
  InputChain* in_chain_{nullptr};
  OutTreeManager out_tree_;
};

#endif // MCPICO2AT_SRC_CONVERTER_H_
