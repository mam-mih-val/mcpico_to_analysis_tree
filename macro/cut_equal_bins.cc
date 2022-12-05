//
// Created by mikhail on 11/11/20.
//

#include <TFile.h>
#include <TH1D.h>
#include <iostream>

int main(int n_args, char** args){
  if( n_args < 5 )
    throw std::runtime_error("4 argumets expected. Try: ./equal_bins path/to/file.root histogram_name n_bins output_file");
  auto file = TFile::Open( args[1], "read" );
  if( !file )
    throw std::runtime_error("Couldn't open the file "+std::string(args[1]));
  TH1F* histo;
  file->GetObject( args[2], histo );
  if( !histo )
    throw std::runtime_error("Couldn't read the histogram "+std::string(args[2])+" from file "+std::string(args[1]));
  int n_bins = atoi(args[3]);
  double part = 1.0/n_bins;
  auto entries = histo->GetEntries();
  int bin_enries=0;
  double sum_Nx=0;
  std::vector<double> new_axis;
  std::vector<double> means;
  new_axis.push_back( histo->GetXaxis()->GetBinLowEdge(1) );
  for( auto i=histo->GetNbinsX(); i > 0; --i ){
    bin_enries+=histo->GetBinContent(i);
    sum_Nx+=histo->GetBinCenter(i)*histo->GetBinContent(i);
    if( bin_enries < part*entries )
      continue;
    else{
      new_axis.push_back(histo->GetBinLowEdge(i-1));
      means.push_back( sum_Nx/ (double) bin_enries );
      bin_enries=0;
      sum_Nx=0;
    }
  }
  new_axis.push_back( histo->GetXaxis()->GetBinUpEdge(histo->GetNbinsX()) );
  means.push_back( sum_Nx/ (double) bin_enries );
  std::cout << "edges: { ";
  for( auto num : new_axis ){
    std::cout << num << ", " ;
  }
  std::cout << "\b\b }" << std::endl;

  std::cout << "means: { ";
  for( auto num : means ){
    std::cout << num << ", " ;
  }
  std::cout << "\b\b }" << std::endl;
  auto result_histo = new TH1F( "new_histogram", "", new_axis.size()-1, new_axis.data() );
  int i=0;
  for( auto num : means ){
    i++;
    result_histo->SetBinContent(i, num);
  }
  auto file_out = TFile::Open(args[4], "recreate");
  result_histo->Write();
  file_out->Close();
  return 0;
}