#ifndef H_LEPTON_WEIGHTER
#define H_LEPTON_WEIGHTER

#include <vector>
#include <utility>

#include "TH2D.h"
#include "TH2F.h"

#include "baby_plus.hpp"

class LeptonWeighter{
public:
  LeptonWeighter();

  static void FullSim(baby_plus &b, float &w_lep, std::vector<float> &sys_lep);
  static void FastSim(baby_plus &b, float &w_fs_lep, std::vector<float> &sys_fs_lep);
  
private:
  static const TH2F sf_full_muon_medium_;
  static const TH2F sf_full_muon_iso_;
  static const TH2F sf_full_muon_vtx_;
  static const TH2D sf_full_muon_tracking_;

  static const TH2F sf_full_electron_medium_;
  static const TH2F sf_full_electron_iso_;
  static const TH2F sf_full_electron_tracking_;

  static const TH2D sf_fast_muon_medium_;
  static const TH2D sf_fast_muon_iso_;
  
  static const TH2D sf_fast_electron_mediumiso_;

  static std::pair<double, double> GetMuonScaleFactor(baby_plus &b, std::size_t ilep);
  static std::pair<double, double> GetElectronScaleFactor(baby_plus &b, std::size_t ilep);

  static std::pair<double, double> GetMuonScaleFactorFS(baby_plus &b, std::size_t ilep);
  static std::pair<double, double> GetElectronScaleFactorFS(baby_plus &b, std::size_t ilep);
};

#endif
