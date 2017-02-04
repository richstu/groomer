#include <iostream>
#include <ctime>
#include <getopt.h>

#include "baby_plus.hpp"
#include "baby_corr.hpp"
#include "utilities.hpp"
#include "cross_sections.hpp"

#include "TError.h"

using namespace std;

namespace {
  string indir = "/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/unprocessed/";
  string tag = "fullbaby_TTJets_TuneCUETP8M1_13TeV-madgraphMLM-pythia8_RunIISpring16MiniAODv2-PUSpring16_80X_mcRun2_asymptotic_2016_miniAODv2_v0-v1_60";
  string outdir = "/net/cms29/cms29r0/babymaker/babies/2017_01_27/mc/corrections/";
  bool quick = false;
}

void GetOptions(int argc, char *argv[]);

int main(int argc, char *argv[]){
  // gErrorIgnoreLevel=6000; // Turns off ROOT errors due to missing branches       
  GetOptions(argc, argv);

  time_t begtime, endtime;
  time(&begtime);

  string fin(tag), fout(tag);
  if (!Contains(tag,".root")) {
    fin += "*.root";
    fout +=".root";
  }
  cout<<"Chaining inputs: "<<indir+"/*"+fin<<endl;
  baby_plus b(indir+"/*"+fin);

  string outfile = outdir+"/corr_"+fout;
  if (quick) outfile = outdir+"/corrquick_"+fout;
  cout<<"Writing output to: "<<outfile<<endl;
  baby_corr c("", outfile);
  c.out_w_pdf().resize(100,0);
  c.out_sys_isr().resize(2,0);
  c.out_sys_lep().resize(2,0);
  c.out_sys_fs_lep().resize(2,0);
  c.out_sys_bctag().resize(2,0);
  c.out_sys_bctag_proc().resize(2,0);
  c.out_sys_bctag_deep().resize(2,0);
  c.out_sys_bctag_deep_proc().resize(2,0);
  c.out_sys_udsgtag().resize(2,0);
  c.out_sys_udsgtag_proc().resize(2,0);
  c.out_sys_udsgtag_deep().resize(2,0);
  c.out_sys_udsgtag_deep_proc().resize(2,0);
  c.out_sys_bchig().resize(2,0);
  c.out_sys_bchig_proc().resize(2,0);
  c.out_sys_bchig_deep().resize(2,0);
  c.out_sys_bchig_deep_proc().resize(2,0);
  c.out_sys_udsghig().resize(2,0);
  c.out_sys_udsghig_proc().resize(2,0);
  c.out_sys_udsghig_deep().resize(2,0);
  c.out_sys_udsghig_deep_proc().resize(2,0);
  c.out_sys_mur().resize(2,0);
  c.out_sys_muf().resize(2,0);
  c.out_sys_murf().resize(2,0);
  c.out_sys_fs_bctag().resize(2,0);
  c.out_sys_fs_bctag_deep().resize(2,0);
  c.out_sys_fs_udsgtag().resize(2,0);
  c.out_sys_fs_udsgtag_deep().resize(2,0);
  c.out_sys_fs_bchig().resize(2,0);
  c.out_sys_fs_bchig_deep().resize(2,0);
  c.out_sys_fs_udsghig().resize(2,0);
  c.out_sys_fs_udsghig_deep().resize(2,0);
  c.out_sys_pu().resize(2,0);
  c.out_sys_pdf().resize(2,0);
  c.out_sys_bctag_loose().resize(2,0);
  c.out_sys_bctag_loose_deep().resize(2,0);
  c.out_sys_udsgtag_loose().resize(2,0);
  c.out_sys_udsgtag_loose_deep().resize(2,0);
  c.out_sys_bctag_tight().resize(2,0);
  c.out_sys_bctag_tight_deep().resize(2,0);
  c.out_sys_udsgtag_tight().resize(2,0);
  c.out_sys_udsgtag_tight_deep().resize(2,0);
  c.out_sys_bctag_loose_proc().resize(2,0);
  c.out_sys_bctag_loose_deep_proc().resize(2,0);
  c.out_sys_udsgtag_loose_proc().resize(2,0);
  c.out_sys_udsgtag_loose_deep_proc().resize(2,0);
  c.out_sys_bctag_tight_proc().resize(2,0);
  c.out_sys_bctag_tight_deep_proc().resize(2,0);
  c.out_sys_udsgtag_tight_proc().resize(2,0);
  c.out_sys_udsgtag_tight_deep_proc().resize(2,0);

  // quantities to keep track of;
  int neff(0);
  double wgt(0), tot_weight_l0(0), tot_weight_l1(0), nent_zlep(0);

  long nent = b.GetEntries();
  cout<<"Running over "<<nent<<" events."<<endl;
  bool isSignal = false;
  for(long entry(0); entry<nent; entry++){
    if (entry==0 && b.type()>100e3) isSignal = true;

    b.GetEntry(entry);
    if (entry%100000==0 || entry == nent-1) {
      cout<<"Processing event: "<<entry<<endl;
    }

    wgt = b.w_lep()*b.w_fs_lep()*
          b.w_btag()*b.w_isr();//*b.w_pu();

    // need special treatment in summing and/or renormalizing
    neff += b.w_lumi()>0 ? 1:-1;

    c.out_w_isr() += b.w_isr();
    for (unsigned i(0); i<b.sys_isr().size(); i++) c.out_sys_isr()[i] += b.sys_isr()[i];

    if(b.nleps()==0) {
      nent_zlep += 1.;
      tot_weight_l0  += wgt;
    } else{
      tot_weight_l1         += wgt;
      c.out_w_lep()         += b.w_lep();
      c.out_w_fs_lep()      += b.w_fs_lep();
      for (unsigned i(0); i<b.sys_lep().size(); i++) c.out_sys_lep()[i] += b.sys_lep()[i];
      for (unsigned i(0); i<b.sys_fs_lep().size(); i++) c.out_sys_fs_lep()[i] += b.sys_fs_lep()[i];
    }
    
    //      Cookie-cutter variables
    //-----------------------------------
    c.out_w_pu()                   += b.w_pu();
    c.out_w_btag()                 += b.w_btag();
    c.out_w_btag_proc()            += b.w_btag_proc();
    c.out_w_btag_deep()            += b.w_btag_deep();
    c.out_w_btag_deep_proc()       += b.w_btag_deep_proc();

    c.out_w_bhig()                 += b.w_bhig();
    c.out_w_bhig_proc()            += b.w_bhig_proc();
    c.out_w_bhig_deep()            += b.w_bhig_deep();
    c.out_w_bhig_deep_proc()       += b.w_bhig_deep_proc();
    
    for (unsigned i(0); i<2; i++) {
      c.out_sys_bctag()[i]                   += b.sys_bctag()[i];
      c.out_sys_bctag_proc()[i]              += b.sys_bctag_proc()[i];
      c.out_sys_bctag_deep()[i]              += b.sys_bctag_deep()[i];
      c.out_sys_bctag_deep_proc()[i]         += b.sys_bctag_deep_proc()[i];
      c.out_sys_udsgtag()[i]                 += b.sys_udsgtag()[i];
      c.out_sys_udsgtag_proc()[i]            += b.sys_udsgtag_proc()[i];
      c.out_sys_udsgtag_deep()[i]            += b.sys_udsgtag_deep()[i];
      c.out_sys_udsgtag_deep_proc()[i]       += b.sys_udsgtag_deep_proc()[i];
        
      c.out_sys_bchig()[i]                   += b.sys_bchig()[i];
      c.out_sys_bchig_proc()[i]              += b.sys_bchig_proc()[i];
      c.out_sys_bchig_deep()[i]              += b.sys_bchig_deep()[i];
      c.out_sys_bchig_deep_proc()[i]         += b.sys_bchig_deep_proc()[i];
      c.out_sys_udsghig()[i]                 += b.sys_udsghig()[i];
      c.out_sys_udsghig_proc()[i]            += b.sys_udsghig_proc()[i];
      c.out_sys_udsghig_deep()[i]            += b.sys_udsghig_deep()[i];
      c.out_sys_udsghig_deep_proc()[i]       += b.sys_udsghig_deep_proc()[i];

      if (isSignal) { // yes, this ignores the fullsim points
        c.out_sys_mur()[i]                     += b.sys_mur()[i];
        c.out_sys_muf()[i]                     += b.sys_muf()[i];
        c.out_sys_murf()[i]                    += b.sys_murf()[i];

        c.out_sys_fs_bctag()[i]              += b.sys_fs_bctag()[i];
        c.out_sys_fs_bctag_deep()[i]         += b.sys_fs_bctag_deep()[i];
        c.out_sys_fs_udsgtag()[i]            += b.sys_fs_udsgtag()[i];
        c.out_sys_fs_udsgtag_deep()[i]       += b.sys_fs_udsgtag_deep()[i];
        c.out_sys_fs_bchig()[i]              += b.sys_fs_bchig()[i];
        c.out_sys_fs_bchig_deep()[i]         += b.sys_fs_bchig_deep()[i];
        c.out_sys_fs_udsghig()[i]            += b.sys_fs_udsghig()[i];
        c.out_sys_fs_udsghig_deep()[i]       += b.sys_fs_udsghig_deep()[i];
      }
    }

    if (!quick) {
      c.out_w_btag_loose()           += b.w_btag_loose();
      c.out_w_btag_loose_deep()      += b.w_btag_loose_deep();
      c.out_w_btag_tight()           += b.w_btag_tight();
      c.out_w_btag_tight_deep()      += b.w_btag_tight_deep();
      c.out_w_btag_loose_proc()      += b.w_btag_loose_proc();
      c.out_w_btag_loose_deep_proc() += b.w_btag_loose_deep_proc();
      c.out_w_btag_tight_proc()      += b.w_btag_tight_proc();
      c.out_w_btag_tight_deep_proc() += b.w_btag_tight_deep_proc();

      for (unsigned i(0); i<b.w_pdf().size(); i++) c.out_w_pdf()[i] += b.w_pdf()[i];

      for (unsigned i(0); i<b.sys_mur().size(); i++) {
        c.out_sys_pu()[i]                      += b.sys_pu()[i];
        c.out_sys_pdf()[i]                     += b.sys_pdf()[i];

        c.out_sys_bctag_loose()[i]             += b.sys_bctag_loose()[i];
        c.out_sys_bctag_loose_deep()[i]        += b.sys_bctag_loose_deep()[i];
        c.out_sys_udsgtag_loose()[i]           += b.sys_udsgtag_loose()[i];
        c.out_sys_udsgtag_loose_deep()[i]      += b.sys_udsgtag_loose_deep()[i];
        c.out_sys_bctag_tight()[i]             += b.sys_bctag_tight()[i];
        c.out_sys_bctag_tight_deep()[i]        += b.sys_bctag_tight_deep()[i];
        c.out_sys_udsgtag_tight()[i]           += b.sys_udsgtag_tight()[i];
        c.out_sys_udsgtag_tight_deep()[i]      += b.sys_udsgtag_tight_deep()[i];
        c.out_sys_bctag_loose_proc()[i]        += b.sys_bctag_loose_proc()[i];
        c.out_sys_bctag_loose_deep_proc()[i]   += b.sys_bctag_loose_deep_proc()[i];
        c.out_sys_udsgtag_loose_proc()[i]      += b.sys_udsgtag_loose_proc()[i];
        c.out_sys_udsgtag_loose_deep_proc()[i] += b.sys_udsgtag_loose_deep_proc()[i];
        c.out_sys_bctag_tight_proc()[i]        += b.sys_bctag_tight_proc()[i];
        c.out_sys_bctag_tight_deep_proc()[i]   += b.sys_bctag_tight_deep_proc()[i];
        c.out_sys_udsgtag_tight_proc()[i]      += b.sys_udsgtag_tight_proc()[i];
        c.out_sys_udsgtag_tight_deep_proc()[i] += b.sys_udsgtag_tight_deep_proc()[i];
      } // loop over 2 sys
    } // if quick
  } // loop over events

  c.out_neff() = neff;

  // Find absolute value of w_lumi
  //-------------------------------
  double xsec(0.); const float lumi = 1000.;
  if (Contains(tag, "SMS")){
    double exsec(0.);
    if(Contains(tag, "T1") || Contains(tag, "T5")) xsec::signalCrossSection(b.mgluino(), xsec, exsec);
    else if(Contains(tag, "TChiHH")) xsec::higgsinoCrossSection(b.mgluino(), xsec, exsec);
    else  xsec::stopCrossSection(b.mgluino(), xsec, exsec);
  } else {
    xsec = xsec::crossSection(tag);
  }
  c.out_w_lumi() = xsec*lumi/neff;
  
  // ISR weights correction is derived based on the mean weight found in 
  // SingleLeptFromT + SingleLeptFromTbar + DiLept sample 
  // (adding without any weight since it's not yet fixed)
  //-----------------------------------------------------------------------
  double corr_w_isr(1.);
  vector<double> corr_sys_isr(2,1.);
  double tot_w_isr = c.out_w_isr();
  if (Contains(tag,"TTJetst_HT") || Contains(tag,"genMET-150")) {
    // in this case take correction from inclusive since should not norm. to unity
    corr_w_isr = 1/1.013; 
    corr_sys_isr[0] = 1/1.065;
    corr_sys_isr[1] = 1/0.9612;
  } else{
    corr_w_isr = nent/c.out_w_isr();
    for (unsigned i(0); i<c.out_sys_isr().size(); i++) corr_sys_isr[i] = nent/c.out_sys_isr()[i];
  }
  c.out_w_isr() = corr_w_isr;
  for (unsigned i(0); i<c.out_sys_isr().size(); i++) c.out_sys_isr()[i] = corr_sys_isr[i];
  
  // Calculate correction to total weight whilst correcting zero lepton
  //----------------------------------------------------------------------
  double w_corr_l0 = (nent-c.out_w_lep())/nent_zlep * (nent-c.out_w_fs_lep())/nent_zlep;
  if(nent_zlep==0) w_corr_l0 = 1.;
  // again normalize to total w_isr, not unity
  c.out_weight() = (tot_w_isr*corr_w_isr)/(tot_weight_l0*w_corr_l0 + tot_weight_l1);

  // Lepton weights corrections to be applied only to 0-lep events
  //----------------------------------------------------------------
  c.out_w_lep()           = (nent-c.out_w_lep())/nent_zlep;
  c.out_w_fs_lep()        = (nent-c.out_w_fs_lep())/nent_zlep;
  for (unsigned i(0); i<c.out_sys_lep().size(); i++) 
    c.out_sys_lep()[i]    = (nent-c.out_sys_lep()[i])/nent_zlep;
  for (unsigned i(0); i<c.out_sys_fs_lep().size(); i++) 
    c.out_sys_fs_lep()[i] = (nent-c.out_sys_fs_lep()[i])/nent_zlep;

  //   Correction to cookie-cutter variables
  //------------------------------------------------
  c.out_w_pu()                   = nent / c.out_w_pu();

  c.out_w_btag()                 = nent / c.out_w_btag();
  c.out_w_btag_proc()            = nent / c.out_w_btag_proc();
  c.out_w_btag_deep()            = nent / c.out_w_btag_deep();
  c.out_w_btag_deep_proc()       = nent / c.out_w_btag_deep_proc();

  c.out_w_bhig()                 = nent / c.out_w_bhig();
  c.out_w_bhig_proc()            = nent / c.out_w_bhig_proc();
  c.out_w_bhig_deep()            = nent / c.out_w_bhig_deep();
  c.out_w_bhig_deep_proc()       = nent / c.out_w_bhig_deep_proc();
  
  for (unsigned i(0); i<2; i++) {
    c.out_sys_bctag()[i]                   = nent / c.out_sys_bctag()[i];
    c.out_sys_bctag_proc()[i]              = nent / c.out_sys_bctag_proc()[i];
    c.out_sys_bctag_deep()[i]              = nent / c.out_sys_bctag_deep()[i];
    c.out_sys_bctag_deep_proc()[i]         = nent / c.out_sys_bctag_deep_proc()[i];
    c.out_sys_udsgtag()[i]                 = nent / c.out_sys_udsgtag()[i];
    c.out_sys_udsgtag_proc()[i]            = nent / c.out_sys_udsgtag_proc()[i];
    c.out_sys_udsgtag_deep()[i]            = nent / c.out_sys_udsgtag_deep()[i];
    c.out_sys_udsgtag_deep_proc()[i]       = nent / c.out_sys_udsgtag_deep_proc()[i];
      
    c.out_sys_bchig()[i]                   = nent / c.out_sys_bchig()[i];
    c.out_sys_bchig_proc()[i]              = nent / c.out_sys_bchig_proc()[i];
    c.out_sys_bchig_deep()[i]              = nent / c.out_sys_bchig_deep()[i];
    c.out_sys_bchig_deep_proc()[i]         = nent / c.out_sys_bchig_deep_proc()[i];
    c.out_sys_udsghig()[i]                 = nent / c.out_sys_udsghig()[i];
    c.out_sys_udsghig_proc()[i]            = nent / c.out_sys_udsghig_proc()[i];
    c.out_sys_udsghig_deep()[i]            = nent / c.out_sys_udsghig_deep()[i];
    c.out_sys_udsghig_deep_proc()[i]       = nent / c.out_sys_udsghig_deep_proc()[i];

    if (isSignal) { // yes, this ignores the fullsim points
      c.out_sys_mur()[i]                     = nent / c.out_sys_mur()[i];
      c.out_sys_muf()[i]                     = nent / c.out_sys_muf()[i];
      c.out_sys_murf()[i]                    = nent / c.out_sys_murf()[i];

      c.out_sys_fs_bctag()[i]              = nent / c.out_sys_fs_bctag()[i];
      c.out_sys_fs_bctag_deep()[i]         = nent / c.out_sys_fs_bctag_deep()[i];
      c.out_sys_fs_udsgtag()[i]            = nent / c.out_sys_fs_udsgtag()[i];
      c.out_sys_fs_udsgtag_deep()[i]       = nent / c.out_sys_fs_udsgtag_deep()[i];
      c.out_sys_fs_bchig()[i]              = nent / c.out_sys_fs_bchig()[i];
      c.out_sys_fs_bchig_deep()[i]         = nent / c.out_sys_fs_bchig_deep()[i];
      c.out_sys_fs_udsghig()[i]            = nent / c.out_sys_fs_udsghig()[i];
      c.out_sys_fs_udsghig_deep()[i]       = nent / c.out_sys_fs_udsghig_deep()[i];
    }
  }

  if (!quick) {
    c.out_w_btag_loose()           = nent / c.out_w_btag_loose();
    c.out_w_btag_loose_deep()      = nent / c.out_w_btag_loose_deep();
    c.out_w_btag_tight()           = nent / c.out_w_btag_tight();
    c.out_w_btag_tight_deep()      = nent / c.out_w_btag_tight_deep();
    c.out_w_btag_loose_proc()      = nent / c.out_w_btag_loose_proc();
    c.out_w_btag_loose_deep_proc() = nent / c.out_w_btag_loose_deep_proc();
    c.out_w_btag_tight_proc()      = nent / c.out_w_btag_tight_proc();
    c.out_w_btag_tight_deep_proc() = nent / c.out_w_btag_tight_deep_proc();

    for (unsigned i(0); i<c.out_w_pdf().size(); i++) c.out_w_pdf()[i] = nent / c.out_w_pdf()[i];

    for (unsigned i(0); i<c.out_sys_mur().size(); i++) {
      c.out_sys_pu()[i]                      = nent / c.out_sys_pu()[i];
      c.out_sys_pdf()[i]                     = nent / c.out_sys_pdf()[i];

      c.out_sys_bctag_loose()[i]             = nent / c.out_sys_bctag_loose()[i];
      c.out_sys_bctag_loose_deep()[i]        = nent / c.out_sys_bctag_loose_deep()[i];
      c.out_sys_udsgtag_loose()[i]           = nent / c.out_sys_udsgtag_loose()[i];
      c.out_sys_udsgtag_loose_deep()[i]      = nent / c.out_sys_udsgtag_loose_deep()[i];
      c.out_sys_bctag_tight()[i]             = nent / c.out_sys_bctag_tight()[i];
      c.out_sys_bctag_tight_deep()[i]        = nent / c.out_sys_bctag_tight_deep()[i];
      c.out_sys_udsgtag_tight()[i]           = nent / c.out_sys_udsgtag_tight()[i];
      c.out_sys_udsgtag_tight_deep()[i]      = nent / c.out_sys_udsgtag_tight_deep()[i];
      c.out_sys_bctag_loose_proc()[i]        = nent / c.out_sys_bctag_loose_proc()[i];
      c.out_sys_bctag_loose_deep_proc()[i]   = nent / c.out_sys_bctag_loose_deep_proc()[i];
      c.out_sys_udsgtag_loose_proc()[i]      = nent / c.out_sys_udsgtag_loose_proc()[i];
      c.out_sys_udsgtag_loose_deep_proc()[i] = nent / c.out_sys_udsgtag_loose_deep_proc()[i];
      c.out_sys_bctag_tight_proc()[i]        = nent / c.out_sys_bctag_tight_proc()[i];
      c.out_sys_bctag_tight_deep_proc()[i]   = nent / c.out_sys_bctag_tight_deep_proc()[i];
      c.out_sys_udsgtag_tight_proc()[i]      = nent / c.out_sys_udsgtag_tight_proc()[i];
      c.out_sys_udsgtag_tight_deep_proc()[i] = nent / c.out_sys_udsgtag_tight_deep_proc()[i];
    }
  }

  c.Fill();
  c.Write();

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
      {"quick", no_argument, 0, 0},  
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