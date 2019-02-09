#ifndef H_LEPTON_WEIGHTER
#define H_LEPTON_WEIGHTER

#include <vector>
#include <utility>

#include "TH2D.h"
#include "TH2F.h"

#include "baby_plus.hpp"

class LeptonWeighter{
public:
  LeptonWeighter(int year);

  void FullSim(baby_plus &b, float &w_lep, std::vector<float> &sys_lep);
  void FastSim(baby_plus &b, float &w_fs_lep, std::vector<float> &sys_fs_lep);
  
private:
  std::string in_full_mu_med_, hist_full_mu_med_;
  std::string in_full_mu_iso_, hist_full_mu_iso_;
  std::string in_full_mu_vtx_, hist_full_mu_vtx_;
  std::string in_full_el_med_, hist_full_el_med_;
  std::string in_full_el_iso_, hist_full_el_iso_;
  std::string in_full_el_trk_, hist_full_el_trk_;
  std::string in_fast_mu_med_, hist_fast_mu_med_;
  std::string in_fast_mu_iso_, hist_fast_mu_iso_;
  std::string in_fast_el_mediso_, hist_fast_el_mediso_;

  TH2F sf_full_muon_medium_;
  TH2F sf_full_muon_iso_;
  TH2F sf_full_muon_vtx_;
  TH2D sf_full_muon_tracking_;

  TH2F sf_full_electron_medium_;
  TH2F sf_full_electron_iso_;
  TH2F sf_full_electron_tracking_;

  TH2D sf_fast_muon_medium_;
  TH2D sf_fast_muon_iso_;
  
  TH2D sf_fast_electron_mediumiso_;

  std::pair<double, double> GetMuonScaleFactor(baby_plus &b, std::size_t ilep);
  std::pair<double, double> GetElectronScaleFactor(baby_plus &b, std::size_t ilep);

  std::pair<double, double> GetMuonScaleFactorFS(baby_plus &b, std::size_t ilep);
  std::pair<double, double> GetElectronScaleFactorFS(baby_plus &b, std::size_t ilep);
};

#endif
