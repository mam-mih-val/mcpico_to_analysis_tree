//
// Created by mikhail on 6/14/22.
//

#include "out_tree_manager.h"

OutTreeManager::OutTreeManager(const std::string &out_file_name,
                     const std::string &out_tree_name) {
//  using namespace AnalysisTree;
  out_file_ = TFile::Open( out_file_name.c_str(), "recreate" );
  out_tree_ = new TTree( out_tree_name.c_str(), "Analysis Tree with MC data" );

  event_header_config_ = AnalysisTree::BranchConfig( "event_header", AnalysisTree::DetType::kEventHeader );
  event_header_config_.AddField<float>( "b", "impact parameter" );
  event_header_config_.AddField<float>( "b_norm", "normalized impact parameter" );
  event_header_config_.AddField<float>( "psi_rp", "reaction plane" );
  event_header_config_.AddField<float>( "centrality", "Impact parameter centrality" );
  event_header_config_.AddField<int>( "M", "number of produced in the collision particles" );

  configuration_.AddBranchConfig(event_header_config_);
  event_header_ = new AnalysisTree::EventHeader( event_header_config_.GetId() );
  event_header_->Init( event_header_config_ );

  particles_config_ = AnalysisTree::BranchConfig( "particles", AnalysisTree::DetType::kParticle );
  particles_config_.AddField<float>("y_cm", "Rapidity in center-of-mass system");
  particles_config_.AddField<float>("Ekin", "Kinetic energy of a particle");
  particles_config_.AddField<int>("type", "0 is for produced and +/- 1 for spectators");
  configuration_.AddBranchConfig(particles_config_);
  particles_ = new AnalysisTree::Particles( particles_config_.GetId() );;

  out_tree_->Branch("event_header", "AnalysisTree::EventHeader",&event_header_);
  out_tree_->Branch("particles", "AnalysisTree::Particles", &particles_);

  out_file_->cd();
  configuration_.Write("Configuration");
}

OutTreeManager::~OutTreeManager() {
  out_file_->cd();
  out_tree_->Write();
  out_file_->Close();

  delete event_header_;
  delete particles_;
}
void OutTreeManager::WriteDataHeader(const std::string &colliding_system,
                                     double energy) {
  data_header_.SetSystem(colliding_system);
  const double M=0.940;
  auto p_mom = energy * sqrt( energy*energy / ( 4*M*M ) - 1 );
  data_header_.SetBeamMomentum(p_mom);
  data_header_.Write("DataHeader");

}
void OutTreeManager::CheckIfNewFile() {
  auto* file = out_tree_->GetCurrentFile();
  if( file != out_file_ ) {
    out_file_ = file;
    out_file_->cd();
    configuration_.Write("Configuration");
    data_header_.Write("DataHeader");
  }
}
