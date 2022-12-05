//
// Created by mikhail on 7/15/22.
//

/* Copyright (C) 2021 Physikalisches Institut, Eberhard Karls Universitaet Tuebingen, Tuebingen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Viktor Klochkov [committer], Viktor Klochkov [committer] */

/** @brief run_analysistree_qa
 ** @param filelist    Filefist (text file) of input AnalysisTree files
 ** @param is_single_file  if true, instead of filelist a single ROOT file will be used as input
 **
 ** Macro to run AnalysisTreeQA package (https://github.com/HeavyIonAnalysis/AnalysisTreeQA)
 ** Produces an output ROOT file with specified histograms / TProfiles.
 ** Examples how to add plots could be found here:
 ** https://github.com/HeavyIonAnalysis/AnalysisTreeQA/blob/master/examples/example.cpp
 ** To add event cuts:
 ** task->SetEventCuts(EventCuts);
 ** where EventCuts is AnalysisTree::Cuts* object, for example
 ** GetCbmEventCuts("RecEventHeader") from macro/analysis/common/cuts/cbm_cuts.h
 ** To apply a cut on some branch, for example select only primiry MC-tracks:
 ** task->AddBranchCut(GetCbmMcTracksCuts("SimParticles"));
 ** or apply quality cuts on STS tracks:
 ** task->AddBranchCut(GetCbmTrackCuts("RecTracks"));
 **/

using namespace AnalysisTree;

void ParticlesQA(QA::Task& task, Cuts* cuts=nullptr);
void EventHeaderQA(QA::Task& task);

Int_t GetIonCharge(int pdgCode) {
  const int kPdgLambda = 10000000;
  const int kPdgCharge = 10000;
  const int kPdgMass   = 10;
  return (pdgCode % kPdgLambda) / kPdgCharge;
}

void run_qa(std::string filelist, std::string output_file="bmn_qa.root", bool is_single_file = false);

const std::string particles       = "particles";
const std::string event_header       = "event_header";



void run_qa(std::string filelist, std::string output_file, bool is_single_file)
{
  if (is_single_file) {
    std::ofstream fl("fl_temp.txt");
    fl << filelist << "\n";
    fl.close();
    filelist = "fl_temp.txt";
  }

  TaskManager* man = TaskManager::GetInstance();

  auto* task = new QA::Task;
  task->SetOutputFileName(output_file);

  ParticlesQA(*task);
  std::vector<int> pid_codes{ 2212, 211, -211, 321, -321 };
  for( auto pid : pid_codes ) {
    ParticlesQA(
        *task,
        new Cuts(std::to_string(pid), {EqualsCut({particles + ".pid"}, pid)}));
  }
  EventHeaderQA(*task);
  man->AddTask(task);

  man->Init({filelist}, {"atree"});
  man->Run(-1);
  man->Finish();

  if (is_single_file) {
    // -----   Finish   -------------------------------------------------------
    std::cout << " Test passed" << std::endl;
    std::cout << " All ok " << std::endl;
  }
}

void ParticlesQA(QA::Task& task, Cuts* cuts)
{
//  AddTrackQA(&task, particles, cuts);

  task.AddH1({"y", {particles, "rapidity"}, {400, -4, 4}}, cuts);
  task.AddH1({"y_{cm}", {particles, "y_cm"}, {400, -4, 4}}, cuts);
  task.AddH1({"#eta", {particles, "eta"}, {600, -6, 6}}, cuts);
  task.AddH1({"p (GeV/c)", {particles, "p"}, {500, 0, 5}}, cuts);
  task.AddH1({"p_{T} (GeV/c)", {particles, "pT"}, {500, 0, 5}}, cuts);
  task.AddH1({"#phi (rad)", {particles, "phi"}, {400, -4, 4}}, cuts);

  task.AddH2({"y", {particles, "rapidity"}, {400, -4, 4}},
             {"p_{T} (GeV/c)", {particles, "pT"}, {500, 0, 5}},
             cuts);
  task.AddH2({"y_{cm}", {particles, "y_cm"}, {400, -4, 4}},
             {"p_{T} (GeV/c)", {particles, "pT"}, {500, 0, 5}},
             cuts);
  task.AddH2({"#eta", {particles, "eta"}, {300, -6, 6}},
             {"p_{T} (GeV/c)", {particles, "pT"}, {500, 0, 5}},
             cuts);
  task.AddH2(
          {"b (fm)", {event_header, "b"}, {200, 0, 20}},
          {"#eta", {particles, "eta"}, {300, -6, 6}},
          cuts );

  Variable v1("v1", {{particles, "phi"}, {event_header, "psi_rp"}}, [](std::vector<double> phi) { return cos(phi[0] - phi[1]); });
  Variable v2("v2", {{particles, "phi"}, {event_header, "psi_rp"}}, [](std::vector<double> phi) { return cos(2 * (phi[0] - phi[1])); });
  task.AddProfile({"#it{y}", {particles, "rapidity"}, {25, 0.0, 2.5}}, {"v_{1}", v1, {}}, cuts);
  task.AddProfile({"#it{y}", {particles, "rapidity"}, {25, 0.0, 2.5}}, {"v_{1}", v2, {}}, cuts);
}

void EventHeaderQA(QA::Task& task) {
  task.AddH1({"b (fm)", {event_header, "b"}, {200, 0, 20}});
  task.AddH1({"#Psi^{RP} (rad)", {event_header, "psi_rp"}, {400, -4, 4}});
  task.AddH1({"M", {event_header, "M"}, {400, 0, 400}});
  task.AddH1({"centrality (5)", {event_header, "centrality"}, {8, 0, 40}});
  task.AddH2(
          {"centrality (5)", {event_header, "centrality"}, {20, 0, 100}},
          {"M", {event_header, "M"}, {400, 0, 400}}
  );
}

int main(int argc, char** argv)
{
  if (argc <= 1) {
    std::cout << "Not enough arguments! Please use:" << std::endl;
    std::cout << "   ./cbm_qa filelist" << std::endl;
    return -1;
  }

  const std::string filelist = argv[1];
  run_qa(filelist);
  return 0;
}

