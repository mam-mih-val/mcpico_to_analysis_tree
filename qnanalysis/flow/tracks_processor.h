//
// Created by mikhail on 11/30/20.
//

#ifndef HADES_RAPIDITY_SRC_RAPIDITY_H_
#define HADES_RAPIDITY_SRC_RAPIDITY_H_

#include <TFile.h>
#include <TTree.h>
#include <TH2F.h>
#include "TProfile.h"


#include <AnalysisTree/AnalysisTask.hpp>

#include <AnalysisTree/Matching.hpp>
#include <AnalysisTree/Detector.hpp>
#include <AnalysisTree/Particle.hpp>
#include <AnalysisTree/BranchConfig.hpp>
#include <AnalysisTree/EventHeader.hpp>

#include <memory>
#include <string>

namespace AnalysisTree {

class TracksProcessor : public Task {
public:
  TracksProcessor() = default;
  ~TracksProcessor() override = default;
  void Init() override;
  void Exec() override;
  void Finish() override {
    v2_vs_b_ones_->Write();
    v2_vs_b_pT_->Write();
  }

protected:
  void LoopSimParticles();

private:
  bool is_mc_ = true;

  Branch in_sim_particles_;
  Branch in_event_header_;

  TProfile* v2_vs_b_ones_ = nullptr;
  TProfile* v2_vs_b_pT_ = nullptr;
};

}
#endif // HADES_RAPIDITY_SRC_RAPIDITY_H_
