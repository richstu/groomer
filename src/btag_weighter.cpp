#include "btag_weighter.hpp"

#include "TH3D.h"

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

BTagWeighter::BTagWeighter(bool is_fast_sim, bool is_cmssw_7):
  calib_full_(new BTagCalibration("csvv2", "data/CSVv2Moriond17_comb.csv")),
  calib_fast_(new BTagCalibration("csvv2_deep", "data/CSV_13TEV_Combined_14_7_2016.csv")),
  readers_full_(),
  readers_fast_(),
  btag_efficiencies_(op_pts_.size(), nullptr),
  btag_efficiencies_proc_(op_pts_.size(), nullptr),
  calib_deep_full_(new BTagCalibration("csvv2_deep", "data/DeepCSVMoriond17_comb.csv")),
  calib_deep_fast_(new BTagCalibration("csvv2_deep", "data/CSV_13TEV_Combined_14_7_2016.csv")),
  readers_deep_full_(),
  readers_deep_fast_(),
  btag_efficiencies_deep_(op_pts_.size(), nullptr),
  btag_efficiencies_deep_proc_(op_pts_.size(), nullptr),
  csv_loose_(is_cmssw_7 ? 0.605 : 0.5426),
  csv_medium_(is_cmssw_7 ? 0.890 : 0.8484),
  csv_tight_(is_cmssw_7 ? 0.970 : 0.9535),
  deep_csv_loose_(is_cmssw_7 ? 0. : 0.2219),
  deep_csv_medium_(is_cmssw_7 ? 0. : 0.6324),
  deep_csv_tight_(is_cmssw_7 ? 0. : 0.8958),
  is_fast_sim_(is_fast_sim){
  for(const auto &op: op_pts_){
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
  }
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

  const map<BTagEntry::OperatingPoint, unique_ptr<BTagCalibrationReader> > *ireaders_full = &readers_full_;
  if (do_deep_csv) ireaders_full = &readers_deep_full_;
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
  return (sf1*sf1_fs*eff1_fs-sf2*sf2_fs*eff2_fs)/(eff1_fs-eff2_fs);  
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
      bin = btag_efficiencies_.at(rdr_idx)->FindFixBin(fabs(eta), pT, pdgId);
      eff = btag_efficiencies_.at(rdr_idx)->GetBinContent(bin);
    }
    else{
      bin = btag_efficiencies_proc_.at(rdr_idx)->FindFixBin(fabs(eta), pT, pdgId);
      eff = btag_efficiencies_proc_.at(rdr_idx)->GetBinContent(bin);
    }
  }
  else{
    if(!do_by_proc){
      bin = btag_efficiencies_deep_.at(rdr_idx)->FindFixBin(fabs(eta), pT, pdgId);
      eff = btag_efficiencies_deep_.at(rdr_idx)->GetBinContent(bin);
    }
    else{
      bin = btag_efficiencies_deep_proc_.at(rdr_idx)->FindFixBin(fabs(eta), pT, pdgId);
      eff = btag_efficiencies_deep_proc_.at(rdr_idx)->GetBinContent(bin);
    }
  }
  
  return eff;
}
