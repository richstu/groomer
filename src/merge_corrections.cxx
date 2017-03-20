#include <vector>
#include <string>
#include <iostream>

#include "baby_corr.hpp"
#include "cross_sections.hpp"
#include "utilities.hpp"

using namespace std;

template<typename T, typename U>
void CopySize(const vector<T> &in, vector<U> &out){
  out = vector<U>(in.size(), static_cast<U>(0.));
}

void Initialize(baby_corr &in, baby_corr &out){
  out.out_w_bhig() = 0.;
  out.out_w_bhig_deep() = 0.;
  out.out_w_bhig_deep_bf() = 0.;
  out.out_w_bhig_deep_gh() = 0.;
  out.out_w_bhig_deep_proc() = 0.;
  out.out_w_bhig_proc() = 0.;
  out.out_w_btag() = 0.;
  out.out_w_btag_bf() = 0.;
  out.out_w_btag_gh() = 0.;
  out.out_w_btag_b() = 0.;
  out.out_w_btag_cd() = 0.;
  out.out_w_btag_ef() = 0.;
  out.out_w_btag_deep() = 0.;
  out.out_w_btag_deep_proc() = 0.;
  out.out_w_btag_loose() = 0.;
  out.out_w_btag_bf_loose() = 0.;
  out.out_w_btag_gh_loose() = 0.;
  out.out_w_btag_b_loose() = 0.;
  out.out_w_btag_cd_loose() = 0.;
  out.out_w_btag_ef_loose() = 0.;
  out.out_w_btag_loose_deep() = 0.;
  out.out_w_btag_loose_deep_proc() = 0.;
  out.out_w_btag_loose_proc() = 0.;
  out.out_w_btag_proc() = 0.;
  out.out_w_btag_tight() = 0.;
  out.out_w_btag_bf_tight() = 0.;
  out.out_w_btag_gh_tight() = 0.;
  out.out_w_btag_b_tight() = 0.;
  out.out_w_btag_cd_tight() = 0.;
  out.out_w_btag_ef_tight() = 0.;
  out.out_w_btag_tight_deep() = 0.;
  out.out_w_btag_tight_deep_proc() = 0.;
  out.out_w_btag_tight_proc() = 0.;
  out.out_w_fs_lep() = 0.;
  out.out_w_isr() = 0.;
  out.out_w_lep() = 0.;
  out.out_w_lumi() = 0.;
  out.out_w_pu() = 0.;
  out.out_weight() = 0.;

  out.out_neff() = 0.;

  CopySize(in.sys_bchig(), out.out_sys_bchig());
  CopySize(in.sys_bchig_deep(), out.out_sys_bchig_deep());
  CopySize(in.sys_bchig_deep_bf(), out.out_sys_bchig_deep_bf());
  CopySize(in.sys_bchig_deep_gh(), out.out_sys_bchig_deep_gh());
  CopySize(in.sys_bchig_deep_proc(), out.out_sys_bchig_deep_proc());
  CopySize(in.sys_bchig_proc(), out.out_sys_bchig_proc());
  CopySize(in.sys_bctag(), out.out_sys_bctag());
  CopySize(in.sys_bctag_bf(), out.out_sys_bctag_bf());
  CopySize(in.sys_bctag_gh(), out.out_sys_bctag_gh());
  CopySize(in.sys_bctag_b(), out.out_sys_bctag_b());
  CopySize(in.sys_bctag_cd(), out.out_sys_bctag_cd());
  CopySize(in.sys_bctag_ef(), out.out_sys_bctag_ef());
  CopySize(in.sys_bctag_deep(), out.out_sys_bctag_deep());
  CopySize(in.sys_bctag_deep_proc(), out.out_sys_bctag_deep_proc());
  CopySize(in.sys_bctag_loose(), out.out_sys_bctag_loose());
  CopySize(in.sys_bctag_bf_loose(), out.out_sys_bctag_bf_loose());
  CopySize(in.sys_bctag_gh_loose(), out.out_sys_bctag_gh_loose());
  CopySize(in.sys_bctag_b_loose(), out.out_sys_bctag_b_loose());
  CopySize(in.sys_bctag_cd_loose(), out.out_sys_bctag_cd_loose());
  CopySize(in.sys_bctag_ef_loose(), out.out_sys_bctag_ef_loose());
  CopySize(in.sys_bctag_loose_deep(), out.out_sys_bctag_loose_deep());
  CopySize(in.sys_bctag_loose_deep_proc(), out.out_sys_bctag_loose_deep_proc());
  CopySize(in.sys_bctag_loose_proc(), out.out_sys_bctag_loose_proc());
  CopySize(in.sys_bctag_proc(), out.out_sys_bctag_proc());
  CopySize(in.sys_bctag_tight(), out.out_sys_bctag_tight());
  CopySize(in.sys_bctag_bf_tight(), out.out_sys_bctag_bf_tight());
  CopySize(in.sys_bctag_gh_tight(), out.out_sys_bctag_gh_tight());
  CopySize(in.sys_bctag_b_tight(), out.out_sys_bctag_b_tight());
  CopySize(in.sys_bctag_cd_tight(), out.out_sys_bctag_cd_tight());
  CopySize(in.sys_bctag_ef_tight(), out.out_sys_bctag_ef_tight());
  CopySize(in.sys_bctag_tight_deep(), out.out_sys_bctag_tight_deep());
  CopySize(in.sys_bctag_tight_deep_proc(), out.out_sys_bctag_tight_deep_proc());
  CopySize(in.sys_bctag_tight_proc(), out.out_sys_bctag_tight_proc());
  CopySize(in.sys_fs_bchig(), out.out_sys_fs_bchig());
  CopySize(in.sys_fs_bchig_deep(), out.out_sys_fs_bchig_deep());
  CopySize(in.sys_fs_bctag(), out.out_sys_fs_bctag());
  CopySize(in.sys_fs_bctag_deep(), out.out_sys_fs_bctag_deep());
  CopySize(in.sys_fs_lep(), out.out_sys_fs_lep());
  CopySize(in.sys_fs_udsghig(), out.out_sys_fs_udsghig());
  CopySize(in.sys_fs_udsghig_deep(), out.out_sys_fs_udsghig_deep());
  CopySize(in.sys_fs_udsgtag(), out.out_sys_fs_udsgtag());
  CopySize(in.sys_fs_udsgtag_deep(), out.out_sys_fs_udsgtag_deep());
  CopySize(in.sys_isr(), out.out_sys_isr());
  CopySize(in.sys_lep(), out.out_sys_lep());
  CopySize(in.sys_muf(), out.out_sys_muf());
  CopySize(in.sys_mur(), out.out_sys_mur());
  CopySize(in.sys_murf(), out.out_sys_murf());
  CopySize(in.sys_pdf(), out.out_sys_pdf());
  CopySize(in.sys_pu(), out.out_sys_pu());
  CopySize(in.sys_udsghig(), out.out_sys_udsghig());
  CopySize(in.sys_udsghig_deep(), out.out_sys_udsghig_deep());
  CopySize(in.sys_udsghig_deep_bf(), out.out_sys_udsghig_deep_bf());
  CopySize(in.sys_udsghig_deep_gh(), out.out_sys_udsghig_deep_gh());
  CopySize(in.sys_udsghig_deep_proc(), out.out_sys_udsghig_deep_proc());
  CopySize(in.sys_udsghig_proc(), out.out_sys_udsghig_proc());
  CopySize(in.sys_udsgtag(), out.out_sys_udsgtag());
  CopySize(in.sys_udsgtag_bf(), out.out_sys_udsgtag_bf());
  CopySize(in.sys_udsgtag_gh(), out.out_sys_udsgtag_gh());
  CopySize(in.sys_udsgtag_b(), out.out_sys_udsgtag_b());
  CopySize(in.sys_udsgtag_cd(), out.out_sys_udsgtag_cd());
  CopySize(in.sys_udsgtag_ef(), out.out_sys_udsgtag_ef());
  CopySize(in.sys_udsgtag_deep(), out.out_sys_udsgtag_deep());
  CopySize(in.sys_udsgtag_deep_proc(), out.out_sys_udsgtag_deep_proc());
  CopySize(in.sys_udsgtag_loose(), out.out_sys_udsgtag_loose());
  CopySize(in.sys_udsgtag_bf_loose(), out.out_sys_udsgtag_bf_loose());
  CopySize(in.sys_udsgtag_gh_loose(), out.out_sys_udsgtag_gh_loose());
  CopySize(in.sys_udsgtag_b_loose(), out.out_sys_udsgtag_b_loose());
  CopySize(in.sys_udsgtag_cd_loose(), out.out_sys_udsgtag_cd_loose());
  CopySize(in.sys_udsgtag_ef_loose(), out.out_sys_udsgtag_ef_loose());
  CopySize(in.sys_udsgtag_loose_deep(), out.out_sys_udsgtag_loose_deep());
  CopySize(in.sys_udsgtag_loose_deep_proc(), out.out_sys_udsgtag_loose_deep_proc());
  CopySize(in.sys_udsgtag_loose_proc(), out.out_sys_udsgtag_loose_proc());
  CopySize(in.sys_udsgtag_proc(), out.out_sys_udsgtag_proc());
  CopySize(in.sys_udsgtag_tight(), out.out_sys_udsgtag_tight());
  CopySize(in.sys_udsgtag_bf_tight(), out.out_sys_udsgtag_bf_tight());
  CopySize(in.sys_udsgtag_gh_tight(), out.out_sys_udsgtag_gh_tight());
  CopySize(in.sys_udsgtag_b_tight(), out.out_sys_udsgtag_b_tight());
  CopySize(in.sys_udsgtag_cd_tight(), out.out_sys_udsgtag_cd_tight());
  CopySize(in.sys_udsgtag_ef_tight(), out.out_sys_udsgtag_ef_tight());
  CopySize(in.sys_udsgtag_tight_deep(), out.out_sys_udsgtag_tight_deep());
  CopySize(in.sys_udsgtag_tight_deep_proc(), out.out_sys_udsgtag_tight_deep_proc());
  CopySize(in.sys_udsgtag_tight_proc(), out.out_sys_udsgtag_tight_proc());
  CopySize(in.w_pdf(), out.out_w_pdf());
}

template<typename T, typename U>
void VecAdd(const vector<T> &in, vector<U> &out){
  for(size_t i = 0; i < in.size(); ++i){
    out.at(i) += in.at(i);
  }
}

void AddEntry(baby_corr &in, baby_corr &out){
  out.out_neff() += in.neff();
  out.out_nent() += in.nent();
  out.out_nent_zlep() += in.nent_zlep();
  out.out_tot_weight_l0() += in.tot_weight_l0();
  out.out_tot_weight_l1() += in.tot_weight_l1();

  out.out_w_bhig() += in.w_bhig();
  out.out_w_bhig_deep() += in.w_bhig_deep();
  out.out_w_bhig_deep_bf() += in.w_bhig_deep_bf();
  out.out_w_bhig_deep_gh() += in.w_bhig_deep_gh();
  out.out_w_bhig_deep_proc() += in.w_bhig_deep_proc();
  out.out_w_bhig_proc() += in.w_bhig_proc();
  out.out_w_btag() += in.w_btag();
  out.out_w_btag_bf() += in.w_btag_bf();
  out.out_w_btag_gh() += in.w_btag_gh();
  out.out_w_btag_b() += in.w_btag_b();
  out.out_w_btag_cd() += in.w_btag_cd();
  out.out_w_btag_ef() += in.w_btag_ef();
  out.out_w_btag_deep() += in.w_btag_deep();
  out.out_w_btag_deep_proc() += in.w_btag_deep_proc();
  out.out_w_btag_loose() += in.w_btag_loose();
  out.out_w_btag_bf_loose() += in.w_btag_bf_loose();
  out.out_w_btag_gh_loose() += in.w_btag_gh_loose();
  out.out_w_btag_b_loose() += in.w_btag_b_loose();
  out.out_w_btag_cd_loose() += in.w_btag_cd_loose();
  out.out_w_btag_ef_loose() += in.w_btag_ef_loose();
  out.out_w_btag_loose_deep() += in.w_btag_loose_deep();
  out.out_w_btag_loose_deep_proc() += in.w_btag_loose_deep_proc();
  out.out_w_btag_loose_proc() += in.w_btag_loose_proc();
  out.out_w_btag_proc() += in.w_btag_proc();
  out.out_w_btag_tight() += in.w_btag_tight();
  out.out_w_btag_bf_tight() += in.w_btag_bf_tight();
  out.out_w_btag_gh_tight() += in.w_btag_gh_tight();
  out.out_w_btag_b_tight() += in.w_btag_b_tight();
  out.out_w_btag_cd_tight() += in.w_btag_cd_tight();
  out.out_w_btag_ef_tight() += in.w_btag_ef_tight();
  out.out_w_btag_tight_deep() += in.w_btag_tight_deep();
  out.out_w_btag_tight_deep_proc() += in.w_btag_tight_deep_proc();
  out.out_w_btag_tight_proc() += in.w_btag_tight_proc();
  out.out_w_fs_lep() += in.w_fs_lep();
  out.out_w_isr() += in.w_isr();
  out.out_w_lep() += in.w_lep();
  out.out_w_pu() += in.w_pu();
  out.out_weight() += in.weight();

  VecAdd(in.sys_bchig(), out.out_sys_bchig());
  VecAdd(in.sys_bchig_deep(), out.out_sys_bchig_deep());
  VecAdd(in.sys_bchig_deep_bf(), out.out_sys_bchig_deep_bf());
  VecAdd(in.sys_bchig_deep_gh(), out.out_sys_bchig_deep_gh());
  VecAdd(in.sys_bchig_deep_proc(), out.out_sys_bchig_deep_proc());
  VecAdd(in.sys_bchig_proc(), out.out_sys_bchig_proc());
  VecAdd(in.sys_bctag(), out.out_sys_bctag());
  VecAdd(in.sys_bctag_bf(), out.out_sys_bctag_bf());
  VecAdd(in.sys_bctag_gh(), out.out_sys_bctag_gh());
  VecAdd(in.sys_bctag_b(), out.out_sys_bctag_b());
  VecAdd(in.sys_bctag_cd(), out.out_sys_bctag_cd());
  VecAdd(in.sys_bctag_ef(), out.out_sys_bctag_ef());
  VecAdd(in.sys_bctag_deep(), out.out_sys_bctag_deep());
  VecAdd(in.sys_bctag_deep_proc(), out.out_sys_bctag_deep_proc());
  VecAdd(in.sys_bctag_loose(), out.out_sys_bctag_loose());
  VecAdd(in.sys_bctag_bf_loose(), out.out_sys_bctag_bf_loose());
  VecAdd(in.sys_bctag_gh_loose(), out.out_sys_bctag_gh_loose());
  VecAdd(in.sys_bctag_b_loose(), out.out_sys_bctag_b_loose());
  VecAdd(in.sys_bctag_cd_loose(), out.out_sys_bctag_cd_loose());
  VecAdd(in.sys_bctag_ef_loose(), out.out_sys_bctag_ef_loose());
  VecAdd(in.sys_bctag_loose_deep(), out.out_sys_bctag_loose_deep());
  VecAdd(in.sys_bctag_loose_deep_proc(), out.out_sys_bctag_loose_deep_proc());
  VecAdd(in.sys_bctag_loose_proc(), out.out_sys_bctag_loose_proc());
  VecAdd(in.sys_bctag_proc(), out.out_sys_bctag_proc());
  VecAdd(in.sys_bctag_tight(), out.out_sys_bctag_tight());
  VecAdd(in.sys_bctag_bf_tight(), out.out_sys_bctag_bf_tight());
  VecAdd(in.sys_bctag_gh_tight(), out.out_sys_bctag_gh_tight());
  VecAdd(in.sys_bctag_b_tight(), out.out_sys_bctag_b_tight());
  VecAdd(in.sys_bctag_cd_tight(), out.out_sys_bctag_cd_tight());
  VecAdd(in.sys_bctag_ef_tight(), out.out_sys_bctag_ef_tight());
  VecAdd(in.sys_bctag_tight_deep(), out.out_sys_bctag_tight_deep());
  VecAdd(in.sys_bctag_tight_deep_proc(), out.out_sys_bctag_tight_deep_proc());
  VecAdd(in.sys_bctag_tight_proc(), out.out_sys_bctag_tight_proc());
  VecAdd(in.sys_fs_bchig(), out.out_sys_fs_bchig());
  VecAdd(in.sys_fs_bchig_deep(), out.out_sys_fs_bchig_deep());
  VecAdd(in.sys_fs_bctag(), out.out_sys_fs_bctag());
  VecAdd(in.sys_fs_bctag_deep(), out.out_sys_fs_bctag_deep());
  VecAdd(in.sys_fs_lep(), out.out_sys_fs_lep());
  VecAdd(in.sys_fs_udsghig(), out.out_sys_fs_udsghig());
  VecAdd(in.sys_fs_udsghig_deep(), out.out_sys_fs_udsghig_deep());
  VecAdd(in.sys_fs_udsgtag(), out.out_sys_fs_udsgtag());
  VecAdd(in.sys_fs_udsgtag_deep(), out.out_sys_fs_udsgtag_deep());
  VecAdd(in.sys_isr(), out.out_sys_isr());
  VecAdd(in.sys_lep(), out.out_sys_lep());
  VecAdd(in.sys_muf(), out.out_sys_muf());
  VecAdd(in.sys_mur(), out.out_sys_mur());
  VecAdd(in.sys_murf(), out.out_sys_murf());
  VecAdd(in.sys_pdf(), out.out_sys_pdf());
  VecAdd(in.sys_pu(), out.out_sys_pu());
  VecAdd(in.sys_udsghig(), out.out_sys_udsghig());
  VecAdd(in.sys_udsghig_deep(), out.out_sys_udsghig_deep());
  VecAdd(in.sys_udsghig_deep_bf(), out.out_sys_udsghig_deep_bf());
  VecAdd(in.sys_udsghig_deep_gh(), out.out_sys_udsghig_deep_gh());
  VecAdd(in.sys_udsghig_deep_proc(), out.out_sys_udsghig_deep_proc());
  VecAdd(in.sys_udsghig_proc(), out.out_sys_udsghig_proc());
  VecAdd(in.sys_udsgtag(), out.out_sys_udsgtag());
  VecAdd(in.sys_udsgtag_bf(), out.out_sys_udsgtag_bf());
  VecAdd(in.sys_udsgtag_gh(), out.out_sys_udsgtag_gh());
  VecAdd(in.sys_udsgtag_b(), out.out_sys_udsgtag_b());
  VecAdd(in.sys_udsgtag_cd(), out.out_sys_udsgtag_cd());
  VecAdd(in.sys_udsgtag_ef(), out.out_sys_udsgtag_ef());
  VecAdd(in.sys_udsgtag_deep(), out.out_sys_udsgtag_deep());
  VecAdd(in.sys_udsgtag_deep_proc(), out.out_sys_udsgtag_deep_proc());
  VecAdd(in.sys_udsgtag_loose(), out.out_sys_udsgtag_loose());
  VecAdd(in.sys_udsgtag_bf_loose(), out.out_sys_udsgtag_bf_loose());
  VecAdd(in.sys_udsgtag_gh_loose(), out.out_sys_udsgtag_gh_loose());
  VecAdd(in.sys_udsgtag_b_loose(), out.out_sys_udsgtag_b_loose());
  VecAdd(in.sys_udsgtag_cd_loose(), out.out_sys_udsgtag_cd_loose());
  VecAdd(in.sys_udsgtag_ef_loose(), out.out_sys_udsgtag_ef_loose());
  VecAdd(in.sys_udsgtag_loose_deep(), out.out_sys_udsgtag_loose_deep());
  VecAdd(in.sys_udsgtag_loose_deep_proc(), out.out_sys_udsgtag_loose_deep_proc());
  VecAdd(in.sys_udsgtag_loose_proc(), out.out_sys_udsgtag_loose_proc());
  VecAdd(in.sys_udsgtag_proc(), out.out_sys_udsgtag_proc());
  VecAdd(in.sys_udsgtag_tight(), out.out_sys_udsgtag_tight());
  VecAdd(in.sys_udsgtag_bf_tight(), out.out_sys_udsgtag_bf_tight());
  VecAdd(in.sys_udsgtag_gh_tight(), out.out_sys_udsgtag_gh_tight());
  VecAdd(in.sys_udsgtag_b_tight(), out.out_sys_udsgtag_b_tight());
  VecAdd(in.sys_udsgtag_cd_tight(), out.out_sys_udsgtag_cd_tight());
  VecAdd(in.sys_udsgtag_ef_tight(), out.out_sys_udsgtag_ef_tight());
  VecAdd(in.sys_udsgtag_tight_deep(), out.out_sys_udsgtag_tight_deep());
  VecAdd(in.sys_udsgtag_tight_deep_proc(), out.out_sys_udsgtag_tight_deep_proc());
  VecAdd(in.sys_udsgtag_tight_proc(), out.out_sys_udsgtag_tight_proc());
  VecAdd(in.w_pdf(), out.out_w_pdf());
}

int GetGluinoMass(const string &path){
  string key = "_mGluino-";
  auto pos1 = path.rfind(key)+key.size();
  auto pos2 = path.find("_", pos1);
  string mass_string = path.substr(pos1, pos2-pos1);
  return stoi(mass_string);
}

void FixLumi(baby_corr &out, const string &out_path){
  double xsec(0.); const float lumi = 1000.;
  if (Contains(out_path, "SMS")){
    double exsec(0.);
    int mglu = GetGluinoMass(out_path);
    if(Contains(out_path, "T1") || Contains(out_path, "T5")){
      xsec::signalCrossSection(mglu, xsec, exsec);
    }else if(Contains(out_path, "TChiHH")){
      xsec::higgsinoCrossSection(mglu, xsec, exsec);
    }else{
      xsec::stopCrossSection(mglu, xsec, exsec);
    }
  }else{
    xsec = xsec::crossSection(out_path);
  }

  out.out_w_lumi() = xsec*lumi/out.out_neff();
}

void FixISR(baby_corr &out, const string &out_path){
  double corr_w_isr(1.);
  vector<double> corr_sys_isr(2,1.);
  double tot_w_isr = out.out_w_isr();
  if(Contains(out_path,"TTJets_HT") || Contains(out_path,"genMET-150")){
    // in this case take correction from inclusive since should not norm. to unity
    corr_w_isr = 1/1.013;
    corr_sys_isr[0] = 1/1.065;
    corr_sys_isr[1] = 1/0.9612;
  }else{
    corr_w_isr = out.out_w_isr() ? out.out_nent()/out.out_w_isr() : 1.;
    for(size_t i = 0; i<out.out_sys_isr().size(); i++){
      corr_sys_isr[i] = out.out_sys_isr()[i] ? out.out_nent()/out.out_sys_isr()[i] : 1.;
    }
  }
  out.out_w_isr() = corr_w_isr;
  for(size_t i = 0; i<out.out_sys_isr().size(); i++){
    out.out_sys_isr()[i] = corr_sys_isr[i];
  }
  double nent = out.out_nent();
  double nent_zlep = out.out_nent_zlep();

  // Calculate correction to total weight whilst correcting zero lepton
  //----------------------------------------------------------------------
  double w_corr_l0 = 1.;
  if (out.out_w_lep()) w_corr_l0 *= (nent-out.out_w_lep())/nent_zlep;
  if (out.out_w_fs_lep()) w_corr_l0 *= (nent-out.out_w_fs_lep())/nent_zlep;
  if(nent_zlep==0) w_corr_l0 = 1.;
  // again normalize to total w_isr, not unity
  out.out_weight() = (tot_w_isr*corr_w_isr)/(out.out_tot_weight_l0()*w_corr_l0 + out.out_tot_weight_l1());
}

void Fix0L(baby_corr &out){
  double nent = out.out_nent();
  double nent_zlep = out.out_nent_zlep();

  // Lepton weights corrections to be applied only to 0-lep events
  //----------------------------------------------------------------
  out.out_w_lep()           = out.out_w_lep() ? (nent-out.out_w_lep())/nent_zlep : 1.;
  out.out_w_fs_lep()        = out.out_w_fs_lep() ? (nent-out.out_w_fs_lep())/nent_zlep : 1.;
  for(size_t i = 0; i<out.out_sys_lep().size(); i++){
    out.out_sys_lep()[i]    = out.out_sys_lep()[i] ? (nent-out.out_sys_lep()[i])/nent_zlep : 1.;
  }
  for(size_t i = 0; i<out.out_sys_fs_lep().size(); i++){
    out.out_sys_fs_lep()[i] = out.out_sys_fs_lep()[i] ? (nent-out.out_sys_fs_lep()[i])/nent_zlep : 1.;
  }
}

template<typename T>
void Normalize(T &x, double nent){
  x = x ? nent/x : 1.;
}

template<typename T>
void Normalize(vector<T> &v, double nent){
  for(auto &x: v) x = x ? nent/x : 1.;
}

void Normalize(baby_corr &out){
  double nent = out.out_nent();

  Normalize(out.out_w_pu(), nent);

  Normalize(out.out_w_btag(), nent);
  Normalize(out.out_w_btag_bf(), nent);
  Normalize(out.out_w_btag_gh(), nent);
  Normalize(out.out_w_btag_b(), nent);
  Normalize(out.out_w_btag_cd(), nent);
  Normalize(out.out_w_btag_ef(), nent);
  Normalize(out.out_w_btag_proc(), nent);
  Normalize(out.out_w_btag_deep(), nent);
  Normalize(out.out_w_btag_deep_proc(), nent);

  Normalize(out.out_w_bhig(), nent);
  Normalize(out.out_w_bhig_proc(), nent);
  Normalize(out.out_w_bhig_deep(), nent);
  Normalize(out.out_w_bhig_deep_bf(), nent);
  Normalize(out.out_w_bhig_deep_gh(), nent);
  Normalize(out.out_w_bhig_deep_proc(), nent);

  Normalize(out.out_sys_bctag(), nent);
  Normalize(out.out_sys_bctag_bf(), nent);
  Normalize(out.out_sys_bctag_gh(), nent);
  Normalize(out.out_sys_bctag_b(), nent);
  Normalize(out.out_sys_bctag_cd(), nent);
  Normalize(out.out_sys_bctag_ef(), nent);
  Normalize(out.out_sys_bctag_proc(), nent);
  Normalize(out.out_sys_bctag_deep(), nent);
  Normalize(out.out_sys_bctag_deep_proc(), nent);
  Normalize(out.out_sys_udsgtag(), nent);
  Normalize(out.out_sys_udsgtag_bf(), nent);
  Normalize(out.out_sys_udsgtag_gh(), nent);
  Normalize(out.out_sys_udsgtag_b(), nent);
  Normalize(out.out_sys_udsgtag_cd(), nent);
  Normalize(out.out_sys_udsgtag_ef(), nent);
  Normalize(out.out_sys_udsgtag_proc(), nent);
  Normalize(out.out_sys_udsgtag_deep(), nent);
  Normalize(out.out_sys_udsgtag_deep_proc(), nent);

  Normalize(out.out_sys_bchig(), nent);
  Normalize(out.out_sys_bchig_proc(), nent);
  Normalize(out.out_sys_bchig_deep(), nent);
  Normalize(out.out_sys_bchig_deep_bf(), nent);
  Normalize(out.out_sys_bchig_deep_gh(), nent);
  Normalize(out.out_sys_bchig_deep_proc(), nent);
  Normalize(out.out_sys_udsghig(), nent);
  Normalize(out.out_sys_udsghig_proc(), nent);
  Normalize(out.out_sys_udsghig_deep(), nent);
  Normalize(out.out_sys_udsghig_deep_bf(), nent);
  Normalize(out.out_sys_udsghig_deep_gh(), nent);
  Normalize(out.out_sys_udsghig_deep_proc(), nent);

  Normalize(out.out_sys_mur(), nent);
  Normalize(out.out_sys_muf(), nent);
  Normalize(out.out_sys_murf(), nent);

  Normalize(out.out_sys_fs_bctag(), nent);
  Normalize(out.out_sys_fs_bctag_deep(), nent);
  Normalize(out.out_sys_fs_udsgtag(), nent);
  Normalize(out.out_sys_fs_udsgtag_deep(), nent);
  Normalize(out.out_sys_fs_bchig(), nent);
  Normalize(out.out_sys_fs_bchig_deep(), nent);
  Normalize(out.out_sys_fs_udsghig(), nent);
  Normalize(out.out_sys_fs_udsghig_deep(), nent);

  Normalize(out.out_w_btag_loose(), nent);
  Normalize(out.out_w_btag_bf_loose(), nent);
  Normalize(out.out_w_btag_gh_loose(), nent);
  Normalize(out.out_w_btag_b_loose(), nent);
  Normalize(out.out_w_btag_cd_loose(), nent);
  Normalize(out.out_w_btag_ef_loose(), nent);
  Normalize(out.out_w_btag_loose_deep(), nent);
  Normalize(out.out_w_btag_tight(), nent);
  Normalize(out.out_w_btag_bf_tight(), nent);
  Normalize(out.out_w_btag_gh_tight(), nent);
  Normalize(out.out_w_btag_b_tight(), nent);
  Normalize(out.out_w_btag_cd_tight(), nent);
  Normalize(out.out_w_btag_ef_tight(), nent);
  Normalize(out.out_w_btag_tight_deep(), nent);
  Normalize(out.out_w_btag_loose_proc(), nent);
  Normalize(out.out_w_btag_loose_deep_proc(), nent);
  Normalize(out.out_w_btag_tight_proc(), nent);
  Normalize(out.out_w_btag_tight_deep_proc(), nent);

  Normalize(out.out_w_pdf(), nent);

  Normalize(out.out_sys_pu(), nent);
  Normalize(out.out_sys_pdf(), nent);

  Normalize(out.out_sys_bctag_loose(), nent);
  Normalize(out.out_sys_bctag_bf_loose(), nent);
  Normalize(out.out_sys_bctag_gh_loose(), nent);
  Normalize(out.out_sys_bctag_b_loose(), nent);
  Normalize(out.out_sys_bctag_cd_loose(), nent);
  Normalize(out.out_sys_bctag_ef_loose(), nent);
  Normalize(out.out_sys_bctag_loose_deep(), nent);
  Normalize(out.out_sys_udsgtag_loose(), nent);
  Normalize(out.out_sys_udsgtag_bf_loose(), nent);
  Normalize(out.out_sys_udsgtag_gh_loose(), nent);
  Normalize(out.out_sys_udsgtag_b_loose(), nent);
  Normalize(out.out_sys_udsgtag_cd_loose(), nent);
  Normalize(out.out_sys_udsgtag_ef_loose(), nent);
  Normalize(out.out_sys_udsgtag_loose_deep(), nent);
  Normalize(out.out_sys_bctag_tight(), nent);
  Normalize(out.out_sys_bctag_bf_tight(), nent);
  Normalize(out.out_sys_bctag_gh_tight(), nent);
  Normalize(out.out_sys_bctag_b_tight(), nent);
  Normalize(out.out_sys_bctag_cd_tight(), nent);
  Normalize(out.out_sys_bctag_ef_tight(), nent);
  Normalize(out.out_sys_bctag_tight_deep(), nent);
  Normalize(out.out_sys_udsgtag_tight(), nent);
  Normalize(out.out_sys_udsgtag_bf_tight(), nent);
  Normalize(out.out_sys_udsgtag_gh_tight(), nent);
  Normalize(out.out_sys_udsgtag_b_tight(), nent);
  Normalize(out.out_sys_udsgtag_cd_tight(), nent);
  Normalize(out.out_sys_udsgtag_ef_tight(), nent);
  Normalize(out.out_sys_udsgtag_tight_deep(), nent);
  Normalize(out.out_sys_bctag_loose_proc(), nent);
  Normalize(out.out_sys_bctag_loose_deep_proc(), nent);
  Normalize(out.out_sys_udsgtag_loose_proc(), nent);
  Normalize(out.out_sys_udsgtag_loose_deep_proc(), nent);
  Normalize(out.out_sys_bctag_tight_proc(), nent);
  Normalize(out.out_sys_bctag_tight_deep_proc(), nent);
  Normalize(out.out_sys_udsgtag_tight_proc(), nent);
  Normalize(out.out_sys_udsgtag_tight_deep_proc(), nent);
}

int main(int argc, char *argv[]){
  if(argc < 3){
    cout << "Too few arguments! Usage: " << argv[0]
         << " output_file input_file [more_input_files...]" << endl;
    return 1;
  }

  string output_path = argv[1];
  vector<string> input_paths(argv+2, argv+argc);

  baby_corr out("", output_path.c_str());
  baby_corr in(input_paths.front().c_str());
  for(size_t i = 1; i < input_paths.size(); ++i){
    in.intree_->Add(input_paths.at(i).c_str());
  }

  size_t num_entries = in.GetEntries();
  if(num_entries <= 0){
    cout << "No entries in input files!" << endl;
    return 1;
  }
  in.GetEntry(0);
  Initialize(in, out);

  for(size_t i = 0; i < num_entries; ++i){
    in.GetEntry(i);
    AddEntry(in, out);
  }

  FixLumi(out, output_path);
  FixISR(out, output_path);
  Fix0L(out);

  Normalize(out);

  out.Fill();
  out.Write();
  cout << "Wrote output to " << output_path << endl;
}
