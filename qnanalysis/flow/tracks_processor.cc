//
// Created by mikhail on 11/30/20.
//

#include "tracks_processor.h"

#include <random>

#include "TLorentzVector.h"

#include "TaskManager.hpp"
#include <AnalysisTree/DataHeader.hpp>

namespace AnalysisTree {

void TracksProcessor::Init() {
  auto man = TaskManager::GetInstance();
  auto chain = man->GetChain();

  AddInputBranch("particles");
  AddInputBranch("event_header");

//  AddInputBranch("SimParticles2GlobalTracks");
  in_sim_particles_ = chain->GetBranch("particles");
  in_sim_particles_.Freeze();
  in_event_header_ = chain->GetBranch("event_header");
  in_event_header_.Freeze();

  v2_vs_b_ones_ = new TProfile( "v2_vs_b_ones_", ";b (fm); v2", 15, 0, 15.0 );
  v2_vs_b_pT_ = new TProfile( "v2_vs_b_pT_", ";b (fm); v2", 15, 0, 15.0 );
}

void TracksProcessor::Exec() {
  this->LoopSimParticles();
}

void TracksProcessor::LoopSimParticles() {
  using AnalysisTree::Particle;

  auto field_phi = in_sim_particles_.GetField("phi");
  auto field_pT = in_sim_particles_.GetField("pT");
  auto field_y_cm = in_sim_particles_.GetField("y_cm");
  auto field_pid = in_sim_particles_.GetField("pid");

  auto field_b = in_event_header_.GetField("b");
  auto field_psi_rp = in_event_header_.GetField("b");

  auto b = in_event_header_.GetDataRaw<EventHeader*>()->GetField<float>(field_b.GetFieldId());
  auto psi_rp = in_event_header_.GetDataRaw<EventHeader*>()->GetField<float>(field_psi_rp.GetFieldId());

  for (size_t i=0; i<in_sim_particles_.size(); ++i) {
    auto in_particle = in_sim_particles_[i];

    if( in_particle[field_y_cm] < 0.0 )
      continue;
    if( in_particle[field_y_cm] > 0.75 )
      continue;
    if( in_particle[field_pT] > 2.0 )
      continue;
    if( in_particle[field_pid] != 2212 )
      continue;

    auto pT = in_particle[field_pT];
    auto phi = in_particle[field_phi];

    v2_vs_b_ones_->Fill( b, cos( 2*(phi - psi_rp) ) );
    v2_vs_b_pT_->Fill( b, cos( 2*(phi - psi_rp) ), pT );
  }
}

} // namespace AnalysisTree