#include <iostream>
#include <ctime>
#include <getopt.h>

#include "baby_base.hpp"
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

  cout<<setprecision(20);
  bool doCorrOutputTree = true;
  string fin(tag), fout(tag);
  if (!Contains(tag,".root")) {
    fin += "*.root";
    fout +=".root";
  }
  cout<<"Chaining inputs: "<<indir+"/*"+fin<<endl;
  string outfile = outdir+"/corr_"+fout;
  if (quick) outfile = outdir+"/corrquick_"+fout;
  cout<<"Writing output to: "<<outfile<<endl;
  baby_base b(indir+"/*"+fin, outfile, doCorrOutputTree);

  // quantities to keep track of;
  int neff(0);
  double wgt(0), tot_weight_l0(0), tot_weight_l1(0), nent_zlep(0);

  long nent = b.GetEntries();
  cout<<"Running over "<<nent<<" events."<<endl;
  bool isSignal = false;
  for(long entry(0); entry<nent; entry++){
    if (entry==0 && b.type()>100e3) isSignal = true;

    b.GetEntry(entry);
    if (entry%10000==0 || entry == nent-1) {
      cout<<"Processing event: "<<entry<<endl;
      cout<<"b.out_w_pu() = "<<b.out_w_pu()<<"\t b.w_pu() = "<<b.w_pu()<<endl;
    }

    wgt = noInfNan(b.w_lep())*noInfNan(b.w_fs_lep())*
          noInfNan(b.w_btag())*noInfNan(b.w_isr());//*noInfNan(b.w_pu());

    // need special treatment in summing and/or renormalizing
    neff += b.w_lumi()>0 ? 1:-1;

    b.out_w_isr() += noInfNan(b.w_isr());
    for (unsigned i(0); i<b.sys_isr().size(); i++) b.out_sys_isr()[i] += noInfNan(b.sys_isr()[i]);


    if(b.nleps()==0) {
      nent_zlep += 1.;
      tot_weight_l0  += wgt;
    } else{
      tot_weight_l1         += wgt;
      b.out_w_lep()         += noInfNan(b.w_lep());
      b.out_w_fs_lep()      += noInfNan(b.w_fs_lep());
      for (unsigned i(0); i<b.sys_lep().size(); i++) b.out_sys_lep()[i] += noInfNan(b.sys_lep()[i]);
      for (unsigned i(0); i<b.sys_fs_lep().size(); i++) b.out_sys_fs_lep()[i] += noInfNan(b.sys_fs_lep()[i]);
    }
    
    //      Cookie-cutter variables
    //-----------------------------------
    b.out_w_pu()                   += noInfNan(b.w_pu());
    b.out_w_btag()                 += noInfNan(b.w_btag());

    for (unsigned i(0); i<b.sys_mur().size(); i++) b.out_sys_mur()[i] += noInfNan(b.sys_mur()[i]);

    if (!quick) {
      b.out_w_btag_deep()            += noInfNan(b.w_btag_deep());
      b.out_w_btag_loose()           += noInfNan(b.w_btag_loose());
      b.out_w_btag_loose_deep()      += noInfNan(b.w_btag_loose_deep());
      b.out_w_btag_tight()           += noInfNan(b.w_btag_tight());
      b.out_w_btag_tight_deep()      += noInfNan(b.w_btag_tight_deep());
      b.out_w_bhig()                 += noInfNan(b.w_bhig());
      b.out_w_bhig_deep()            += noInfNan(b.w_bhig_deep());
      b.out_w_btag_proc()            += noInfNan(b.w_btag_proc());
      b.out_w_btag_deep_proc()       += noInfNan(b.w_btag_deep_proc());
      b.out_w_btag_loose_proc()      += noInfNan(b.w_btag_loose_proc());
      b.out_w_btag_loose_deep_proc() += noInfNan(b.w_btag_loose_deep_proc());
      b.out_w_btag_tight_proc()      += noInfNan(b.w_btag_tight_proc());
      b.out_w_btag_tight_deep_proc() += noInfNan(b.w_btag_tight_deep_proc());
      b.out_w_bhig_proc()            += noInfNan(b.w_bhig_proc());
      b.out_w_bhig_deep_proc()       += noInfNan(b.w_bhig_deep_proc());

      for (unsigned i(0); i<b.w_pdf().size(); i++)  b.out_w_pdf()[i] += noInfNan(b.w_pdf()[i]);

      for (unsigned i(0); i<2; i++) {
        b.out_sys_mur()[i]                     += noInfNan(b.sys_mur()[i]);
        b.out_sys_muf()[i]                     += noInfNan(b.sys_muf()[i]);
        b.out_sys_murf()[i]                    += noInfNan(b.sys_murf()[i]);
        b.out_sys_pdf()[i]                     += noInfNan(b.sys_pdf()[i]);
        b.out_sys_bctag()[i]                   += noInfNan(b.sys_bctag()[i]);
        b.out_sys_bctag_deep()[i]              += noInfNan(b.sys_bctag_deep()[i]);
        b.out_sys_udsgtag()[i]                 += noInfNan(b.sys_udsgtag()[i]);
        b.out_sys_udsgtag_deep()[i]            += noInfNan(b.sys_udsgtag_deep()[i]);
        b.out_sys_bctag_loose()[i]             += noInfNan(b.sys_bctag_loose()[i]);
        b.out_sys_bctag_loose_deep()[i]        += noInfNan(b.sys_bctag_loose_deep()[i]);
        b.out_sys_udsgtag_loose()[i]           += noInfNan(b.sys_udsgtag_loose()[i]);
        b.out_sys_udsgtag_loose_deep()[i]      += noInfNan(b.sys_udsgtag_loose_deep()[i]);
        b.out_sys_bctag_tight()[i]             += noInfNan(b.sys_bctag_tight()[i]);
        b.out_sys_bctag_tight_deep()[i]        += noInfNan(b.sys_bctag_tight_deep()[i]);
        b.out_sys_udsgtag_tight()[i]           += noInfNan(b.sys_udsgtag_tight()[i]);
        b.out_sys_udsgtag_tight_deep()[i]      += noInfNan(b.sys_udsgtag_tight_deep()[i]);
        b.out_sys_bchig()[i]                   += noInfNan(b.sys_bchig()[i]);
        b.out_sys_bchig_deep()[i]              += noInfNan(b.sys_bchig_deep()[i]);
        b.out_sys_udsghig()[i]                 += noInfNan(b.sys_udsghig()[i]);
        b.out_sys_udsghig_deep()[i]            += noInfNan(b.sys_udsghig_deep()[i]);
        b.out_sys_bctag_proc()[i]              += noInfNan(b.sys_bctag_proc()[i]);
        b.out_sys_bctag_deep_proc()[i]         += noInfNan(b.sys_bctag_deep_proc()[i]);
        b.out_sys_udsgtag_proc()[i]            += noInfNan(b.sys_udsgtag_proc()[i]);
        b.out_sys_udsgtag_deep_proc()[i]       += noInfNan(b.sys_udsgtag_deep_proc()[i]);
        b.out_sys_bctag_loose_proc()[i]        += noInfNan(b.sys_bctag_loose_proc()[i]);
        b.out_sys_bctag_loose_deep_proc()[i]   += noInfNan(b.sys_bctag_loose_deep_proc()[i]);
        b.out_sys_udsgtag_loose_proc()[i]      += noInfNan(b.sys_udsgtag_loose_proc()[i]);
        b.out_sys_udsgtag_loose_deep_proc()[i] += noInfNan(b.sys_udsgtag_loose_deep_proc()[i]);
        b.out_sys_bctag_tight_proc()[i]        += noInfNan(b.sys_bctag_tight_proc()[i]);
        b.out_sys_bctag_tight_deep_proc()[i]   += noInfNan(b.sys_bctag_tight_deep_proc()[i]);
        b.out_sys_udsgtag_tight_proc()[i]      += noInfNan(b.sys_udsgtag_tight_proc()[i]);
        b.out_sys_udsgtag_tight_deep_proc()[i] += noInfNan(b.sys_udsgtag_tight_deep_proc()[i]);
        b.out_sys_bchig_proc()[i]              += noInfNan(b.sys_bchig_proc()[i]);
        b.out_sys_bchig_deep_proc()[i]         += noInfNan(b.sys_bchig_deep_proc()[i]);
        b.out_sys_udsghig_proc()[i]            += noInfNan(b.sys_udsghig_proc()[i]);
        b.out_sys_udsghig_deep_proc()[i]       += noInfNan(b.sys_udsghig_deep_proc()[i]);
        b.out_sys_pu()[i]                      += noInfNan(b.sys_pu()[i]);
        if (isSignal) { // yes, this ignores the fullsim points
          b.out_sys_fs_bctag()[i]              += noInfNan(b.sys_fs_bctag()[i]);
          b.out_sys_fs_bctag_deep()[i]         += noInfNan(b.sys_fs_bctag_deep()[i]);
          b.out_sys_fs_udsgtag()[i]            += noInfNan(b.sys_fs_udsgtag()[i]);
          b.out_sys_fs_udsgtag_deep()[i]       += noInfNan(b.sys_fs_udsgtag_deep()[i]);
          b.out_sys_fs_bchig()[i]              += noInfNan(b.sys_fs_bchig()[i]);
          b.out_sys_fs_bchig_deep()[i]         += noInfNan(b.sys_fs_bchig_deep()[i]);
          b.out_sys_fs_udsghig()[i]            += noInfNan(b.sys_fs_udsghig()[i]);
          b.out_sys_fs_udsghig_deep()[i]       += noInfNan(b.sys_fs_udsghig_deep()[i]);
        }
      }
    }
  } // loop over events

  b.out_nevents() = nent;

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
  b.out_w_lumi() = xsec*lumi/neff;
  
  // ISR weights correction is derived based on the mean weight found in 
  // SingleLeptFromT + SingleLeptFromTbar + DiLept sample 
  // (adding without any weight since it's not yet fixed)
  //-----------------------------------------------------------------------
  double corr_w_isr(1.);
  vector<double> corr_sys_isr(2,1.);
  double tot_w_isr = b.val_w_isr();
  if (Contains(tag,"TTJetst_HT") || Contains(tag,"genMET-150")) {
    // in this case take correction from inclusive since should not norm. to unity
    corr_w_isr = 1/1.013; 
    corr_sys_isr[0] = 1/1.065;
    corr_sys_isr[1] = 1/0.9612;
  } else{
    corr_w_isr = nent/b.val_w_isr();
    for (unsigned i(0); i<b.sys_isr().size(); i++) corr_sys_isr[i] = nent/b.val_sys_isr()[i];
  }
  b.out_w_isr() = corr_w_isr;
  for (unsigned i(0); i<b.sys_isr().size(); i++) b.out_sys_isr()[i] = corr_sys_isr[i];
  
  // Calculate correction to total weight whilst correcting zero lepton
  //----------------------------------------------------------------------
  double w_corr_l0 = (nent-b.val_w_lep())/nent_zlep * (nent-b.val_w_fs_lep())/nent_zlep;
  if(nent_zlep==0) w_corr_l0 = 1.;
  // again normalize to total w_isr, not unity
  b.out_weight() = (tot_w_isr*corr_w_isr)/(tot_weight_l0*w_corr_l0 + tot_weight_l1);

  // Lepton weights corrections to be applied only to 0-lep events
  //----------------------------------------------------------------
  b.out_w_lep()           = (nent-b.val_w_lep())/nent_zlep;
  b.out_w_fs_lep()        = (nent-b.val_w_fs_lep())/nent_zlep;
  for (unsigned i(0); i<b.sys_lep().size(); i++) 
    b.out_sys_lep()[i]    = (nent-b.val_sys_lep()[i])/nent_zlep;
  for (unsigned i(0); i<b.sys_fs_lep().size(); i++) 
    b.out_sys_fs_lep()[i] = (nent-b.val_sys_fs_lep()[i])/nent_zlep;

  //   Correction to cookie-cutter variables
  //------------------------------------------------
  cout<<"Before final calc, val wpu = "<<b.val_w_pu()<<" out_w_pu = "<<b.out_w_pu()<<endl;
  b.out_w_pu()                   = nent / b.val_w_pu();
  cout<<"After final calc, val wpu = "<<b.val_w_pu()<<" out_w_pu = "<<b.out_w_pu()<<endl;
  b.out_w_btag()                 = nent / b.val_w_btag();
  
  for (unsigned i(0); i<2; i++) b.out_sys_mur()[i]                     = nent / b.val_sys_mur()[i];

  if (!quick) {
    b.out_w_btag_deep()            = nent / b.val_w_btag_deep();
    b.out_w_btag_loose()           = nent / b.val_w_btag_loose();
    b.out_w_btag_loose_deep()      = nent / b.val_w_btag_loose_deep();
    b.out_w_btag_tight()           = nent / b.val_w_btag_tight();
    b.out_w_btag_tight_deep()      = nent / b.val_w_btag_tight_deep();
    b.out_w_bhig()                 = nent / b.val_w_bhig();
    b.out_w_bhig_deep()            = nent / b.val_w_bhig_deep();
    b.out_w_btag_proc()            = nent / b.val_w_btag_proc();
    b.out_w_btag_deep_proc()       = nent / b.val_w_btag_deep_proc();
    b.out_w_btag_loose_proc()      = nent / b.val_w_btag_loose_proc();
    b.out_w_btag_loose_deep_proc() = nent / b.val_w_btag_loose_deep_proc();
    b.out_w_btag_tight_proc()      = nent / b.val_w_btag_tight_proc();
    b.out_w_btag_tight_deep_proc() = nent / b.val_w_btag_tight_deep_proc();
    b.out_w_bhig_proc()            = nent / b.val_w_bhig_proc();
    b.out_w_bhig_deep_proc()       = nent / b.val_w_bhig_deep_proc();

    for (unsigned i(0); i<b.w_pdf().size(); i++) b.out_w_pdf()[i] = nent / b.val_w_pdf()[i];

    for (unsigned i(0); i<b.sys_mur().size(); i++) {
      b.out_sys_mur()[i]                     = nent / b.val_sys_mur()[i];
      b.out_sys_muf()[i]                     = nent / b.val_sys_muf()[i];
      b.out_sys_murf()[i]                    = nent / b.val_sys_murf()[i];
      b.out_sys_pdf()[i]                     = nent / b.val_sys_pdf()[i];
      b.out_sys_bctag()[i]                   = nent / b.val_sys_bctag()[i];
      b.out_sys_bctag_deep()[i]              = nent / b.val_sys_bctag_deep()[i];
      b.out_sys_udsgtag()[i]                 = nent / b.val_sys_udsgtag()[i];
      b.out_sys_udsgtag_deep()[i]            = nent / b.val_sys_udsgtag_deep()[i];
      b.out_sys_bctag_loose()[i]             = nent / b.val_sys_bctag_loose()[i];
      b.out_sys_bctag_loose_deep()[i]        = nent / b.val_sys_bctag_loose_deep()[i];
      b.out_sys_udsgtag_loose()[i]           = nent / b.val_sys_udsgtag_loose()[i];
      b.out_sys_udsgtag_loose_deep()[i]      = nent / b.val_sys_udsgtag_loose_deep()[i];
      b.out_sys_bctag_tight()[i]             = nent / b.val_sys_bctag_tight()[i];
      b.out_sys_bctag_tight_deep()[i]        = nent / b.val_sys_bctag_tight_deep()[i];
      b.out_sys_udsgtag_tight()[i]           = nent / b.val_sys_udsgtag_tight()[i];
      b.out_sys_udsgtag_tight_deep()[i]      = nent / b.val_sys_udsgtag_tight_deep()[i];
      b.out_sys_bchig()[i]                   = nent / b.val_sys_bchig()[i];
      b.out_sys_bchig_deep()[i]              = nent / b.val_sys_bchig_deep()[i];
      b.out_sys_udsghig()[i]                 = nent / b.val_sys_udsghig()[i];
      b.out_sys_udsghig_deep()[i]            = nent / b.val_sys_udsghig_deep()[i];
      b.out_sys_bctag_proc()[i]              = nent / b.val_sys_bctag_proc()[i];
      b.out_sys_bctag_deep_proc()[i]         = nent / b.val_sys_bctag_deep_proc()[i];
      b.out_sys_udsgtag_proc()[i]            = nent / b.val_sys_udsgtag_proc()[i];
      b.out_sys_udsgtag_deep_proc()[i]       = nent / b.val_sys_udsgtag_deep_proc()[i];
      b.out_sys_bctag_loose_proc()[i]        = nent / b.val_sys_bctag_loose_proc()[i];
      b.out_sys_bctag_loose_deep_proc()[i]   = nent / b.val_sys_bctag_loose_deep_proc()[i];
      b.out_sys_udsgtag_loose_proc()[i]      = nent / b.val_sys_udsgtag_loose_proc()[i];
      b.out_sys_udsgtag_loose_deep_proc()[i] = nent / b.val_sys_udsgtag_loose_deep_proc()[i];
      b.out_sys_bctag_tight_proc()[i]        = nent / b.val_sys_bctag_tight_proc()[i];
      b.out_sys_bctag_tight_deep_proc()[i]   = nent / b.val_sys_bctag_tight_deep_proc()[i];
      b.out_sys_udsgtag_tight_proc()[i]      = nent / b.val_sys_udsgtag_tight_proc()[i];
      b.out_sys_udsgtag_tight_deep_proc()[i] = nent / b.val_sys_udsgtag_tight_deep_proc()[i];
      b.out_sys_bchig_proc()[i]              = nent / b.val_sys_bchig_proc()[i];
      b.out_sys_bchig_deep_proc()[i]         = nent / b.val_sys_bchig_deep_proc()[i];
      b.out_sys_udsghig_proc()[i]            = nent / b.val_sys_udsghig_proc()[i];
      b.out_sys_udsghig_deep_proc()[i]       = nent / b.val_sys_udsghig_deep_proc()[i];
      b.out_sys_pu()[i]                      = nent / b.val_sys_pu()[i];
      if (isSignal) { // yes, this ignores the fullsim points
        b.out_sys_fs_bctag()[i]              = nent / b.val_sys_fs_bctag()[i];
        b.out_sys_fs_bctag_deep()[i]         = nent / b.val_sys_fs_bctag_deep()[i];
        b.out_sys_fs_udsgtag()[i]            = nent / b.val_sys_fs_udsgtag()[i];
        b.out_sys_fs_udsgtag_deep()[i]       = nent / b.val_sys_fs_udsgtag_deep()[i];
        b.out_sys_fs_bchig()[i]              = nent / b.val_sys_fs_bchig()[i];
        b.out_sys_fs_bchig_deep()[i]         = nent / b.val_sys_fs_bchig_deep()[i];
        b.out_sys_fs_udsghig()[i]            = nent / b.val_sys_fs_udsghig()[i];
        b.out_sys_fs_udsghig_deep()[i]       = nent / b.val_sys_fs_udsghig_deep()[i];
      }
    }
  }

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