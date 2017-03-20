#include <iostream>
#include <ctime>
#include <getopt.h>

#include "baby_plus.hpp"
#include "baby_corr.hpp"
#include "utilities.hpp"
#include "hig_utils.hpp"
#include "cross_sections.hpp"

#include "TError.h"

using namespace std;

namespace {
  string corrfile = "/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/corrections/corr_fullbaby_TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1_60.root";
  string infile = "/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/unprocessed/fullbaby_TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1_60.root";
  string outfile = "test.root";
  bool quick = false;
}

void GetOptions(int argc, char *argv[]);

int main(int argc, char *argv[]){
  // gErrorIgnoreLevel=6000; // Turns off ROOT errors due to missing branches       
  GetOptions(argc, argv);

  time_t begtime, endtime;
  time(&begtime);

  cout<<"Input file: "<<infile<<endl;
  baby_plus b(infile, outfile);
  long nent = b.GetEntries();
  cout<<"Running over "<<nent<<" events."<<endl;

  cout<<"Corr. file: "<<corrfile<<endl;
  baby_corr c(corrfile);
  if (c.GetEntries()==1) {
    cout<<"Correction file OK."<<endl;
    c.GetEntry(0);
  } else {
    cout<<"No entries in the corrections files."<<endl;
  }

  bool isSignal = false;
  for(long entry(0); entry<nent; entry++){
    if (b.type()>100e3) isSignal = true;

    b.GetEntry(entry);
    if (entry%100000==0) {
      cout<<"Processing event: "<<entry<<endl;
    }

    if (b.type() == 106e3) { // TCHiHH
      // trigger efficiency and uncertainty
      b.out_eff_trig() = hig_utils::eff_higtrig(b);
      float effunc = hig_utils::effunc_higtrig(b);
      b.out_sys_trig();
      b.out_sys_trig().at(0) = 1+effunc;
      b.out_sys_trig().at(1) = 1-effunc;
      // fix mass point branch
      b.out_mgluino() = hig_utils::mchi(b);
    }

    b.out_baseline() = b.pass_ra2_badmu() && b.met()/b.met_calo()<5
                       && b.nleps()==1 && b.nveto()==0 && b.met()>200
                       && b.st()>500 && b.njets()>=6 && b.nbm()>=1;
    if (!isSignal) b.out_baseline() = b.out_baseline() && b.pass();

    if(b.nleps()==0) { // load from calculated correction
      b.out_w_lep()         = c.w_lep();
      b.out_w_fs_lep()      = c.w_fs_lep();
      for (unsigned i(0); i<b.sys_lep().size(); i++) 
        b.out_sys_lep()[i] = c.sys_lep()[i];
      for (unsigned i(0); i<b.sys_fs_lep().size(); i++) 
        b.out_sys_fs_lep()[i] = c.sys_fs_lep()[i];
    } else { //load from original tree
      b.out_w_lep()         = b.w_lep();
      b.out_w_fs_lep()      = b.w_fs_lep();
      for (unsigned i(0); i<b.sys_lep().size(); i++) 
        b.out_sys_lep()[i] = b.sys_lep()[i];
      for (unsigned i(0); i<b.sys_fs_lep().size(); i++) 
        b.out_sys_fs_lep()[i] = b.sys_fs_lep()[i];
    }

    b.out_w_lumi() = b.w_lumi()>0 ? 1. : -1.;
    b.out_w_lumi() *= c.w_lumi();

    b.out_weight() = c.weight() *b.out_w_lumi() 
                     *b.out_w_lep() *b.out_w_fs_lep() //post-corr values in order for 0l to be correct
                     *b.w_btag() *b.w_isr() *b.eff_jetid();
    
    b.out_w_isr() = c.w_isr()*b.w_isr();
    for (unsigned i(0); i<b.sys_isr().size(); i++) 
      b.out_sys_isr()[i] = c.sys_isr()[i]*b.sys_isr()[i];

    //      Cookie-cutter variables
    //-----------------------------------
    b.out_w_pu()                   *= c.w_pu();
    b.out_w_btag()                 *= c.w_btag();
    b.out_w_btag_bf()              *= c.w_btag_bf();
    b.out_w_btag_gh()              *= c.w_btag_gh();
    b.out_w_btag_b()               *= c.w_btag_b();
    b.out_w_btag_cd()              *= c.w_btag_cd();
    b.out_w_btag_ef()              *= c.w_btag_ef();
    b.out_w_btag_proc()            *= c.w_btag_proc();
    b.out_w_btag_deep()            *= c.w_btag_deep();
    b.out_w_btag_deep_proc()       *= c.w_btag_deep_proc();

    b.out_w_bhig()                 *= c.w_bhig();
    b.out_w_bhig_proc()            *= c.w_bhig_proc();
    b.out_w_bhig_deep()            *= c.w_bhig_deep();
    b.out_w_bhig_deep_bf()         *= c.w_bhig_deep_bf();
    b.out_w_bhig_deep_gh()         *= c.w_bhig_deep_gh();
    b.out_w_bhig_deep_proc()       *= c.w_bhig_deep_proc();
    
    for (unsigned i(0); i<2; i++) {
      b.out_sys_bctag()[i]                   *= c.sys_bctag()[i];
      b.out_sys_bctag_bf()[i]                *= c.sys_bctag_bf()[i];
      b.out_sys_bctag_gh()[i]                *= c.sys_bctag_gh()[i];
      b.out_sys_bctag_b()[i]                 *= c.sys_bctag_b()[i];
      b.out_sys_bctag_cd()[i]                *= c.sys_bctag_cd()[i];
      b.out_sys_bctag_ef()[i]                *= c.sys_bctag_ef()[i];
      b.out_sys_bctag_proc()[i]              *= c.sys_bctag_proc()[i];
      b.out_sys_bctag_deep()[i]              *= c.sys_bctag_deep()[i];
      b.out_sys_bctag_deep_proc()[i]         *= c.sys_bctag_deep_proc()[i];
      b.out_sys_udsgtag()[i]                 *= c.sys_udsgtag()[i];
      b.out_sys_udsgtag_bf()[i]              *= c.sys_udsgtag_bf()[i];
      b.out_sys_udsgtag_gh()[i]              *= c.sys_udsgtag_gh()[i];
      b.out_sys_udsgtag_b()[i]               *= c.sys_udsgtag_b()[i];
      b.out_sys_udsgtag_cd()[i]              *= c.sys_udsgtag_cd()[i];
      b.out_sys_udsgtag_ef()[i]              *= c.sys_udsgtag_ef()[i];
      b.out_sys_udsgtag_proc()[i]            *= c.sys_udsgtag_proc()[i];
      b.out_sys_udsgtag_deep()[i]            *= c.sys_udsgtag_deep()[i];
      b.out_sys_udsgtag_deep_proc()[i]       *= c.sys_udsgtag_deep_proc()[i];
        
      b.out_sys_bchig()[i]                   *= c.sys_bchig()[i];
      b.out_sys_bchig_proc()[i]              *= c.sys_bchig_proc()[i];
      b.out_sys_bchig_deep()[i]              *= c.sys_bchig_deep()[i];
      b.out_sys_bchig_deep_bf()[i]              *= c.sys_bchig_deep_bf()[i];
      b.out_sys_bchig_deep_gh()[i]              *= c.sys_bchig_deep_gh()[i];
      b.out_sys_bchig_deep_proc()[i]         *= c.sys_bchig_deep_proc()[i];
      b.out_sys_udsghig()[i]                 *= c.sys_udsghig()[i];
      b.out_sys_udsghig_proc()[i]            *= c.sys_udsghig_proc()[i];
      b.out_sys_udsghig_deep()[i]            *= c.sys_udsghig_deep()[i];
      b.out_sys_udsghig_deep_bf()[i]            *= c.sys_udsghig_deep_bf()[i];
      b.out_sys_udsghig_deep_gh()[i]            *= c.sys_udsghig_deep_gh()[i];
      b.out_sys_udsghig_deep_proc()[i]       *= c.sys_udsghig_deep_proc()[i];

      if (isSignal) { // yes, this ignores the fullsim points
        b.out_sys_mur()[i]                     *= c.sys_mur()[i];
        b.out_sys_muf()[i]                     *= c.sys_muf()[i];
        b.out_sys_murf()[i]                    *= c.sys_murf()[i];

        b.out_sys_fs_bctag()[i]              *= c.sys_fs_bctag()[i];
        b.out_sys_fs_bctag_deep()[i]         *= c.sys_fs_bctag_deep()[i];
        b.out_sys_fs_udsgtag()[i]            *= c.sys_fs_udsgtag()[i];
        b.out_sys_fs_udsgtag_deep()[i]       *= c.sys_fs_udsgtag_deep()[i];
        b.out_sys_fs_bchig()[i]              *= c.sys_fs_bchig()[i];
        b.out_sys_fs_bchig_deep()[i]         *= c.sys_fs_bchig_deep()[i];
        b.out_sys_fs_udsghig()[i]            *= c.sys_fs_udsghig()[i];
        b.out_sys_fs_udsghig_deep()[i]       *= c.sys_fs_udsghig_deep()[i];
      }
    }

    if (!quick) {
      b.out_w_btag_loose()           *= c.w_btag_loose();
      b.out_w_btag_bf_loose()        *= c.w_btag_bf_loose();
      b.out_w_btag_gh_loose()        *= c.w_btag_gh_loose();
      b.out_w_btag_b_loose()         *= c.w_btag_b_loose();
      b.out_w_btag_cd_loose()        *= c.w_btag_cd_loose();
      b.out_w_btag_ef_loose()        *= c.w_btag_ef_loose();
      b.out_w_btag_loose_deep()      *= c.w_btag_loose_deep();
      b.out_w_btag_tight()           *= c.w_btag_tight();
      b.out_w_btag_bf_tight()        *= c.w_btag_bf_tight();
      b.out_w_btag_gh_tight()        *= c.w_btag_gh_tight();
      b.out_w_btag_b_tight()         *= c.w_btag_b_tight();
      b.out_w_btag_cd_tight()        *= c.w_btag_cd_tight();
      b.out_w_btag_ef_tight()        *= c.w_btag_ef_tight();
      b.out_w_btag_tight_deep()      *= c.w_btag_tight_deep();
      b.out_w_btag_loose_proc()      *= c.w_btag_loose_proc();
      b.out_w_btag_loose_deep_proc() *= c.w_btag_loose_deep_proc();
      b.out_w_btag_tight_proc()      *= c.w_btag_tight_proc();
      b.out_w_btag_tight_deep_proc() *= c.w_btag_tight_deep_proc();

      for (unsigned i(0); i<b.w_pdf().size(); i++) b.out_w_pdf()[i] *= c.w_pdf()[i];

      for (unsigned i(0); i<b.sys_mur().size(); i++) {
        b.out_sys_pu()[i]                      *= c.sys_pu()[i];
        // b.out_sys_pdf()[i]                     *= c.sys_pdf()[i];

        b.out_sys_bctag_loose()[i]             *= c.sys_bctag_loose()[i];
	b.out_sys_bctag_bf_loose()[i]          *= c.sys_bctag_bf_loose()[i];
	b.out_sys_bctag_gh_loose()[i]          *= c.sys_bctag_gh_loose()[i];
	b.out_sys_bctag_b_loose()[i]           *= c.sys_bctag_b_loose()[i];
	b.out_sys_bctag_cd_loose()[i]          *= c.sys_bctag_cd_loose()[i];
	b.out_sys_bctag_ef_loose()[i]          *= c.sys_bctag_ef_loose()[i];
        b.out_sys_bctag_loose_deep()[i]        *= c.sys_bctag_loose_deep()[i];
        b.out_sys_udsgtag_loose()[i]           *= c.sys_udsgtag_loose()[i];
	b.out_sys_udsgtag_bf_loose()[i]        *= c.sys_udsgtag_bf_loose()[i];
	b.out_sys_udsgtag_gh_loose()[i]        *= c.sys_udsgtag_gh_loose()[i];
	b.out_sys_udsgtag_b_loose()[i]         *= c.sys_udsgtag_b_loose()[i];
	b.out_sys_udsgtag_cd_loose()[i]        *= c.sys_udsgtag_cd_loose()[i];
	b.out_sys_udsgtag_ef_loose()[i]        *= c.sys_udsgtag_ef_loose()[i];
        b.out_sys_udsgtag_loose_deep()[i]      *= c.sys_udsgtag_loose_deep()[i];
        b.out_sys_bctag_tight()[i]             *= c.sys_bctag_tight()[i];
	b.out_sys_bctag_bf_tight()[i]          *= c.sys_bctag_bf_tight()[i];
	b.out_sys_bctag_gh_tight()[i]          *= c.sys_bctag_gh_tight()[i];
	b.out_sys_bctag_b_tight()[i]           *= c.sys_bctag_b_tight()[i];
	b.out_sys_bctag_cd_tight()[i]          *= c.sys_bctag_cd_tight()[i];
	b.out_sys_bctag_ef_tight()[i]          *= c.sys_bctag_ef_tight()[i];
        b.out_sys_bctag_tight_deep()[i]        *= c.sys_bctag_tight_deep()[i];
        b.out_sys_udsgtag_tight()[i]           *= c.sys_udsgtag_tight()[i];
	b.out_sys_udsgtag_bf_tight()[i]        *= c.sys_udsgtag_bf_tight()[i];
	b.out_sys_udsgtag_gh_tight()[i]        *= c.sys_udsgtag_gh_tight()[i];
	b.out_sys_udsgtag_b_tight()[i]         *= c.sys_udsgtag_b_tight()[i];
	b.out_sys_udsgtag_cd_tight()[i]        *= c.sys_udsgtag_cd_tight()[i];
	b.out_sys_udsgtag_ef_tight()[i]        *= c.sys_udsgtag_ef_tight()[i];
        b.out_sys_udsgtag_tight_deep()[i]      *= c.sys_udsgtag_tight_deep()[i];
        b.out_sys_bctag_loose_proc()[i]        *= c.sys_bctag_loose_proc()[i];
        b.out_sys_bctag_loose_deep_proc()[i]   *= c.sys_bctag_loose_deep_proc()[i];
        b.out_sys_udsgtag_loose_proc()[i]      *= c.sys_udsgtag_loose_proc()[i];
        b.out_sys_udsgtag_loose_deep_proc()[i] *= c.sys_udsgtag_loose_deep_proc()[i];
        b.out_sys_bctag_tight_proc()[i]        *= c.sys_bctag_tight_proc()[i];
        b.out_sys_bctag_tight_deep_proc()[i]   *= c.sys_bctag_tight_deep_proc()[i];
        b.out_sys_udsgtag_tight_proc()[i]      *= c.sys_udsgtag_tight_proc()[i];
        b.out_sys_udsgtag_tight_deep_proc()[i] *= c.sys_udsgtag_tight_deep_proc()[i];
      } // loop over 2 sys
    } // if quick
    
    b.Fill();

  } // loop over events
  
  b.Write();

  cout<<endl;
  time(&endtime); 
  cout<<"Time passed: "<<hoursMinSec(difftime(endtime, begtime))<<endl<<endl;  
}

void GetOptions(int argc, char *argv[]){
  while(true){
    static struct option long_options[] = {
      {"infile", required_argument, 0, 'i'},  // Method to run on (if you just want one)
      {"corrfile", required_argument, 0, 'c'},       // Apply correction
      {"outfile", required_argument, 0, 'o'},    // Luminosity to normalize MC with (no data)
      {"quick", no_argument, 0, 0},  
      {0, 0, 0, 0}
    };

    char opt = -1;
    int option_index;
    opt = getopt_long(argc, argv, "qi:c:o:", long_options, &option_index);
    if(opt == -1) break;

    string optname;
    switch(opt){
    case 'i':
      infile = optarg;
      break;
    case 'c':
      corrfile = optarg;
      break;
    case 'o':
      outfile = optarg;
      break;
    case 0:
      optname = long_options[option_index].name;
      if(optname == "quick"){
        quick = true;
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
