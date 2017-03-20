#ifndef H_BTAG_WEIGHTER
#define H_BTAG_WEIGHTER

#include <string>

#include "TH3D.h"

#include "baby_plus.hpp"
#include "BTagEntry.hpp"
#include "BTagCalibration.hpp"
#include "BTagCalibrationReader.hpp"

class BTagWeighter{
public:
  enum class Runs{all, BtoF, GtoH, B, CtoD, EtoF };

  explicit BTagWeighter(std::string proc,
                        bool is_fast_sim = false,
			bool is_cmssw_7 = false);

  double EventWeight(baby_plus &b, BTagEntry::OperatingPoint op,
		     const std::string &bc_full_syst, const std::string &udsg_full_syst,
		     const std::string &bc_fast_syst, const std::string &udsg_fast_syst,
		     bool do_deep_csv, bool do_by_proc, Runs runs = Runs::all) const;

  double EventWeight(baby_plus &b, BTagEntry::OperatingPoint op,
		     const std::string &bc_full_syst, const std::string &udsg_full_syst,
		     bool do_deep_csv, bool do_by_proc, Runs runs = Runs::all) const;

  double EventWeight(baby_plus &b, const std::vector<BTagEntry::OperatingPoint> &ops,
		     const std::string &bc_full_syst, const std::string &udsg_full_syst,
		     bool do_deep_csv, bool do_by_proc, Runs runs = Runs::all) const;

  double EventWeight(baby_plus &b, const std::vector<BTagEntry::OperatingPoint> &ops,
		     const std::string &bc_full_syst, const std::string &udsg_full_syst,
		     const std::string &bc_fast_syst, const std::string &udsg_fast_syst,
		     bool do_deep_csv, bool do_by_proc, Runs runs = Runs::all) const;

  double JetBTagWeight(baby_plus &b, std::size_t ijet, BTagEntry::OperatingPoint op,
		       const std::string &bc_full_syst, const std::string &udsg_full_syst,
		       const std::string &bc_fast_syst, const std::string &udsg_fast_syst,
		       bool do_deep_csv, bool do_by_proc, Runs runs = Runs::all) const;

  double JetBTagWeight(baby_plus &b, std::size_t ijet, BTagEntry::OperatingPoint op,
		       const std::string &bc_full_syst, const std::string &udsg_full_syst,
		       bool do_deep_csv, bool do_by_proc, Runs runs = Runs::all) const;

  double JetBTagWeight(baby_plus &b, std::size_t ijet, const std::vector<BTagEntry::OperatingPoint> &ops,
		       const std::string &bc_full_syst, const std::string &udsg_full_syst,
		       bool do_deep_csv, bool do_by_proc, Runs runs = Runs::all) const;

  double JetBTagWeight(baby_plus &b, std::size_t ijet, const std::vector<BTagEntry::OperatingPoint> &ops,
		       const std::string &bc_full_syst, const std::string &udsg_full_syst,
		       const std::string &bc_fast_syst, const std::string &udsg_fast_syst,
		       bool do_deep_csv, bool do_by_proc, Runs runs = Runs::all) const;

private:
  double GetMCTagEfficiency(int pdgId, float pT, float eta,
			    BTagEntry::OperatingPoint op, bool do_deep_csv, bool do_by_proc) const;

  static const std::vector<BTagEntry::OperatingPoint> op_pts_;
  static const std::vector<BTagEntry::JetFlavor> flavors_;

  std::unique_ptr<BTagCalibration> calib_full_;
  std::unique_ptr<BTagCalibration> calib_full_bf_;
  std::unique_ptr<BTagCalibration> calib_full_gh_;
  std::unique_ptr<BTagCalibration> calib_full_b_;
  std::unique_ptr<BTagCalibration> calib_full_cd_;
  std::unique_ptr<BTagCalibration> calib_full_ef_;
  std::unique_ptr<BTagCalibration> calib_fast_;
  std::map<BTagEntry::OperatingPoint, std::unique_ptr<BTagCalibrationReader> > readers_full_;
  std::map<BTagEntry::OperatingPoint, std::unique_ptr<BTagCalibrationReader> > readers_full_bf_;
  std::map<BTagEntry::OperatingPoint, std::unique_ptr<BTagCalibrationReader> > readers_full_gh_;
  std::map<BTagEntry::OperatingPoint, std::unique_ptr<BTagCalibrationReader> > readers_full_b_;
  std::map<BTagEntry::OperatingPoint, std::unique_ptr<BTagCalibrationReader> > readers_full_cd_;
  std::map<BTagEntry::OperatingPoint, std::unique_ptr<BTagCalibrationReader> > readers_full_ef_;
  std::map<BTagEntry::OperatingPoint, std::unique_ptr<BTagCalibrationReader> > readers_fast_;
  std::vector<TH3D> btag_efficiencies_;
  std::vector<TH3D> btag_efficiencies_proc_;

  std::unique_ptr<BTagCalibration> calib_deep_full_;
  std::unique_ptr<BTagCalibration> calib_deep_full_bf_;
  std::unique_ptr<BTagCalibration> calib_deep_full_gh_;
  std::unique_ptr<BTagCalibration> calib_deep_fast_;
  std::map<BTagEntry::OperatingPoint, std::unique_ptr<BTagCalibrationReader> > readers_deep_full_;
  std::map<BTagEntry::OperatingPoint, std::unique_ptr<BTagCalibrationReader> > readers_deep_full_bf_;
  std::map<BTagEntry::OperatingPoint, std::unique_ptr<BTagCalibrationReader> > readers_deep_full_gh_;
  std::map<BTagEntry::OperatingPoint, std::unique_ptr<BTagCalibrationReader> > readers_deep_fast_;
  std::vector<TH3D> btag_efficiencies_deep_;
  std::vector<TH3D> btag_efficiencies_deep_proc_;

  double csv_loose_, csv_medium_, csv_tight_;
  double deep_csv_loose_, deep_csv_medium_, deep_csv_tight_;

  bool is_fast_sim_;
};

#endif
