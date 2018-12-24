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
  string corr_dir = "";
  string out_dir = "";
  bool quick = false;
  bool fix_b_wgt = true;
  bool fix_lep_wgt = false;
}

void GetOptions(int argc, char *argv[]);

int main(int argc, char *argv[]){
  // gErrorIgnoreLevel=6000; // Turns off ROOT errors due to missing branches       
  GetOptions(argc, argv);

  time_t begtime, endtime;
  time(&begtime);

  string base_dir = "", file_name = "";
  SplitFilePath(in_file, base_dir, file_name);

  if(corr_dir == "") corr_dir = base_dir+(quick ? string("/corrections") : string("/corrections_quick"));
  if(out_dir == "") out_dir = base_dir+"/reweighted";

  string corr_file = corr_dir + "/" + file_name;
  string out_file = out_dir + "/" + file_name;
  
  cout << "Input file: " << in_file << endl;
  baby_plus b(in_file, out_file);

  cout << "Writing output to: " << out_file << endl;
  cout << "Writing sum-of-weights to: " << corr_file << endl;

  bool isSignal = false;
  if(b.GetEntries() > 0){
    b.GetEntry(0);
    if(b.type()>=100e3) isSignal = true;
  }

  string proc = "tt";
  if(Contains(file_name, "WJets")) proc = "wjets";
  else if(Contains(file_name, "QCD")) proc = "qcd";
  //Need to improve to handle FullSim signal points
  BTagWeighter btw(proc, isSignal, false);

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

    float w_btag_deep = fix_b_wgt ? btw.EventWeight(b, op_med, ctr, ctr, true, false) : b.w_btag_deep();
    float w_lep(1.), w_fs_lep(1.);
    vector<float> sys_lep(2,1.), sys_fs_lep(2,1.);
    if(fix_lep_wgt){
      LeptonWeighter::FullSim(b, w_lep, sys_lep);
      if(isSignal) LeptonWeighter::FastSim(b, w_fs_lep, sys_fs_lep);
      b.out_w_lep() = w_lep;
      b.out_sys_lep() = sys_lep;
      b.out_w_fs_lep() = w_fs_lep;
      b.out_sys_fs_lep() = sys_fs_lep;
    }else{
      w_lep = b.w_lep();
      sys_lep = b.sys_lep();
      if(isSignal){
	    w_fs_lep = b.w_lep();
	    sys_fs_lep = b.sys_fs_lep();
      }
    }

    wgt = w_lep*(isSignal ? w_fs_lep : 1.)*
          w_btag_deep*b.w_isr()*b.w_pu();

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
      if(isSignal){
	c.out_w_fs_lep()      += w_fs_lep;
	for(size_t i = 0; i<b.sys_fs_lep().size(); ++i){
	  c.out_sys_fs_lep().at(i) += sys_fs_lep.at(i);
	}
      }
    }
    
    //      Cookie-cutter variables
    //-----------------------------------
    c.out_w_pu()             += b.w_pu();

    double tmp = 0.;
    
    c.out_w_btag_deep()+= w_btag_deep; b.out_w_btag_deep() = w_btag_deep;

    tmp = fix_b_wgt ? btw.EventWeight(b, op_all, ctr, ctr, true, false)        : b.w_bhig_deep();
    c.out_w_bhig_deep()+= tmp; b.out_w_bhig_deep() = tmp;
    
    for(size_t i = 0; i<2; ++i){
      tmp = fix_b_wgt ? btw.EventWeight(b, op_med, i==0 ? vup : vdown, ctr, true, false)        : b.sys_bctag_deep().at(i);
      c.out_sys_bctag_deep().at(i)+= tmp; b.out_sys_bctag_deep().at(i) = tmp;
      tmp = fix_b_wgt ? btw.EventWeight(b, op_med, ctr, i==0 ? vup : vdown, true, false)        : b.sys_udsgtag_deep().at(i);
      c.out_sys_udsgtag_deep().at(i)+= tmp; b.out_sys_udsgtag_deep().at(i) = tmp;
      
      tmp = fix_b_wgt ? btw.EventWeight(b, op_all, i==0 ? vup : vdown, ctr, true, false)        : b.sys_bchig_deep().at(i);
      c.out_sys_bchig_deep().at(i)+= tmp; b.out_sys_bchig_deep().at(i) = tmp;
      tmp = fix_b_wgt ? btw.EventWeight(b, op_all, ctr, i==0 ? vup : vdown, true, false)        : b.sys_udsghig_deep().at(i);
      c.out_sys_udsghig_deep().at(i)+= tmp; b.out_sys_udsghig_deep().at(i) = tmp;

      if(isSignal){ // yes, this ignores the fullsim points
        c.out_sys_mur().at(i)             += b.sys_mur().at(i);
        c.out_sys_muf().at(i)             += b.sys_muf().at(i);
        c.out_sys_murf().at(i)            += b.sys_murf().at(i);

        tmp = fix_b_wgt ? btw.EventWeight(b, op_med, ctr, ctr, i==0 ? vup : vdown, ctr, true, false)  : b.sys_fs_bctag_deep().at(i);
        c.out_sys_fs_bctag_deep().at(i)+= tmp; b.out_sys_fs_bctag_deep().at(i) = tmp;
        tmp = fix_b_wgt ? btw.EventWeight(b, op_med, ctr, ctr, ctr, i==0 ? vup : vdown, true, false)  : b.sys_fs_udsgtag_deep().at(i);
        c.out_sys_fs_udsgtag_deep().at(i)+= tmp; b.out_sys_fs_udsgtag_deep().at(i) = tmp;
        tmp = fix_b_wgt ? btw.EventWeight(b, op_all, ctr, ctr, i==0 ? vup : vdown, ctr, false, false) : b.sys_fs_bchig_deep().at(i);
        c.out_sys_fs_bchig_deep().at(i)+= tmp; b.out_sys_fs_bchig_deep().at(i) = tmp;
        tmp = fix_b_wgt ? btw.EventWeight(b, op_all, ctr, ctr, i==0 ? vup : vdown, ctr, false, false) : b.sys_fs_udsghig_deep().at(i);
        c.out_sys_fs_udsghig_deep().at(i)+= tmp; b.out_sys_fs_udsghig_deep().at(i) = tmp;
      }
    }

    if(!quick){
      tmp = fix_b_wgt ? btw.EventWeight(b, op_loose, ctr, ctr, true, false)        : b.w_btag_loose_deep();
      c.out_w_btag_loose_deep()+= tmp; b.out_w_btag_loose_deep() = tmp;
      tmp = fix_b_wgt ? btw.EventWeight(b, op_tight, ctr, ctr, true, false)        : b.w_btag_tight_deep();
      c.out_w_btag_tight_deep()+= tmp; b.out_w_btag_tight_deep() = tmp;

      for(size_t i = 0; i<b.w_pdf().size(); ++i){
	    c.out_w_pdf().at(i) += b.w_pdf().at(i);
      }

      for(size_t i = 0; i<b.sys_mur().size(); ++i){
        c.out_sys_pu().at(i)                      += b.sys_pu().at(i);
	if(i < b.sys_pdf().size()){
	  c.out_sys_pdf().at(i)                   += b.sys_pdf().at(i);
	}

        tmp = fix_b_wgt ? btw.EventWeight(b, op_loose, i==0 ? vup : vdown, ctr, true, false)        : b.sys_bctag_loose_deep().at(i);
        c.out_sys_bctag_loose_deep().at(i)+= tmp; b.out_sys_bctag_loose_deep().at(i) = tmp;
        tmp = fix_b_wgt ? btw.EventWeight(b, op_loose, ctr, i==0 ? vup : vdown, true, false)        : b.sys_udsgtag_loose_deep().at(i);
        c.out_sys_udsgtag_loose_deep().at(i)+= tmp; b.out_sys_udsgtag_loose_deep().at(i) = tmp;
        tmp = fix_b_wgt ? btw.EventWeight(b, op_tight, i==0 ? vup : vdown, ctr, true, false)        : b.sys_bctag_tight_deep().at(i);
        c.out_sys_bctag_tight_deep().at(i)+= tmp; b.out_sys_bctag_tight_deep().at(i) = tmp;
        tmp = fix_b_wgt ? btw.EventWeight(b, op_tight, ctr, i==0 ? vup : vdown, true, false)        : b.sys_udsgtag_tight_deep().at(i);
        c.out_sys_udsgtag_tight_deep().at(i)+= tmp; b.out_sys_udsgtag_tight_deep().at(i) = tmp;
      } // loop over 2 sys
    } // if quick
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
      {"in_file", required_argument, 0, 'f'},  // Input file with unmodified weights
      {"corr_dir", required_argument, 0, 'c'}, // Directory in which to put sum-of-weights correction file
      {"out_dir", required_argument, 0, 'o'},  // Directory in which to place modified baby
      {"keep_b_wgt", no_argument, 0, 0},       // Use existing b-tag weights/systematics instead of applying new SFs
      {"keep_lep_wgt", no_argument, 0, 0},     // Use existing lepton weights/systematics instead of applying new SFs
      {"quick", no_argument, 0, 0},            // Leave less important variables uncorrected
      {0, 0, 0, 0}
    };

    char opt = -1;
    int option_index;
    opt = getopt_long(argc, argv, "f:c:o:", long_options, &option_index);
    if(opt == -1) break;

    string optname;
    switch(opt){
    case 'f':
      in_file = optarg;
      break;
    case 'c':
      corr_dir = optarg;
      break;
    case 'o':
      out_dir = optarg;
      break;
    case 0:
      optname = long_options[option_index].name;
      if(optname == "quick"){
        quick = true;
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
