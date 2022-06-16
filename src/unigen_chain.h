//
// Created by mikhail on 6/16/22.
//

#ifndef MCPICO2AT_SRC_INIGEN_CHAIN_H_
#define MCPICO2AT_SRC_INIGEN_CHAIN_H_

#include "input_chain.h"
#include "unigen_format/UEvent.h"
#include "unigen_format/UParticle.h"

class UnigenChain : public InputChain {
public:
  UnigenChain(const std::string &input_file_list,
              const std::string &input_tree_name);
  ~UnigenChain() override = default;
  [[nodiscard]] float GetImpactParameter() const override {
    return float(event_->GetB());
  }
  [[nodiscard]] float GetReactionPlain() const override {
    return float(event_->GetPhi());
  }
  [[nodiscard]] int GetNParticles() const override {
    return event_->GetNpa();
  }
  std::tuple<float, float, float> GetMomentum(int idx) override {
    auto* p = event_->GetParticle(idx);
    return { float(p->Px()), float(p->Py()), float(p->Pz() )};
  }
  float GetPx(int idx) override {
    auto*p = event_->GetParticle(idx);
    return float(p->Px());
  }
  float GetPy(int idx) override {
    auto*p = event_->GetParticle(idx);
    return float(p->Py());
  }
  float GetPz(int idx) override {
    auto*p = event_->GetParticle(idx);
    return float(p->Pz());
  }
  int GetPdgCode(int idx) override {
    auto*p = event_->GetParticle(idx);
    return p->GetPdg();
  }
  int GetProductionRegion(int idx) override {
    auto*p = event_->GetParticle(idx);
    return int( p->T() );
  }

protected:
  UEvent* event_{nullptr};
};

#endif // MCPICO2AT_SRC_INIGEN_CHAIN_H_
