//
// Created by mikhail on 6/16/22.
//

#ifndef MCPICO2AT_SRC_MC_PICO_CHAIN_CC_INPUT_CHAIN_H_
#define MCPICO2AT_SRC_MC_PICO_CHAIN_CC_INPUT_CHAIN_H_

#include <TChain.h>
#include <TVector3.h>
class InputChain {
public:
  explicit InputChain(const std::string& input_file_list,
                      const std::string& input_tree_name);
  virtual ~InputChain() = default;

  void AddFiles(const std::string& file_list);
  bool NextEvent();

  [[nodiscard]] virtual float GetImpactParameter() const { return 0.0; }
  [[nodiscard]] virtual float GetReactionPlain() const { return 0.0; }
  [[nodiscard]] virtual int GetNParticles() const { return 0.0; }
  virtual std::tuple<float, float, float>  GetMomentum(int idx){ return { 0.0, 0.0, 0.0 }; }
  virtual float GetPx( int idx ){return 0.0;}
  virtual float GetPy( int idx ){return 0.0;}
  virtual float GetPz( int idx ){return 0.0;}
  virtual int GetPdgCode(int idx){ return 0; }
  virtual int GetProductionRegion(int idx){ return 0; }

protected:
  TChain* chain_;
  long long current_event_=-1;
};

#endif // MCPICO2AT_SRC_MC_PICO_CHAIN_CC_INPUT_CHAIN_H_
