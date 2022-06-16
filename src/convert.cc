//
// Created by mikhail on 6/14/22.
//

#include "converter.h"
#include <boost/program_options.hpp>

int main(int argv, char** argc){
  namespace po = boost::program_options;

  if( argv < 2 ){
    throw std::runtime_error( "Incorrect number of argument. Type ./convert --help to see options" );
  }

  std::string input_list;
  std::string input_tree;
  std::string output_file;
  std::string output_tree;
  std::string system;
  double energy; // Sqrt snn
  bool sample_reaction_plane;
  bool boost_to_lab;
  bool is_unigen;
  bool is_mcpico;

  po::options_description options("Options");
  options.add_options()
      ("help,h", "Help screen")
      ("input,i", po::value<std::string>(&input_list),"Input file list")
      ("output,o", po::value<std::string>(&output_file)->default_value("output.root"),"Name of output file")
      ("input-tree", po::value<std::string>(&input_tree)->default_value("mctree"),"Name of input tree")
      ("output-tree", po::value<std::string>(&output_tree)->default_value("atree"),"Name of output tree")
      ("system,s", po::value<std::string>(&system)->default_value("Au+Au"),"Collision system")
      ("energy,e", po::value<double>(&energy)->default_value(3.3),"Collision energy per nucleon pair")
      ("sample-reaction-plane","The reaction plane angle will be sampled")
      ("boost-to-lab","The output particles in the tree will have boosted along z-axis values")
      ("mcpico","The input is in mcpico format")
      ("unigen","The input is in unigen format");
  po::variables_map vm;
  po::parsed_options parsed = po::command_line_parser(argv, argc).options(options).run();
  po::store(parsed, vm);
  po::notify(vm);
  if (vm.count("help")){
    std::cout << options << std::endl;
    return 0;
  }
  sample_reaction_plane = vm.count("sample-reaction-plane");
  boost_to_lab = vm.count("boost-to-lab");
  is_mcpico = vm.count("mcpico");
  is_unigen = vm.count("unigen");

  if( is_mcpico && is_unigen )
    throw std::runtime_error( "You cannot specify both unigen and mcini input data formats simultaneously" );
  if( !is_mcpico && !is_unigen )
    throw std::runtime_error( "Specify the data format of the input tree" );

  auto converter = Converter( output_file, output_tree );
  if( is_mcpico )
    converter.SetInChain( new McPicoChain(input_list, input_tree) );
  if( is_unigen )
    converter.SetInChain( new UnigenChain(input_list, input_tree) );
  converter.SampleReactionPlane(sample_reaction_plane);
  converter.BoostToLab(boost_to_lab);
  converter.SetCollidingSystem( system, energy );
  converter.Run();
  return 0;
}