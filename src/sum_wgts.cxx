#include <iostream>
#include <ctime>
#include <getopt.h>

#include "baby_base.hpp"
#include "utilities.hpp"

#include "TError.h"

using namespace std;

namespace {
  string indir = "/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/unprocessed/";
  string tag = "fullbaby_TTJets_Tune";
  string outdir = "/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/sum_wgts/";
}

void GetOptions(int argc, char *argv[]);

int main(int argc, char *argv[]){
  gErrorIgnoreLevel=6000; // Turns off ROOT errors due to missing branches       
  GetOptions(argc, argv);

  time_t begtime, endtime;
  time(&begtime);

  bool doSumOutputTree = true;
  baby_base b(indir+"/*"+tag+"*.root", outdir+"/sum_"+tag+".root", doSumOutputTree);
  long nevents = b.GetEntries();
  bool isSignal = false;
  for(long entry(0); entry<nevents; entry++){
    if (entry==0 && b.type()>100e3) isSignal = true;

    b.GetEntry(entry);
    if (entry%1000==0) cout<<"Processing event: "<<entry<<endl;

    // Cookie-cutter variables
    b.out_w_pu()                   += b.w_pu();
    b.out_w_btag()                 += b.w_btag();
    b.out_w_btag_deep()            += b.w_btag_deep();
    b.out_w_btag_loose()           += b.w_btag_loose();
    b.out_w_btag_loose_deep()      += b.w_btag_loose_deep();
    b.out_w_btag_tight()           += b.w_btag_tight();
    b.out_w_btag_tight_deep()      += b.w_btag_tight_deep();
    b.out_w_bhig()                 += b.w_bhig();
    b.out_w_bhig_deep()            += b.w_bhig_deep();
    b.out_w_btag_proc()            += b.w_btag_proc();
    b.out_w_btag_deep_proc()       += b.w_btag_deep_proc();
    b.out_w_btag_loose_proc()      += b.w_btag_loose_proc();
    b.out_w_btag_loose_deep_proc() += b.w_btag_loose_deep_proc();
    b.out_w_btag_tight_proc()      += b.w_btag_tight_proc();
    b.out_w_btag_tight_deep_proc() += b.w_btag_tight_deep_proc();
    b.out_w_bhig_proc()            += b.w_bhig_proc();
    b.out_w_bhig_deep_proc()       += b.w_bhig_deep_proc();

    for (unsigned i(0); i<b.w_pdf().size(); i++)  b.out_w_pdf()[i] += b.w_pdf()[i];

    for (unsigned i(0); i<b.sys_mur().size(); i++) {
      b.out_sys_mur()[i]                     += b.sys_mur()[i];
      b.out_sys_muf()[i]                     += b.sys_muf()[i];
      b.out_sys_murf()[i]                    += b.sys_murf()[i];
      b.out_sys_pdf()[i]                     += b.sys_pdf()[i];
      b.out_sys_bctag()[i]                   += b.sys_bctag()[i];
      b.out_sys_bctag_deep()[i]              += b.sys_bctag_deep()[i];
      b.out_sys_udsgtag()[i]                 += b.sys_udsgtag()[i];
      b.out_sys_udsgtag_deep()[i]            += b.sys_udsgtag_deep()[i];
      b.out_sys_bctag_loose()[i]             += b.sys_bctag_loose()[i];
      b.out_sys_bctag_loose_deep()[i]        += b.sys_bctag_loose_deep()[i];
      b.out_sys_udsgtag_loose()[i]           += b.sys_udsgtag_loose()[i];
      b.out_sys_udsgtag_loose_deep()[i]      += b.sys_udsgtag_loose_deep()[i];
      b.out_sys_bctag_tight()[i]             += b.sys_bctag_tight()[i];
      b.out_sys_bctag_tight_deep()[i]        += b.sys_bctag_tight_deep()[i];
      b.out_sys_udsgtag_tight()[i]           += b.sys_udsgtag_tight()[i];
      b.out_sys_udsgtag_tight_deep()[i]      += b.sys_udsgtag_tight_deep()[i];
      b.out_sys_bchig()[i]                   += b.sys_bchig()[i];
      b.out_sys_bchig_deep()[i]              += b.sys_bchig_deep()[i];
      b.out_sys_udsghig()[i]                 += b.sys_udsghig()[i];
      b.out_sys_udsghig_deep()[i]            += b.sys_udsghig_deep()[i];
      b.out_sys_bctag_proc()[i]              += b.sys_bctag_proc()[i];
      b.out_sys_bctag_deep_proc()[i]         += b.sys_bctag_deep_proc()[i];
      b.out_sys_udsgtag_proc()[i]            += b.sys_udsgtag_proc()[i];
      b.out_sys_udsgtag_deep_proc()[i]       += b.sys_udsgtag_deep_proc()[i];
      b.out_sys_bctag_loose_proc()[i]        += b.sys_bctag_loose_proc()[i];
      b.out_sys_bctag_loose_deep_proc()[i]   += b.sys_bctag_loose_deep_proc()[i];
      b.out_sys_udsgtag_loose_proc()[i]      += b.sys_udsgtag_loose_proc()[i];
      b.out_sys_udsgtag_loose_deep_proc()[i] += b.sys_udsgtag_loose_deep_proc()[i];
      b.out_sys_bctag_tight_proc()[i]        += b.sys_bctag_tight_proc()[i];
      b.out_sys_bctag_tight_deep_proc()[i]   += b.sys_bctag_tight_deep_proc()[i];
      b.out_sys_udsgtag_tight_proc()[i]      += b.sys_udsgtag_tight_proc()[i];
      b.out_sys_udsgtag_tight_deep_proc()[i] += b.sys_udsgtag_tight_deep_proc()[i];
      b.out_sys_bchig_proc()[i]              += b.sys_bchig_proc()[i];
      b.out_sys_bchig_deep_proc()[i]         += b.sys_bchig_deep_proc()[i];
      b.out_sys_udsghig_proc()[i]            += b.sys_udsghig_proc()[i];
      b.out_sys_udsghig_deep_proc()[i]       += b.sys_udsghig_deep_proc()[i];
      b.out_sys_pu()[i]                      += b.sys_pu()[i];
      if (isSignal) { // yes, this ignores the fullsim points
        b.out_sys_fs_bctag()[i]              += b.sys_fs_bctag()[i];
        b.out_sys_fs_bctag_deep()[i]         += b.sys_fs_bctag_deep()[i];
        b.out_sys_fs_udsgtag()[i]            += b.sys_fs_udsgtag()[i];
        b.out_sys_fs_udsgtag_deep()[i]       += b.sys_fs_udsgtag_deep()[i];
        b.out_sys_fs_bchig()[i]              += b.sys_fs_bchig()[i];
        b.out_sys_fs_bchig_deep()[i]         += b.sys_fs_bchig_deep()[i];
        b.out_sys_fs_udsghig()[i]            += b.sys_fs_udsghig()[i];
        b.out_sys_fs_udsghig_deep()[i]       += b.sys_fs_udsghig_deep()[i];
      }
    }
  } 
  b.out_nevents() = nevents;
  b.Fill();
  b.Write();

  cout<<endl;
  time(&endtime); 
  cout<<"Time passed: "<<hoursMinSec(difftime(endtime, begtime))<<endl<<endl;  
}

void GetOptions(int argc, char *argv[]){
  while(true){
    static struct option long_options[] = {
      {"indir", required_argument, 0, 'i'},  // Method to run on (if you just want one)
      {"tag", required_argument, 0, 't'},       // Apply correction
      {"outdir", required_argument, 0, 'o'},    // Luminosity to normalize MC with (no data)
      {0, 0, 0, 0}
    };

    char opt = -1;
    int option_index;
    opt = getopt_long(argc, argv, "i:t:o:", long_options, &option_index);
    if(opt == -1) break;

    string optname;
    switch(opt){
    case 'i':
      indir = optarg;
      break;
    case 't':
      tag = optarg;
      break;
    case 'o':
      outdir = optarg;
      break;
    case 0:
    default:
      printf("Bad option! getopt_long returned character code 0%o\n", opt);
      break;
    }
  }
}