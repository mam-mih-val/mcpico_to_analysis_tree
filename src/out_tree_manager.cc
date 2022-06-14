//
// Created by mikhail on 6/14/22.
//

#include "out_tree_manager.h"

OutTreeManager::OutTreeManager(const std::string &out_file_name,
                     const std::string &out_tree_name) {
  using namespace AnalysisTree;
  out_file_ = TFile::Open( out_file_name.c_str(), "recreate" );
  out_file_->cd();
  out_tree_ = new TTree( out_tree_name.c_str(), "Analysis Tree with MC data" );

  configuration_ = new Configuration();
  data_header_ = new DataHeader();

  event_header_config_ = BranchConfig( "event_header", DetType::kEventHeader );
  event_header_config_.AddField<float>( "b", "impact parameter" );
  event_header_config_.AddField<float>( "psi_rp", "reaction plane" );
  event_header_config_.AddField<int>( "M", "number of produced in the collision particles" );

  configuration_->AddBranchConfig(event_header_config_);
  event_header_ = new EventHeader( configuration_->GetBranchConfig("event_header").GetId() );
  event_header_->Init( event_header_config_ );

  particles_config_ = BranchConfig( "particles", DetType::kParticle );
  particles_ = new Particles;
  configuration_->AddBranchConfig(particles_config_);

  out_tree_->Branch("event_header", "AnalysisTree::EventHeader",&event_header_);
  out_tree_->Branch("particles", "AnalysisTree::Particles", &particles_);

  configuration_->Write("Configuration");
  data_header_->Write("DataHeader");
}

OutTreeManager::~OutTreeManager() {
  out_file_->cd();
  out_tree_->Write();
  out_file_->Close();

  delete configuration_;
  delete data_header_;
  delete event_header_;
  delete particles_;
}
