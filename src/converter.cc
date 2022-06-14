//
// Created by mikhail on 6/14/22.
//

#include <TDatabasePDG.h>
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

  while( in_chain_.NextEvent() ){
    out_particles->ClearChannels();

    auto b = in_chain_.GetImpactParameter();
    auto rp = in_chain_.GetReactionPlain();
    auto n_part = in_chain_.GetNParticles();

    out_event_header->SetField( b, out_event_header_config.GetFieldId( "b" ) );
    out_event_header->SetField( rp, out_event_header_config.GetFieldId( "psi_rp" ) );
    out_event_header->SetField( n_part, out_event_header_config.GetFieldId( "M" ) );
    for( int i=0; i<n_part; ++i ){
      auto mom3 = in_chain_.GetMomentum(i);
      auto pid = in_chain_.GetPdgCode(i);
      auto mass = TDatabasePDG::Instance()->GetParticle(pid)->Mass();
      auto particle = out_particles->AddChannel(out_particles_config);
      particle.SetMomentum3( mom3 );
      particle.SetPid( pid );
      particle.SetMass( float(mass) );
    }
    out_tree_.Fill();
  }
}
