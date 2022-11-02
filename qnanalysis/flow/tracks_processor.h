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
  void SetOutFileName(const std::string &file_out_name) {
    out_file_name_ = file_out_name;
  }
  void Finish() override {
    out_file_ = TFile::Open(out_file_name_.c_str(), "recreate" );
    out_file_->cd();
    v1_vs_b_ones_->Write();
    v1_vs_b_pT_->Write();
    out_file_->Close();
  }

protected:
  void LoopSimParticles();

private:
  bool is_mc_ = true;

  std::string out_file_name_{"out.root"};
  TFile* out_file_{nullptr};

  Branch in_sim_particles_;
  Branch in_event_header_;

  TProfile* v1_vs_b_ones_ = nullptr;
  TProfile* v1_vs_b_pT_ = nullptr;
};

}
#endif // HADES_RAPIDITY_SRC_RAPIDITY_H_
