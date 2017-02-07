#ifndef H_BTAG_WEIGHTER
#define H_BTAG_WEIGHTER

#include <string>

#include "baby_plus.hpp"
#include "BTagEntry.hpp"
#include "BTagCalibration.hpp"
#include "BTagCalibrationReader.hpp"

class TH3D;

class BTagWeighter{
public:
  explicit BTagWeighter(bool is_fast_sim = false,
			bool is_cmssw_7 = false);

  double JetBTagWeight(baby_plus &b, std::size_t ijet, BTagEntry::OperatingPoint op,
		       const std::string &bc_full_syst, const std::string &udsg_full_syst,
		       const std::string &bc_fast_syst, const std::string &udsg_fast_syst,
		       bool do_deep_csv, bool do_by_proc) const;

  double JetBTagWeight(baby_plus &b, std::size_t ijet, BTagEntry::OperatingPoint op,
		       const std::string &bc_full_syst, const std::string &udsg_full_syst,
		       bool do_deep_csv, bool do_by_proc) const;

  double JetBTagWeight(baby_plus &b, std::size_t ijet, const std::vector<BTagEntry::OperatingPoint> &ops,
		       const std::string &bc_full_syst, const std::string &udsg_full_syst,
		       bool do_deep_csv, bool do_by_proc) const;

  double JetBTagWeight(baby_plus &b, std::size_t ijet, const std::vector<BTagEntry::OperatingPoint> &ops,
		       const std::string &bc_full_syst, const std::string &udsg_full_syst,
		       const std::string &bc_fast_syst, const std::string &udsg_fast_syst,
		       bool do_deep_csv, bool do_by_proc) const;
private:
  double GetMCTagEfficiency(int pdgId, float pT, float eta,
			    BTagEntry::OperatingPoint op, bool do_deep_csv, bool do_by_proc) const;

  static const std::vector<BTagEntry::OperatingPoint> op_pts_;
  static const std::vector<BTagEntry::JetFlavor> flavors_;

  std::unique_ptr<BTagCalibration> calib_full_;
  std::unique_ptr<BTagCalibration> calib_fast_;
  std::map<BTagEntry::OperatingPoint, std::unique_ptr<BTagCalibrationReader> > readers_full_;
  std::map<BTagEntry::OperatingPoint, std::unique_ptr<BTagCalibrationReader> > readers_fast_;
  std::vector<const TH3D*> btag_efficiencies_;
  std::vector<const TH3D*> btag_efficiencies_proc_;

  std::unique_ptr<BTagCalibration> calib_deep_full_;
  std::unique_ptr<BTagCalibration> calib_deep_fast_;
  std::map<BTagEntry::OperatingPoint, std::unique_ptr<BTagCalibrationReader> > readers_deep_full_;
  std::map<BTagEntry::OperatingPoint, std::unique_ptr<BTagCalibrationReader> > readers_deep_fast_;
  std::vector<const TH3D*> btag_efficiencies_deep_;
  std::vector<const TH3D*> btag_efficiencies_deep_proc_;

  double csv_loose_, csv_medium_, csv_tight_;
  double deep_csv_loose_, deep_csv_medium_, deep_csv_tight_;

  bool is_fast_sim_;
};

#endif
