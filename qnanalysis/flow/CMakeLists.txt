add_executable(flow tracks_processor.cc tracks_processor.h main.cc)
target_link_libraries(flow ROOT::EG ROOT::ProofBench AnalysisTreeBase AnalysisTreeInfra Boost::program_options)