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
  string corr_file = "";
  string in_file = "";
  string out_file = "";
  int year = 2016;
  bool isFastSim = false;
}

void GetOptions(int argc, char *argv[]);

int main(int argc, char *argv[]){
  // gErrorIgnoreLevel=6000; // Turns off ROOT errors due to missing branches       
  GetOptions(argc, argv);

  time_t begtime, endtime;
  time(&begtime);


  cout << "Running on "<< (isFastSim ? "FastSim" : "FullSim") << endl;
  cout << "Calculating weights based on " << year << " inputs." << endl;

  cout<<"Input file: "<<in_file<<endl;
  baby_plus b(in_file, out_file);
  long nent = b.GetEntries();
  cout<<"Running over "<<nent<<" events."<<endl;

  cout<<"Corr. file: "<<corr_file<<endl;
  baby_corr c(corr_file);
  if (c.GetEntries()==1) {
    cout<<"Correction file OK."<<endl;
    c.GetEntry(0);
  } else {
    cout<<"No entries in the corrections files."<<endl;
  }

  for(long entry(0); entry<nent; entry++){

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
    if (!isFastSim) b.out_baseline() = b.out_baseline() && b.pass();

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

    if (year==2016) {
      b.out_weight() = c.weight() *b.out_w_lumi() 
                     *b.out_w_lep() *b.out_w_fs_lep() //post-corr values in order for 0l to be correct
                     *b.w_btag_deep() *b.w_isr() *b.w_pu();
    } else {
      b.out_weight() = c.weight() *b.out_w_lumi() 
                     *b.out_w_lep() *b.out_w_fs_lep() //post-corr values in order for 0l to be correct
                     *b.w_btag_deep() *b.w_pu();
      if (isFastSim) 
        b.out_weight() *= b.w_isr();
    }
    
    b.out_w_isr() = c.w_isr()*b.w_isr();
    for (unsigned i(0); i<b.sys_isr().size(); i++) 
      b.out_sys_isr()[i] = c.sys_isr()[i]*b.sys_isr()[i];

    //      Cookie-cutter variables
    //-----------------------------------
    b.out_w_pu()                   *= c.w_pu();
    b.out_w_btag_deep()            *= c.w_btag_deep();
    b.out_w_btag_loose_deep()      *= c.w_btag_loose_deep();  
    b.out_w_btag_tight_deep()      *= c.w_btag_tight_deep();

    b.out_w_bhig_deep()            *= c.w_bhig_deep();
    
    for (unsigned i(0); i<2; i++) {

      b.out_sys_pu()[i]                      *= c.sys_pu()[i];
      
      b.out_sys_bctag_deep()[i]              *= c.sys_bctag_deep()[i];
      b.out_sys_udsgtag_deep()[i]            *= c.sys_udsgtag_deep()[i];
      b.out_sys_bctag_loose_deep()[i]        *= c.sys_bctag_loose_deep()[i];
      b.out_sys_udsgtag_loose_deep()[i]      *= c.sys_udsgtag_loose_deep()[i];
      b.out_sys_bctag_tight_deep()[i]        *= c.sys_bctag_tight_deep()[i];
      b.out_sys_udsgtag_tight_deep()[i]      *= c.sys_udsgtag_tight_deep()[i];
        
      b.out_sys_bchig_deep()[i]              *= c.sys_bchig_deep()[i];
      b.out_sys_udsghig_deep()[i]            *= c.sys_udsghig_deep()[i];

      if (isFastSim) { // yes, this ignores the fullsim points
        b.out_sys_mur()[i]                     *= c.sys_mur()[i];
        b.out_sys_muf()[i]                     *= c.sys_muf()[i];
        b.out_sys_murf()[i]                    *= c.sys_murf()[i];

        b.out_sys_fs_bctag_deep()[i]         *= c.sys_fs_bctag_deep()[i];
        b.out_sys_fs_udsgtag_deep()[i]       *= c.sys_fs_udsgtag_deep()[i];
        b.out_sys_fs_bchig_deep()[i]         *= c.sys_fs_bchig_deep()[i];
        b.out_sys_fs_udsghig_deep()[i]       *= c.sys_fs_udsghig_deep()[i];
      }
    
    } 

    for (unsigned i(0); i<b.w_pdf().size(); i++) b.out_w_pdf()[i] *= c.w_pdf()[i];    
    for (unsigned i(0); i<b.sys_pdf().size(); i++) b.out_sys_pdf()[i] *= c.sys_pdf()[i];    
    
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
      {"in_file", required_argument, 0, 'i'},  
      {"corr_file", required_argument, 0, 'c'},
      {"out_file", required_argument, 0, 'o'}, 
      {"year", required_argument, 0, 'y'},
      {"fastsim", no_argument, 0, 0},  
      {0, 0, 0, 0}
    };

    char opt = -1;
    int option_index;
    opt = getopt_long(argc, argv, "i:c:o:y:", long_options, &option_index);
    if(opt == -1) break;

    string optname;
    switch(opt){
    case 'i':
      in_file = optarg;
      break;
    case 'c':
      corr_file = optarg;
      break;
    case 'o':
      out_file = optarg;
      break;
    case 'y':
      year = atoi(optarg);
      break;
    case 0:
      optname = long_options[option_index].name;
      if(optname == "fastsim"){
        isFastSim = true;
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
