//
// Created by mikhail on 6/14/22.
//

#ifndef MCPICO2AT_SRC_MC_PICO_CHAIN_H_
#define MCPICO2AT_SRC_MC_PICO_CHAIN_H_

#include <TChain.h>
#include <TVector3.h>

class McPicoChain {
public:
  McPicoChain() = default;
  explicit McPicoChain(const std::string& input_file_list){
    this->Init();
    this->AddFiles(input_file_list);
  };
  ~McPicoChain() = default;
  void Init();
  void AddFiles(const std::string& file_list);
  bool NextEvent();

  [[nodiscard]] float GetImpactParameter() const { return impact_parameter_; }
  [[nodiscard]] float GetReactionPlain() const { return reaction_plain_; }
  [[nodiscard]] int GetNParticles() const { return n_particles_; }
  TVector3 GetMomentum(int idx){ return { px_[idx], py_[idx], pz_[idx] }; };
  int GetPdgCode(int idx){ return pdg_code_[idx]; };

private:
  TChain* chain_;
  long long current_event_=-1;
  float impact_parameter_;
  float reaction_plain_;
  int n_particles_;
  float px_[389];
  float py_[389];
  float pz_[389];
  int pdg_code_[389];
};

#endif // MCPICO2AT_SRC_MC_PICO_CHAIN_H_
