#include "btag_weighter.hpp"

#include <cmath>

#include "utilities.hpp"

using namespace std;

const vector<BTagEntry::OperatingPoint> BTagWeighter::op_pts_{BTagEntry::OP_LOOSE, BTagEntry::OP_MEDIUM, BTagEntry::OP_TIGHT};
const vector<BTagEntry::JetFlavor> BTagWeighter::flavors_{BTagEntry::FLAV_B, BTagEntry::FLAV_C, BTagEntry::FLAV_UDSG};

namespace{
  template<typename T, typename... Args>
    std::unique_ptr<T> MakeUnique(Args&&... args){
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
  }
}

BTagWeighter::BTagWeighter(string proc, bool is_fast_sim, int year):
  is_fast_sim_(is_fast_sim){

  // currently not in use...
  btag_efficiencies_ = vector<TH3D>(); btag_efficiencies_.resize(op_pts_.size());
  btag_efficiencies_proc_ = vector<TH3D>(); btag_efficiencies_proc_.resize(op_pts_.size());
  calib_full_ = unique_ptr<BTagCalibration>(new BTagCalibration("csvv2", "data/CSVv2_Moriond17_B_H.csv"));
  calib_fast_ = unique_ptr<BTagCalibration>(new BTagCalibration("csvv2_deep", "data/fastsim_csvv2_ttbar_26_1_2017.csv"));
  csv_loose_ = 0.;
  csv_medium_ = 0.;
  csv_tight_ = 0.;

  // setup SFs and WPs depending on the year
  btag_efficiencies_deep_ = vector<TH3D>(); btag_efficiencies_deep_.resize(op_pts_.size());
  btag_efficiencies_deep_proc_ = vector<TH3D>(); btag_efficiencies_deep_proc_.resize(op_pts_.size());
  if (year==2016) {
    calib_deep_full_ = unique_ptr<BTagCalibration>(new BTagCalibration("csvv2_deep", "data/DeepCSV_Moriond17_B_H.csv"));
    calib_deep_fast_ = unique_ptr<BTagCalibration>(new BTagCalibration("csvv2_deep", "data/fastsim_deepcsv_ttbar_26_1_2017.csv"));
    deep_csv_loose_ = 0.2219;
    deep_csv_medium_ = 0.6324;
    deep_csv_tight_ = 0.8958;
  } else if (year==2017) {
    calib_deep_full_ = unique_ptr<BTagCalibration>(new BTagCalibration("csvv2_deep", "data/DeepCSV_94XSF_V3_B_F.csv"));
    calib_deep_fast_ = unique_ptr<BTagCalibration>(new BTagCalibration("csvv2_deep", "data/fastsim_deepcsv_ttbar_26_1_2017.csv")); // *TBD*
    deep_csv_loose_ = 0.1522;
    deep_csv_medium_ = 0.4941;
    deep_csv_tight_ = 0.8001;
  } else {
    calib_deep_full_ = unique_ptr<BTagCalibration>(new BTagCalibration("csvv2_deep", "data/DeepCSV_94XSF_V3_B_F.csv")); // *TBD*
    calib_deep_fast_ = unique_ptr<BTagCalibration>(new BTagCalibration("csvv2_deep", "data/fastsim_deepcsv_ttbar_26_1_2017.csv")); // *TBD*
    deep_csv_loose_ = 0.1241;
    deep_csv_medium_ = 0.4184;
    deep_csv_tight_ = 0.7527;
  }

  if(proc != "tt" && proc != "qcd" && proc != "wjets"){
    ERROR("Process "+proc+" not found. Valid processes are tt, qcd, and wjets.");
  }
  TFile file_eff("data/btagEfficiency.root", "read");
  TFile file_deep("data/btagEfficiency_deep.root", "read");
  TFile file_proc(("data/btagEfficiency_"+proc+".root").c_str(), "read");
  TFile file_deep_proc(("data/btagEfficiency_deep_"+proc+".root").c_str(), "read");

  for(size_t i = 0; i < op_pts_.size(); ++i){
    const auto op = op_pts_.at(i);
    
    readers_full_[op] = MakeUnique<BTagCalibrationReader>(op, "central", vector<string>{"up", "down"});
    readers_full_.at(op)->load(*calib_full_, BTagEntry::FLAV_UDSG, "incl");
    readers_full_.at(op)->load(*calib_full_, BTagEntry::FLAV_C, "comb");
    readers_full_.at(op)->load(*calib_full_, BTagEntry::FLAV_B, "comb");

    readers_deep_full_[op] = MakeUnique<BTagCalibrationReader>(op, "central", vector<string>{"up", "down"});
    readers_deep_full_.at(op)->load(*calib_deep_full_, BTagEntry::FLAV_UDSG, "incl");
    readers_deep_full_.at(op)->load(*calib_deep_full_, BTagEntry::FLAV_C, "comb");
    readers_deep_full_.at(op)->load(*calib_deep_full_, BTagEntry::FLAV_B, "comb");

    readers_fast_[op] = MakeUnique<BTagCalibrationReader>(op, "central", vector<string>{"up", "down"});
    readers_fast_.at(op)->load(*calib_fast_, BTagEntry::FLAV_UDSG, "fastsim");
    readers_fast_.at(op)->load(*calib_fast_, BTagEntry::FLAV_C, "fastsim");
    readers_fast_.at(op)->load(*calib_fast_, BTagEntry::FLAV_B, "fastsim");

    readers_deep_fast_[op] = MakeUnique<BTagCalibrationReader>(op, "central", vector<string>{"up", "down"});
    readers_deep_fast_.at(op)->load(*calib_deep_fast_, BTagEntry::FLAV_UDSG, "fastsim");
    readers_deep_fast_.at(op)->load(*calib_deep_fast_, BTagEntry::FLAV_C, "fastsim");
    readers_deep_fast_.at(op)->load(*calib_deep_fast_, BTagEntry::FLAV_B, "fastsim");

    string hist_eff, hist_deep;
    switch(op){
    case BTagEntry::OP_LOOSE:
      hist_eff = "btagEfficiency_loose";
      hist_deep = "btagEfficiency_deep_loose";
      break;
    case BTagEntry::OP_MEDIUM:
      hist_eff = "btagEfficiency_medium";
      hist_deep = "btagEfficiency_deep_medium";
      break;
    case BTagEntry::OP_TIGHT:
      hist_eff = "btagEfficiency_tight";
      hist_deep = "btagEfficiency_deep_tight";
      break;
    case BTagEntry::OP_RESHAPING:
      hist_eff = "btagEfficiency_reshaping";
      hist_deep = "btagEfficiency_deep_reshaping";
      break;
    default:
      hist_eff = "btagEfficiency";
      hist_deep = "btagEfficiency";
      break;
    }
    
    btag_efficiencies_.at(i) = *static_cast<const TH3D*>(file_eff.Get(hist_eff.c_str()));
    btag_efficiencies_proc_.at(i) = *static_cast<const TH3D*>(file_proc.Get(hist_eff.c_str()));
    btag_efficiencies_deep_.at(i) = *static_cast<const TH3D*>(file_deep.Get(hist_deep.c_str()));
    btag_efficiencies_deep_proc_.at(i) = *static_cast<const TH3D*>(file_deep_proc.Get(hist_deep.c_str()));
  }
}

double BTagWeighter::EventWeight(baby_plus &b, BTagEntry::OperatingPoint op,
                                 const string &bc_full_syst, const string &udsg_full_syst,
                                 const string &bc_fast_syst, const string &udsg_fast_syst,
                                 bool do_deep_csv, bool do_by_proc) const{
  double product = 1.;
  auto n_jets = b.jets_islep().size();
  for(size_t i = 0; i < n_jets; ++i){
    if(!b.jets_islep().at(i)){
      product *= JetBTagWeight(b, i, op,
                               bc_full_syst, udsg_full_syst,
                               bc_fast_syst, udsg_fast_syst,
                               do_deep_csv, do_by_proc);
    }
  }
  return product;
}

double BTagWeighter::EventWeight(baby_plus &b, BTagEntry::OperatingPoint op,
                                 const string &bc_full_syst, const string &udsg_full_syst,
                                 bool do_deep_csv, bool do_by_proc) const{
  double product = 1.;
  auto n_jets = b.jets_islep().size();
  for(size_t i = 0; i < n_jets; ++i){
    if(!b.jets_islep().at(i)){
      product *= JetBTagWeight(b, i, op,
                               bc_full_syst, udsg_full_syst,
                               do_deep_csv, do_by_proc);
    }
  }
  return product;
}

double BTagWeighter::EventWeight(baby_plus &b, const vector<BTagEntry::OperatingPoint> &ops,
                                 const string &bc_full_syst, const string &udsg_full_syst,
                                 bool do_deep_csv, bool do_by_proc) const{
  double product = 1.;
  auto n_jets = b.jets_islep().size();
  for(size_t i = 0; i < n_jets; ++i){
    if(!b.jets_islep().at(i)){
      product *= JetBTagWeight(b, i, ops,
                               bc_full_syst, udsg_full_syst,
                               do_deep_csv, do_by_proc);
    }
  }
  return product;
}

double BTagWeighter::EventWeight(baby_plus &b, const vector<BTagEntry::OperatingPoint> &ops,
                                 const string &bc_full_syst, const string &udsg_full_syst,
                                 const string &bc_fast_syst, const string &udsg_fast_syst,
                                 bool do_deep_csv, bool do_by_proc) const{
  double product = 1.;
  auto n_jets = b.jets_islep().size();
  for(size_t i = 0; i < n_jets; ++i){
    if(!b.jets_islep().at(i)){
      product *= JetBTagWeight(b, i, ops,
                               bc_full_syst, udsg_full_syst,
                               bc_fast_syst, udsg_fast_syst,
                               do_deep_csv, do_by_proc);
    }
  }
  return product;
}

double BTagWeighter::JetBTagWeight(baby_plus &b, size_t ijet, BTagEntry::OperatingPoint op,
                                   const string &bc_full_syst, const string &udsg_full_syst,
                                   const string &bc_fast_syst, const string &udsg_fast_syst,
                                   bool do_deep_csv, bool do_by_proc) const{
  return JetBTagWeight(b, ijet, vector<BTagEntry::OperatingPoint>{op},
                       bc_full_syst, udsg_full_syst,
                       bc_fast_syst, udsg_fast_syst,
                       do_deep_csv, do_by_proc);
}

double BTagWeighter::JetBTagWeight(baby_plus &b, size_t ijet, BTagEntry::OperatingPoint op,
                                   const string &bc_full_syst, const string &udsg_full_syst,
                                   bool do_deep_csv, bool do_by_proc) const{
  return JetBTagWeight(b, ijet, vector<BTagEntry::OperatingPoint>{op},
                       bc_full_syst, udsg_full_syst,
                       "central", "central",
                       do_deep_csv, do_by_proc);
}

double BTagWeighter::JetBTagWeight(baby_plus &b, size_t ijet, const vector<BTagEntry::OperatingPoint> &ops,
                                   const string &bc_full_syst, const string &udsg_full_syst,
                                   bool do_deep_csv, bool do_by_proc) const{
  return JetBTagWeight(b, ijet, ops,
                       bc_full_syst, udsg_full_syst,
                       "central", "central",
                       do_deep_csv, do_by_proc);
}

double BTagWeighter::JetBTagWeight(baby_plus &b, size_t ijet, const vector<BTagEntry::OperatingPoint> &ops,
                                   const string &bc_full_syst, const string &udsg_full_syst,
                                   const string &bc_fast_syst, const string &udsg_fast_syst,
                                   bool do_deep_csv, bool do_by_proc) const{
  // procedure from https://twiki.cern.ch/twiki/bin/view/CMS/BTagSFMethods#1a_Event_reweighting_using_scale
  int hadronFlavour = abs(b.jets_hflavor().at(ijet));
  BTagEntry::JetFlavor flav;
  string full_syst, fast_syst;
  switch(hadronFlavour){
    case 5: flav = BTagEntry::FLAV_B; break;
    case 4: flav = BTagEntry::FLAV_C; break;
    default: flav = BTagEntry::FLAV_UDSG; break;
  }
  switch(flav){
    case BTagEntry::FLAV_B:
    case BTagEntry::FLAV_C:
      full_syst = bc_full_syst;
      fast_syst = bc_fast_syst;
      break;
    case BTagEntry::FLAV_UDSG:
      full_syst = udsg_full_syst;
      fast_syst = udsg_fast_syst;
      break;
    default:
      ERROR("Did not recognize BTagEntry::JetFlavor "+std::to_string(static_cast<int>(flav)));
  }

  vector<float> opcuts;
  for (auto &iop: ops) {
    if (iop==BTagEntry::OP_LOOSE) opcuts.push_back(do_deep_csv ? deep_csv_loose_ : csv_loose_);
    else if (iop==BTagEntry::OP_MEDIUM) opcuts.push_back(do_deep_csv ? deep_csv_medium_ : csv_medium_); 
    else if (iop==BTagEntry::OP_TIGHT) opcuts.push_back(do_deep_csv ? deep_csv_tight_ : csv_tight_);
  }

  float csv = b.jets_csv().at(ijet);
  if (do_deep_csv) 
    csv = b.jets_csvd().at(ijet);

  int tag = -1;
  for (unsigned iop(0); iop<opcuts.size(); iop++) 
    if (csv>opcuts[iop]) tag = iop;

  const map<BTagEntry::OperatingPoint, unique_ptr<BTagCalibrationReader> > *ireaders_full = nullptr;
  ireaders_full = do_deep_csv ? &readers_deep_full_ : &readers_full_;

  const map<BTagEntry::OperatingPoint, unique_ptr<BTagCalibrationReader> > *ireaders_fast = &readers_fast_;
  if (do_deep_csv) ireaders_fast = &readers_deep_fast_;

  double jet_pt = b.jets_pt().at(ijet);
  double jet_eta = b.jets_eta().at(ijet);
  double eff1(1), eff2(0), sf1(1), sf2(1), sf1_fs(1), sf2_fs(1);
  if (tag >= 0){
    BTagEntry::OperatingPoint iop = ops[tag];
    eff1 = GetMCTagEfficiency(hadronFlavour, jet_pt, jet_eta, iop, do_deep_csv, do_by_proc);
    sf1 = ireaders_full->at(iop)->eval_auto_bounds(full_syst, flav, jet_eta, jet_pt);
    if (is_fast_sim_) sf1_fs = ireaders_fast->at(iop)->eval_auto_bounds(fast_syst, flav, jet_eta, jet_pt);
  }
  if (tag < int(ops.size())-1) {
    BTagEntry::OperatingPoint iop = ops[tag+1];
    eff2 = GetMCTagEfficiency(hadronFlavour, jet_pt, jet_eta, iop, do_deep_csv, do_by_proc);
    sf2 = ireaders_full->at(iop)->eval_auto_bounds(full_syst, flav, jet_eta, jet_pt);
    if (is_fast_sim_) sf2_fs = ireaders_fast->at(iop)->eval_auto_bounds(fast_syst, flav, jet_eta, jet_pt);
  }

  double eff1_fs(eff1/sf1_fs), eff2_fs(eff2/sf2_fs);
  double result = (sf1*sf1_fs*eff1_fs-sf2*sf2_fs*eff2_fs)/(eff1_fs-eff2_fs);
  if(std::isnan(result) || std::isinf(result)){
    result = 1.;
    DBG("SF is NaN or inf. Setting to 1.!");
  }
  return result;
}

double BTagWeighter::GetMCTagEfficiency(int pdgId, float pT, float eta,
                                        BTagEntry::OperatingPoint op, bool do_deep_csv, bool do_by_proc) const{
  size_t rdr_idx = distance(op_pts_.cbegin(), find(op_pts_.cbegin(), op_pts_.cend(), op));
  pdgId = abs(pdgId);
  if(pdgId != 4 && pdgId != 5){
    // in the ghost clustering scheme to determine flavor, there are only b, c and other (id=0) flavors
    pdgId = 0;
  }

  int bin;
  float eff;
  if(!do_deep_csv){
    if(!do_by_proc){
      bin = btag_efficiencies_.at(rdr_idx).FindFixBin(fabs(eta), pT, pdgId);
      eff = btag_efficiencies_.at(rdr_idx).GetBinContent(bin);
    }else{
      bin = btag_efficiencies_proc_.at(rdr_idx).FindFixBin(fabs(eta), pT, pdgId);
      eff = btag_efficiencies_proc_.at(rdr_idx).GetBinContent(bin);
    }
  }else{
    if(!do_by_proc){
      bin = btag_efficiencies_deep_.at(rdr_idx).FindFixBin(fabs(eta), pT, pdgId);
      eff = btag_efficiencies_deep_.at(rdr_idx).GetBinContent(bin);
    }else{
      bin = btag_efficiencies_deep_proc_.at(rdr_idx).FindFixBin(fabs(eta), pT, pdgId);
      eff = btag_efficiencies_deep_proc_.at(rdr_idx).GetBinContent(bin);
    }
  }
  
  return eff;
}
