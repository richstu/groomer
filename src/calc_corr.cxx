#include <ctime>

#include <iostream>

#include <getopt.h>

#include "TError.h"

#include "baby_plus.hpp"
#include "baby_corr.hpp"
#include "utilities.hpp"
#include "cross_sections.hpp"
#include "btag_weighter.hpp"
#include "lepton_weighter.hpp"

using namespace std;

namespace {
  string in_file = "";
  string wgt_dir = "";
  string out_dir = "";
  bool fix_b_wgt = true;
  bool fix_lep_wgt = true;
  bool isFastSim = false;
  int year = 2016;
}

void GetOptions(int argc, char *argv[]);

int main(int argc, char *argv[]){
  // gErrorIgnoreLevel=6000; // Turns off ROOT errors due to missing branches       
  GetOptions(argc, argv);

  time_t begtime, endtime;
  time(&begtime);

  if(in_file=="" || wgt_dir == "" || out_dir == "") {
    cout<<"ERROR: Input file, sum-of-weights and/or output directory not specified. Exit."<<endl;
    exit(0);
  }

  string base_dir = "", file_name = "";
  SplitFilePath(in_file, base_dir, file_name);

  string corr_file = wgt_dir + "/" + file_name;
  string out_file = out_dir + "/" + file_name;
  
  cout << "Input file: " << in_file << endl;
  baby_plus b(in_file, out_file);

  cout << "Writing output to: " << out_file << endl;
  cout << "Writing sum-of-weights to: " << corr_file << endl;

  cout << "Running on "<< (isFastSim ? "FastSim" : "FullSim") << endl;
  cout << "Calculating weights based on " << year << " inputs." << endl;

  string proc = "tt";
  if(Contains(file_name, "WJets")) proc = "wjets";
  else if(Contains(file_name, "QCD")) proc = "qcd";
  //Need to improve to handle FullSim signal points
  BTagWeighter btw(proc, isFastSim, false);
  LeptonWeighter lw(year);

  baby_corr c("", corr_file);
  c.out_w_pdf().resize(100,0);
  c.out_sys_isr().resize(2,0);
  c.out_sys_lep().resize(2,0);
  c.out_sys_fs_lep().resize(2,0);
  c.out_sys_bctag_deep().resize(2,0);
  c.out_sys_udsgtag_deep().resize(2,0);
  c.out_sys_bchig_deep().resize(2,0);
  c.out_sys_udsghig_deep().resize(2,0);
  c.out_sys_mur().resize(2,0);
  c.out_sys_muf().resize(2,0);
  c.out_sys_murf().resize(2,0);
  c.out_sys_fs_bctag_deep().resize(2,0);
  c.out_sys_fs_udsgtag_deep().resize(2,0);
  c.out_sys_fs_bchig_deep().resize(2,0);
  c.out_sys_fs_udsghig_deep().resize(2,0);
  c.out_sys_pu().resize(2,0);
  c.out_sys_pdf().resize(2,0);
  c.out_sys_bctag_loose_deep().resize(2,0);
  c.out_sys_udsgtag_loose_deep().resize(2,0);
  c.out_sys_bctag_tight_deep().resize(2,0);
  c.out_sys_udsgtag_tight_deep().resize(2,0);

  // quantities to keep track of;
  int neff(0);
  double wgt(0);

  c.out_nent_zlep() = 0.;
  c.out_tot_weight_l0() = 0.;
  c.out_tot_weight_l1() = 0.;
  c.out_nent() = b.GetEntries();
  cout<<"Running over "<<c.out_nent()<<" events."<<endl;

  const string ctr = "central";
  const string vup = "up";
  const string vdown = "down";
  const auto op_loose = BTagEntry::OP_LOOSE;
  const auto op_med = BTagEntry::OP_MEDIUM;
  const auto op_tight = BTagEntry::OP_TIGHT;
  const vector<BTagEntry::OperatingPoint> op_all = {BTagEntry::OP_LOOSE, BTagEntry::OP_MEDIUM, BTagEntry::OP_TIGHT};

  for(long entry(0); entry<c.out_nent(); ++entry){
    b.GetEntry(entry);
    if (entry%100000==0 || entry == c.out_nent()-1) {
      cout<<"Processing event: "<<entry<<endl;
    }

    float w_lep(1.), w_fs_lep(1.);
    vector<float> sys_lep(2,1.), sys_fs_lep(2,1.);
    if(fix_lep_wgt){
      lw.FullSim(b, w_lep, sys_lep);
      if(isFastSim) lw.FastSim(b, w_fs_lep, sys_fs_lep);
      b.out_w_lep() = w_lep;
      b.out_sys_lep() = sys_lep;
      b.out_w_fs_lep() = w_fs_lep;
      b.out_sys_fs_lep() = sys_fs_lep;
    }else{
      w_lep = b.w_lep();
      sys_lep = b.sys_lep();
      if(isFastSim){
            w_fs_lep = b.w_lep();
            sys_fs_lep = b.sys_fs_lep();
      }
    }

    const bool doDeepCSV = true;
    const bool doProc = false;
    double w_btag_deep_ = fix_b_wgt ? btw.EventWeight(b, op_med, ctr, ctr, doDeepCSV, doProc) : b.w_btag_deep();
    if (year==2016) {
      wgt = w_lep*(isFastSim ? w_fs_lep : 1.)*w_btag_deep_*b.w_isr()*b.w_pu();
    } else {
      wgt = w_lep*(isFastSim ? w_fs_lep : 1.)*w_btag_deep_*b.w_pu();
      if (isFastSim)
        wgt *= b.w_isr();
    }

    // need special treatment in summing and/or renormalizing
    neff += b.w_lumi()>0 ? 1:-1;

    c.out_w_isr() += b.w_isr();
    for(size_t i = 0; i<b.sys_isr().size(); ++i){
      c.out_sys_isr().at(i) += b.sys_isr().at(i);
    }

    if(b.nleps()==0){
      c.out_nent_zlep()     += 1.;
      c.out_tot_weight_l0() += wgt;
    }else{
      c.out_tot_weight_l1() += wgt;
      c.out_w_lep()         += w_lep;
      for(size_t i = 0; i<b.sys_lep().size(); ++i){
        c.out_sys_lep().at(i) += sys_lep.at(i);
      }
      if(isFastSim){
        c.out_w_fs_lep()      += w_fs_lep;
        for(size_t i = 0; i<b.sys_fs_lep().size(); ++i){
          c.out_sys_fs_lep().at(i) += sys_fs_lep.at(i);
        }
      }
    }
    
    //      Cookie-cutter variables
    //-----------------------------------
    c.out_w_pu() += b.w_pu();

    double tmp = 0.;
    
    // calculated above -> fix_b_wgt ? btw.EventWeight(b, op_med, ctr, ctr, doDeepCSV, doProc) : b.w_btag_deep();
    c.out_w_btag_deep()+= w_btag_deep_; b.out_w_btag_deep() = w_btag_deep_;

    tmp = fix_b_wgt ? btw.EventWeight(b, op_loose, ctr, ctr, doDeepCSV, doProc) : b.w_btag_loose_deep();
    c.out_w_btag_loose_deep()+= tmp; b.out_w_btag_loose_deep() = tmp;
    tmp = fix_b_wgt ? btw.EventWeight(b, op_tight, ctr, ctr, doDeepCSV, doProc) : b.w_btag_tight_deep();
    c.out_w_btag_tight_deep()+= tmp; b.out_w_btag_tight_deep() = tmp;
    tmp = fix_b_wgt ? btw.EventWeight(b, op_all, ctr, ctr, doDeepCSV, doProc)   : b.w_bhig_deep();
    c.out_w_bhig_deep()+= tmp; b.out_w_bhig_deep() = tmp;
    
    for(size_t i = 0; i<2; ++i){
      tmp = fix_b_wgt ? btw.EventWeight(b, op_med, i==0 ? vup : vdown, ctr, doDeepCSV, doProc)        : b.sys_bctag_deep().at(i);
      c.out_sys_bctag_deep().at(i)+= tmp; b.out_sys_bctag_deep().at(i) = tmp;
      tmp = fix_b_wgt ? btw.EventWeight(b, op_med, ctr, i==0 ? vup : vdown, doDeepCSV, doProc)        : b.sys_udsgtag_deep().at(i);
      c.out_sys_udsgtag_deep().at(i)+= tmp; b.out_sys_udsgtag_deep().at(i) = tmp;
      tmp = fix_b_wgt ? btw.EventWeight(b, op_loose, i==0 ? vup : vdown, ctr, doDeepCSV, doProc)        : b.sys_bctag_loose_deep().at(i);
      c.out_sys_bctag_loose_deep().at(i)+= tmp; b.out_sys_bctag_loose_deep().at(i) = tmp;
      tmp = fix_b_wgt ? btw.EventWeight(b, op_loose, ctr, i==0 ? vup : vdown, doDeepCSV, doProc)        : b.sys_udsgtag_loose_deep().at(i);
      c.out_sys_udsgtag_loose_deep().at(i)+= tmp; b.out_sys_udsgtag_loose_deep().at(i) = tmp;
      tmp = fix_b_wgt ? btw.EventWeight(b, op_tight, i==0 ? vup : vdown, ctr, doDeepCSV, doProc)        : b.sys_bctag_tight_deep().at(i);
      c.out_sys_bctag_tight_deep().at(i)+= tmp; b.out_sys_bctag_tight_deep().at(i) = tmp;
      tmp = fix_b_wgt ? btw.EventWeight(b, op_tight, ctr, i==0 ? vup : vdown, doDeepCSV, doProc)        : b.sys_udsgtag_tight_deep().at(i);
      c.out_sys_udsgtag_tight_deep().at(i)+= tmp; b.out_sys_udsgtag_tight_deep().at(i) = tmp;

      tmp = fix_b_wgt ? btw.EventWeight(b, op_all, i==0 ? vup : vdown, ctr, doDeepCSV, doProc)        : b.sys_bchig_deep().at(i);
      c.out_sys_bchig_deep().at(i)+= tmp; b.out_sys_bchig_deep().at(i) = tmp;
      tmp = fix_b_wgt ? btw.EventWeight(b, op_all, ctr, i==0 ? vup : vdown, doDeepCSV, doProc)        : b.sys_udsghig_deep().at(i);
      c.out_sys_udsghig_deep().at(i)+= tmp; b.out_sys_udsghig_deep().at(i) = tmp;

      if(isFastSim){ 
        tmp = fix_b_wgt ? btw.EventWeight(b, op_med, ctr, ctr, i==0 ? vup : vdown, ctr, doDeepCSV, doProc)  : b.sys_fs_bctag_deep().at(i);
        c.out_sys_fs_bctag_deep().at(i)+= tmp; b.out_sys_fs_bctag_deep().at(i) = tmp;
        tmp = fix_b_wgt ? btw.EventWeight(b, op_med, ctr, ctr, ctr, i==0 ? vup : vdown, doDeepCSV, doProc)  : b.sys_fs_udsgtag_deep().at(i);
        c.out_sys_fs_udsgtag_deep().at(i)+= tmp; b.out_sys_fs_udsgtag_deep().at(i) = tmp;
        tmp = fix_b_wgt ? btw.EventWeight(b, op_all, ctr, ctr, i==0 ? vup : vdown, ctr, doDeepCSV, doProc) : b.sys_fs_bchig_deep().at(i);
        c.out_sys_fs_bchig_deep().at(i)+= tmp; b.out_sys_fs_bchig_deep().at(i) = tmp;
        tmp = fix_b_wgt ? btw.EventWeight(b, op_all, ctr, ctr, i==0 ? vup : vdown, ctr, doDeepCSV, doProc) : b.sys_fs_udsghig_deep().at(i);
        c.out_sys_fs_udsghig_deep().at(i)+= tmp; b.out_sys_fs_udsghig_deep().at(i) = tmp;
      }
    
      c.out_sys_pu().at(i)    += b.sys_pu().at(i);
      c.out_sys_mur().at(i)   += b.sys_mur().at(i);
      c.out_sys_muf().at(i)   += b.sys_muf().at(i);
      c.out_sys_murf().at(i)  += b.sys_murf().at(i);
    } // loop over 2 sys

    for(size_t i = 0; i<b.w_pdf().size(); ++i) c.out_w_pdf().at(i) += b.w_pdf().at(i);
    for(size_t i = 0; i<b.sys_pdf().size(); ++i) c.out_sys_pdf().at(i) += b.sys_pdf().at(i);

    b.Fill();
  } // loop over events

  c.out_neff() = neff;

  c.Fill();
  c.Write();
  b.Write();

  cout<<endl;
  time(&endtime); 
  cout<<"Time passed: "<<hoursMinSec(difftime(endtime, begtime))<<endl<<endl;  
}

void GetOptions(int argc, char *argv[]){
  while(true){
    static struct option long_options[] = {
      {"in_file", required_argument, 0, 'i'},  // Input file with unmodified weights
      {"wgt_dir", required_argument, 0, 'w'}, // Directory in which to put sum-of-weights correction file
      {"out_dir", required_argument, 0, 'o'},  // Directory in which to place modified baby
      {"year", required_argument, 0, 'y'},  
      {"fastsim", no_argument, 0, 0},       
      {"keep_b_wgt", no_argument, 0, 0},       // Use existing b-tag weights/systematics instead of applying new SFs
      {"keep_lep_wgt", no_argument, 0, 0},     // Use existing lepton weights/systematics instead of applying new SFs
      {0, 0, 0, 0}
    };

    char opt = -1;
    int option_index;
    opt = getopt_long(argc, argv, "i:w:o:y:", long_options, &option_index);
    if(opt == -1) break;

    string optname;
    switch(opt){
    case 'i':
      in_file = optarg;
      break;
    case 'w':
      wgt_dir = optarg;
      break;
    case 'o':
      out_dir = optarg;
      break;
    case 'y':
      year = atoi(optarg);
      break;
    case 0:
      optname = long_options[option_index].name;
      if(optname == "fastsim"){
        isFastSim = true;
      }else if(optname == "keep_b_wgt"){
        fix_b_wgt = false;
      }else if(optname == "keep_lep_wgt"){
        fix_lep_wgt = false;
      }else{
        printf("Bad option! Found option name %s\n", optname.c_str());
        exit(1);
      }
      break;
    default:
      printf("Bad option! getopt_long returned character code 0%o\n", opt);
      break;
    }
  }
}
