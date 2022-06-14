//
// Created by mikhail on 6/14/22.
//

#include "converter.h"
Converter::Converter(const std::string& input_file_list, const std::string& out_file_name,
                     const std::string& out_tree_name) :  in_chain_(input_file_list), out_tree_( out_file_name, out_tree_name ) {
}
void Converter::Run() {
  auto out_event_header = out_tree_.GetEventHeader();
  auto out_event_header_config = out_tree_.GetEventHeaderConfig();
  auto out_particles = out_tree_.GetParticles();
  auto out_particles_config = out_tree_.GetParticles();

  while( in_chain_.NextEvent() ){
    auto b = in_chain_.GetImpactParameter();
    auto rp = in_chain_.GetReactionPlain();
    auto n_part = in_chain_.GetNParticles();

    out_event_header->SetField( b, out_event_header_config.GetFieldId( "b" ) );
    out_event_header->SetField( rp, out_event_header_config.GetFieldId( "psi_rp" ) );
    out_event_header->SetField( n_part, out_event_header_config.GetFieldId( "M" ) );

//    out_tree_.GetOutFile()->cd();
    out_tree_.Fill();
  }
}
