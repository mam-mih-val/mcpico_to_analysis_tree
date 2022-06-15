//
// Created by mikhail on 6/14/22.
//

#include <TDatabasePDG.h>
#include <random>
#include "converter.h"
Converter::Converter(const std::string& input_file_list, const std::string& out_file_name,
                     const std::string& out_tree_name) :
                                                         in_chain_(input_file_list),
                                                         out_tree_(out_file_name, out_tree_name){}
void Converter::Run() {
  auto out_event_header = out_tree_.GetEventHeader();
  auto out_event_header_config = out_tree_.GetEventHeaderConfig();
  auto out_particles = out_tree_.GetParticles();
  auto out_particles_config = out_tree_.GetParticlesConfig();

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_real_distribution<> dist(-M_PI, M_PI);

  int n_events=0;
  while( in_chain_.NextEvent() ){
    out_particles->ClearChannels();

    auto b = in_chain_.GetImpactParameter();
    auto model_reaction_plane = in_chain_.GetReactionPlain();
    auto sampled_reaction_plane = float( dist( rng ) );
    auto n_part = in_chain_.GetNParticles();

    out_event_header->SetField( b, out_event_header_config.GetFieldId( "b" ) );
    out_event_header->SetField(sample_reaction_plane_ ? sampled_reaction_plane : model_reaction_plane, out_event_header_config.GetFieldId( "psi_rp" ) );
    out_event_header->SetField( n_part, out_event_header_config.GetFieldId( "M" ) );
    for( int i=0; i<n_part; ++i ){
      auto [px, py, pz] = in_chain_.GetMomentum(i);
      if( sample_reaction_plane_ ){
        // Rotation back to Psi_RP == 0
        auto new_px =   px*cos(model_reaction_plane) + py*sin(model_reaction_plane);
        auto new_py = - px*sin(model_reaction_plane) + py*cos(model_reaction_plane);
        // Rotation to sampled Psi_RP
        px = new_px*cos( sampled_reaction_plane) - new_py*sin(sampled_reaction_plane);
        py = new_px*sin( sampled_reaction_plane) + new_py*cos(sampled_reaction_plane);
      }
      auto pid = in_chain_.GetPdgCode(i);
      auto mass = TDatabasePDG::Instance()->GetParticle(pid)->Mass();
      if( boost_to_lab_ ){
        pz = gama_cm_*( pz + beta_cm_*sqrt( px*px + py*py + pz*pz + mass*mass ) );
      }
      auto& particle = out_particles->AddChannel(out_particles_config);
      particle.SetMomentum( float(px), float(py), float(pz) );
      particle.SetPid( pid );
      particle.SetMass( float(mass) );
    }
    out_tree_.Fill();
    out_tree_.CheckIfNewFile();
    n_events++;
  }
  std::cout << n_events << " events were converted to AnalysisTree" << std::endl;
}
