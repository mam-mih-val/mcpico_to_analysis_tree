//
// Created by mikhail on 6/14/22.
//

#include <TDatabasePDG.h>
#include <random>
#include "converter.h"
#include "input_chain.h"
Converter::Converter(const std::string& out_file_name, const std::string& out_tree_name) :
                                                         out_tree_(out_file_name, out_tree_name){}
void Converter::Run() {
  auto out_event_header = out_tree_.GetEventHeader();
  auto out_event_header_config = out_tree_.GetEventHeaderConfig();
  auto out_particles = out_tree_.GetParticles();
  auto out_particles_config = out_tree_.GetParticlesConfig();

  auto b_id = out_event_header_config.GetFieldId( "b" );
  auto b_norm_id = out_event_header_config.GetFieldId( "b_norm" );
  auto centrality_id = out_event_header_config.GetFieldId( "centrality" );
  auto psi_rp_id = out_event_header_config.GetFieldId( "psi_rp" );
  auto M_id = out_event_header_config.GetFieldId( "M");

  auto y_cm_id = out_particles_config.GetFieldId("y_cm");
  auto Ekin_id = out_particles_config.GetFieldId("Ekin");
  auto type_id = out_particles_config.GetFieldId("type");

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<float> dist(-M_PI, M_PI);

  if( colliding_system_ == "Au+Au" ){
    b_edges_ = { 0, 3.888, 5.67, 6.966, 8.1, 9.072, 10.044, 10.854, 11.664, 12.474, 16.2 };
    if( fabs(sqrt_snn_ - 2.4) < 1e-2 )
      mult_edges_ = {
            /* 0-5% */  124,
            /* 5-10% */ 103,
            /* 10-15% */ 86,
            /* 15-20% */ 72,
            /* 20-25% */ 60,
            /* 25-30% */ 50,
            /* 30-35% */ 41,
            /* 35-40% */ 33
    };
    nucleus_radius_ = 1.2 * pow( 197, 1.0/3.0 );
  } else if( colliding_system_ == "Xe+Cs" ){
    b_edges_ = { 0, 3.608, 5.248, 6.56, 7.708, 8.692, 9.512, 10.332, 11.152, 12.3, 16.4 };
    nucleus_radius_ = 1.2 * pow( 131, 1.0/3.0 );
  } else if( colliding_system_ == "Ag+Ag" ){
    b_edges_ = {  0, 3.12, 4.55, 5.59, 6.5, 7.28, 8.06, 8.71, 9.36, 10.14, 13  };

    if( fabs(sqrt_snn_ - 2.4) < 1e-2 )
      mult_edges_ = {
              /* 0-5% */  69,
              /* 5-10% */ 57,
              /* 10-15% */ 48,
              /* 15-20% */ 40,
              /* 20-25% */ 33,
              /* 25-30% */ 27,
              /* 30-35% */ 22,
              /* 35-40% */ 17
      };
    if( fabs(sqrt_snn_ - 2.5) < 1e-2 )
      mult_edges_ = {
              /* 0-5% */  67,
              /* 5-10% */ 55,
              /* 10-15% */ 46,
              /* 15-20% */ 38,
              /* 20-25% */ 31,
              /* 25-30% */ 25,
              /* 30-35% */ 20,
              /* 35-40% */ 16
      };
    nucleus_radius_ = 1.2 * pow( 108, 1.0/3.0 );
  }

  int n_events=0;
  while( in_chain_->NextEvent() ){
    out_particles->ClearChannels();

    auto b = in_chain_->GetImpactParameter();
    auto b_norm = static_cast<float>(b / nucleus_radius_);
    auto model_reaction_plane = in_chain_->GetReactionPlain();
    auto sampled_reaction_plane = dist( rng );
    auto n_part = in_chain_->GetNParticles();

    auto centrality = -1.0f;
    int idx = 0;
    float bin_edge = !b_edges_.empty() ? b_edges_[idx] : 0;
    while( b > bin_edge &&
           idx < b_edges_.size()-1 ){
      idx++;
      bin_edge = b_edges_[idx];
    }
    centrality = static_cast<float>( 2*idx - 1 )*10/2.0f;

    out_event_header->SetField( b, b_id );
    out_event_header->SetField( b_norm, b_norm_id );
    out_event_header->SetField( centrality, centrality_id );
    out_event_header->SetField(sample_reaction_plane_ ? sampled_reaction_plane : model_reaction_plane, psi_rp_id );
    int multiplicity = 0;
    for( int i=0; i<n_part; ++i ){
      auto [px, py, pz] = in_chain_->GetMomentum(i);
      if( sample_reaction_plane_ ){
        // Rotation back to Psi_RP == 0
        auto new_px =   px*cosf(model_reaction_plane) + py*sinf(model_reaction_plane);
        auto new_py = - px*sinf(model_reaction_plane) + py*cosf(model_reaction_plane);
        // Rotation to sampled Psi_RP
        px = new_px*cosf( sampled_reaction_plane) - new_py*sinf(sampled_reaction_plane);
        py = new_px*sinf( sampled_reaction_plane) + new_py*cosf(sampled_reaction_plane);
      }
      auto pid = in_chain_->GetPdgCode(i);
      auto type = in_chain_->GetProductionRegion(i);
      double mass = 0.0;
      int charge = 0;
      if( TDatabasePDG::Instance()->GetParticle(pid) ) {
        mass = TDatabasePDG::Instance()->GetParticle(pid)->Mass();
        charge = static_cast<int>( TDatabasePDG::Instance()->GetParticle(pid)->Charge() / 3.0 );
      }
      else{
        // PDG Nuclei numbering scheme: +-10L'ZZZ'AAA'I ( L -number of Lambda, Z = np, A = np+nn, I=0 in case of ground state )
        int A = (pid % 10'000) / 10;
        mass = 0.931 * A;
        charge = (pid % 10'000'000) / 10'000;
      }
      auto E = sqrt( px*px + py*py + pz*pz + mass*mass );
      auto y_cm = 0.5*( log( E + pz ) - log(E - pz) );
      if( boost_to_lab_ ){
        pz = gama_cm_*( pz + beta_cm_*E );
      }
      auto p = sqrt(px*px + py*py + pz*pz); // full momentum
      auto eta = atanh( pz / p ); // pseudorapidity
      E = sqrt( px*px + py*py + pz*pz + mass*mass );
      auto Ekin = E - mass;
      auto& particle = out_particles->AddChannel(out_particles_config);
      particle.SetMomentum(
              static_cast<float>(px),
              static_cast<float>(py),
              static_cast<float>(pz) );
      particle.SetPid( pid );
      particle.SetMass( static_cast<float>(mass) );

      particle.SetField(static_cast<float>(y_cm), y_cm_id);
      particle.SetField(static_cast<float>(Ekin), Ekin_id);
      particle.SetField(static_cast<int>(type), type_id);

      if(  eta < -1.0 )
        continue;
      if( eta > 1.0 )
        continue;
      if( charge == 0 )
        continue;
      multiplicity++; // counting only charged particles
    }
    out_event_header->SetField( multiplicity, M_id);

    out_tree_.Fill();
    out_tree_.CheckIfNewFile();
    n_events++;
  }
  std::cout << n_events << " events were converted to AnalysisTree" << std::endl;
}
