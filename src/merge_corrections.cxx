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
  out.out_w_bhig_deep() = 0.;
  out.out_w_btag_deep() = 0.;
  out.out_w_btag_loose_deep() = 0.;
  out.out_w_btag_tight_deep() = 0.;
  out.out_w_fs_lep() = 0.;
  out.out_w_isr() = 0.;
  out.out_w_lep() = 0.;
  out.out_w_lumi() = 0.;
  out.out_w_pu() = 0.;
  out.out_weight() = 0.;

  out.out_neff() = 0.;

  CopySize(in.sys_bchig_deep(),         out.out_sys_bchig_deep());
  CopySize(in.sys_bctag_deep(),         out.out_sys_bctag_deep());
  CopySize(in.sys_bctag_loose_deep(),   out.out_sys_bctag_loose_deep());
  CopySize(in.sys_bctag_tight_deep(),   out.out_sys_bctag_tight_deep());
  CopySize(in.sys_fs_bchig_deep(),      out.out_sys_fs_bchig_deep());
  CopySize(in.sys_fs_bctag_deep(),      out.out_sys_fs_bctag_deep());
  CopySize(in.sys_fs_lep(),             out.out_sys_fs_lep());
  CopySize(in.sys_fs_udsghig_deep(),    out.out_sys_fs_udsghig_deep());
  CopySize(in.sys_fs_udsgtag_deep(),    out.out_sys_fs_udsgtag_deep());
  CopySize(in.sys_isr(),                out.out_sys_isr());
  CopySize(in.sys_lep(),                out.out_sys_lep());
  CopySize(in.sys_muf(),                out.out_sys_muf());
  CopySize(in.sys_mur(),                out.out_sys_mur());
  CopySize(in.sys_murf(),               out.out_sys_murf());
  CopySize(in.sys_pdf(),                out.out_sys_pdf());
  CopySize(in.sys_pu(),                 out.out_sys_pu());
  CopySize(in.sys_udsghig_deep(),       out.out_sys_udsghig_deep());
  CopySize(in.sys_udsgtag_deep(),       out.out_sys_udsgtag_deep());
  CopySize(in.sys_udsgtag_loose_deep(), out.out_sys_udsgtag_loose_deep());
  CopySize(in.sys_udsgtag_tight_deep(), out.out_sys_udsgtag_tight_deep());
  CopySize(in.w_pdf(),                  out.out_w_pdf());
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

  out.out_w_bhig_deep()       += in.w_bhig_deep();
  out.out_w_btag_deep()       += in.w_btag_deep();
  out.out_w_btag_loose_deep() += in.w_btag_loose_deep();
  out.out_w_btag_tight_deep() += in.w_btag_tight_deep();
  out.out_w_fs_lep()          += in.w_fs_lep();
  out.out_w_isr()             += in.w_isr();
  out.out_w_lep()             += in.w_lep();
  out.out_w_pu()              += in.w_pu();
  out.out_weight()            += in.weight();

  VecAdd(in.sys_bchig_deep(),         out.out_sys_bchig_deep());
  VecAdd(in.sys_bctag_deep(),         out.out_sys_bctag_deep());
  VecAdd(in.sys_bctag_loose_deep(),   out.out_sys_bctag_loose_deep());
  VecAdd(in.sys_bctag_tight_deep(),   out.out_sys_bctag_tight_deep());
  VecAdd(in.sys_fs_bchig_deep(),      out.out_sys_fs_bchig_deep());
  VecAdd(in.sys_fs_bctag_deep(),      out.out_sys_fs_bctag_deep());
  VecAdd(in.sys_fs_lep(),             out.out_sys_fs_lep());
  VecAdd(in.sys_fs_udsghig_deep(),    out.out_sys_fs_udsghig_deep());
  VecAdd(in.sys_fs_udsgtag_deep(),    out.out_sys_fs_udsgtag_deep());
  VecAdd(in.sys_isr(),                out.out_sys_isr());
  VecAdd(in.sys_lep(),                out.out_sys_lep());
  VecAdd(in.sys_muf(),                out.out_sys_muf());
  VecAdd(in.sys_mur(),                out.out_sys_mur());
  VecAdd(in.sys_murf(),               out.out_sys_murf());
  VecAdd(in.sys_pdf(),                out.out_sys_pdf());
  VecAdd(in.sys_pu(),                 out.out_sys_pu());
  VecAdd(in.sys_udsghig_deep(),       out.out_sys_udsghig_deep());
  VecAdd(in.sys_udsgtag_deep(),       out.out_sys_udsgtag_deep());
  VecAdd(in.sys_udsgtag_loose_deep(), out.out_sys_udsgtag_loose_deep());
  VecAdd(in.sys_udsgtag_tight_deep(), out.out_sys_udsgtag_tight_deep());
  VecAdd(in.w_pdf(),                  out.out_w_pdf());
}

int GetGluinoMass(const string &path){
  string key = "_mGluino-";
  // if (Contains(path, "T2tt")) key = "_mStop-"; 
  auto pos1 = path.rfind(key)+key.size();
  auto pos2 = path.find("_", pos1);
  string mass_string = path.substr(pos1, pos2-pos1);
  return stoi(mass_string);
}

void FixLumi(baby_corr &out, const string &out_path, int year){
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
    xsec = xsec::crossSection(out_path, (year==2016));  
  }

  out.out_w_lumi() = xsec*lumi/out.out_neff();
}

void FixISR(baby_corr &out, const string &out_path, int year){
  double corr_w_isr(1.);
  vector<double> corr_sys_isr(2,1.);
  double tot_w_isr = out.out_w_isr();
  if (Contains(out_path,"TTJets_HT") || Contains(out_path,"genMET-150")){
  // in this case take correction from inclusive since should not norm. to unity
  //values consistent within 0.001 between 2016 and 2017 amazingly...
    if (Contains(out_path,"TTJets_DiLept")) {
      corr_w_isr = 1/0.997;
      corr_sys_isr[0] = 1/1.057;
      corr_sys_isr[1] = 1/0.938;
    } else {
      corr_w_isr = 1/1.017;
      corr_sys_isr[0] = 1/1.067;
      corr_sys_isr[1] = 1/0.967;        
    }
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
  if(year==2016) 
    out.out_weight() = (tot_w_isr*corr_w_isr)/(out.out_tot_weight_l0()*w_corr_l0 + out.out_tot_weight_l1());
  else
    out.out_weight() = nent/(out.out_tot_weight_l0()*w_corr_l0 + out.out_tot_weight_l1());
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

  Normalize(out.out_w_btag_deep(), nent);

  Normalize(out.out_w_bhig_deep(), nent);

  Normalize(out.out_sys_bctag_deep(), nent);
  Normalize(out.out_sys_udsgtag_deep(), nent);

  Normalize(out.out_sys_bchig_deep(), nent);
  Normalize(out.out_sys_udsghig_deep(), nent);

  Normalize(out.out_sys_mur(), nent);
  Normalize(out.out_sys_muf(), nent);
  Normalize(out.out_sys_murf(), nent);

  Normalize(out.out_sys_fs_bctag_deep(), nent);
  Normalize(out.out_sys_fs_udsgtag_deep(), nent);
  Normalize(out.out_sys_fs_bchig_deep(), nent);
  Normalize(out.out_sys_fs_udsghig_deep(), nent);

  Normalize(out.out_w_btag_loose_deep(), nent);
  Normalize(out.out_w_btag_tight_deep(), nent);

  Normalize(out.out_w_pdf(), nent);

  Normalize(out.out_sys_pu(), nent);
  Normalize(out.out_sys_pdf(), nent);

  Normalize(out.out_sys_bctag_loose_deep(), nent);
  Normalize(out.out_sys_udsgtag_loose_deep(), nent);
  Normalize(out.out_sys_bctag_tight_deep(), nent);
  Normalize(out.out_sys_udsgtag_tight_deep(), nent);
}

int main(int argc, char *argv[]){
  if(argc < 3){
    cout << "Too few arguments! Usage: " << argv[0]
         << " output_file input_file [more_input_files...]" << endl;
    return 1;
  }

  int year = atoi(argv[1]);
  string output_path = argv[2];
  vector<string> input_paths(argv+3, argv+argc);

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

  FixLumi(out, output_path, year);
  FixISR(out, output_path, year);
  Fix0L(out);

  Normalize(out);

  out.Fill();
  out.Write();
  cout << "Wrote output to " << output_path << endl;
}
