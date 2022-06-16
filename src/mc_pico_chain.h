//
// Created by mikhail on 6/14/22.
//

#ifndef MCPICO2AT_SRC_MC_PICO_CHAIN_H_
#define MCPICO2AT_SRC_MC_PICO_CHAIN_H_

#include <TChain.h>
#include <TVector3.h>
#include "input_chain.h"

class McPicoChain : public InputChain {
public:
  McPicoChain(const std::string &input_file_list,
              const std::string &input_tree_name);
  ~McPicoChain() override = default;
  [[nodiscard]] float GetImpactParameter() const override { return impact_parameter_; };
  [[nodiscard]] float GetReactionPlain() const override { return reaction_plain_; };
  [[nodiscard]] int GetNParticles() const override {return n_particles_; };

  std::tuple<float, float, float> GetMomentum(int idx) override {return {px_[idx], py_[idx], pz_[idx]};};
  float GetPx(int idx) override {return px_[idx];};
  float GetPy(int idx) override {return py_[idx];};
  float GetPz(int idx) override {return pz_[idx];};
  int GetPdgCode(int idx) override {return pdg_code_[idx];};
  int GetProductionRegion(int idx) override {
    return InputChain::GetProductionRegion(idx);
  }

protected:
  float impact_parameter_;
  int n_particles_;
  int pdg_code_[1000];
  float px_[1000];
  float py_[1000];
  float pz_[1000];
  float reaction_plain_;
};

#endif // MCPICO2AT_SRC_MC_PICO_CHAIN_H_
