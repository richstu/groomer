// Lists the MC cross sections

#include <iostream>
#include "cross_sections.hpp"

using namespace std;

namespace xsec{

  float crossSection(const TString &file, bool is2016){
    float xsec(999999.), Htobb(0.5824);

    if (is2016) {
        if(file.Contains("SMS-T1tttt_mGluino-1200_mLSP-800_Tune")) xsec = 0.0985;
        if(file.Contains("SMS-T1tttt_mGluino-2000_mLSP-100_Tune")) xsec = 0.00101;
        
        if(file.Contains("RPV") && file.Contains("1000"))  xsec = 0.325388;
        if(file.Contains("RPV") && file.Contains("1100"))  xsec = 0.163491;
        if(file.Contains("RPV") && file.Contains("1200"))  xsec = 0.0856418;
        if(file.Contains("RPV") && file.Contains("1300"))  xsec = 0.0460525;
        if(file.Contains("RPV") && file.Contains("1400"))  xsec = 0.0252977;


        //  Cross-section taken from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/TtbarNNLO
        // Alternative option: https://twiki.cern.ch/twiki/bin/view/Sandbox/FullNNLOcrossSections#Top_cross_section_for_13_TeV
        if(file.Contains("TTJets_Tune") || file.Contains("TT_"))  xsec = 815.96;
        if(file.Contains("TTJets_HT")){//LO cross sections with k-factor of 1.625 already applied
          if(file.Contains("2500toInf")) xsec = 0.0023234211;
          if(file.Contains("1200to2500")) xsec = 0.194972521;
          if(file.Contains("800to1200")) xsec = 1.07722318;
          if(file.Contains("600to800")) xsec = 2.61537118;

        }
        // The efficiency of the mtt>1000 cut is taken from sigma(mtt>1000)/sigma(inclusive) from mcm
        double mtt_1000_eff=(11.16/670.3);
        if(file.Contains("TTJets_Mtt-1000toInf")) xsec = 815.96*mtt_1000_eff;

        if(file.Contains("TTJets_DiLept") || file.Contains("TTTo2L2Nu")) xsec = 85.66; // (3*0.108)^2*815.96
        if(file.Contains("TTJets_SingleLept")) xsec = 178.7; //(1- ((1-3*0.108)^2+(3*0.108)^2))*815.96*0.5 per half
        if(file.Contains("TTToSemiLeptonic")) xsec = 357.4;
        
        if(file.Contains("TTJets_DiLept_genMET-150")) xsec = 0.06392*85.66; // filter_eff*(3*0.108)^2*815.96
        if(file.Contains("TTJets_SingleLept") && file.Contains("genMET-150")) xsec = 0.05217*178.7; //filter_eff*(1- ((1-3*0.108)^2+(3*0.108)^2))*815.96*0.5 per half

        // cross sections from mcm
        if(file.Contains("TTG")) xsec = 3.697;                
        if(file.Contains("TTTT_Tune")) xsec = 0.009103;
        // mcm cross section times the same kfactors used for leptonic samples
        if(file.Contains("WJetsToQQ_HT-600ToInf")) xsec = 95.14*1.21;
        if(file.Contains("ZJetsToQQ_HT600toInf")) xsec = 5.67*1.23;
        
        // From https://cms-pdmv.cern.ch/mcm
        // k-factors from https://mangano.web.cern.ch/mangano/public/MECCA/samples_50ns_miniaod.txt
        // k-factors are ratio of https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat13TeV
        // NLO/NNLO cross-sections to that of an inclusive sample in mcm at lower order (LO/NLO)

        if(file.Contains("WJetsToLNu_Tune")) xsec=61526.7; //NNLO from Lesya's summary table 

        //cross-section per slice changed due to change in genHT definition
        if(file.Contains("WJetsToLNu_HT-70To100"))  xsec = 1372.*1.21; 
        if(file.Contains("WJetsToLNu_HT-100To200"))  xsec = 1347.*1.21; 
        if(file.Contains("WJetsToLNu_HT-200To400"))  xsec = 360.*1.21;
        if(file.Contains("WJetsToLNu_HT-400To600"))  xsec = 48.98*1.21;
        if(file.Contains("WJetsToLNu_HT-600ToInf"))  xsec = 18.77*1.21;
        if(file.Contains("WJetsToLNu_HT-600To800"))  xsec = 12.05*1.21;
        if(file.Contains("WJetsToLNu_HT-800To1200"))  xsec = 5.501*1.21;
        if(file.Contains("WJetsToLNu_HT-1200To2500"))  xsec = 1.329*1.21;
        if(file.Contains("WJetsToLNu_HT-2500ToInf"))  xsec = 0.03216*1.21;

        //updated 02-2019 with XSDB
        if(file.Contains("QCD_HT100to200_Tune")) xsec = 28060000;
        if(file.Contains("QCD_HT200to300_Tune"))   xsec = 1710000;
        if(file.Contains("QCD_HT300to500_Tune"))   xsec = 347500;
        if(file.Contains("QCD_HT500to700_Tune"))   xsec = 32060;
        if(file.Contains("QCD_HT700to1000_Tune"))  xsec = 6829;
        if(file.Contains("QCD_HT1000to1500_Tune")) xsec = 1207;
        if(file.Contains("QCD_HT1500to2000_Tune")) xsec = 120.0;
        if(file.Contains("QCD_HT2000toInf_Tune"))  xsec = 25.25;

        // Cross sections from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SingleTopRefXsec
        // multiplied by BF(W->mu,e,tau) = 0.324
        if (file.Contains("ST_s-channel_4f_leptonDecays"))     xsec = 3.34;
        if (file.Contains("ST_t-channel_antitop_4f_inclusiveDecays")) xsec = 80.95;
        if (file.Contains("ST_t-channel_top_4f_inclusiveDecays")) xsec = 136.02;
        if (file.Contains("ST_tW_antitop_5f_NoFullyHadronicDecays"))     xsec = 35.85*0.543;
        if (file.Contains("ST_tW_top_5f_NoFullyHadronicDecays"))     xsec = 35.85*0.543; 

        if(file.Contains("DYJetsToLL_M-10to50_Tune")) xsec = 18610*1.23;
        if(file.Contains("DYJetsToLL_M-50_Tune"))     xsec = 4895*1.23;

        if(file.Contains("DYJetsToLL_M-50_HT-70to100"))    xsec = 175.3*1.23;
        if(file.Contains("DYJetsToLL_M-50_HT-100to200"))    xsec = 139.4*1.23;
        if(file.Contains("DYJetsToLL_M-50_HT-200to400"))    xsec = 42.75*1.23;
        if(file.Contains("DYJetsToLL_M-50_HT-400to600"))    xsec = 5.497*1.23;
        if(file.Contains("DYJetsToLL_M-50_HT-600to800"))    xsec = 1.363*1.23;
        if(file.Contains("DYJetsToLL_M-50_HT-800to1200"))    xsec = 0.6759*1.23;
        if(file.Contains("DYJetsToLL_M-50_HT-1200to2500"))    xsec = 0.116*1.23;
        if(file.Contains("DYJetsToLL_M-50_HT-2500toInf"))    xsec = 0.002592*1.23;
        if(file.Contains("DYJetsToLL_M-50_HT-600toInf"))    xsec = 2.21*1.23;

        if(file.Contains("ZJetsToNuNu_HT-100To200"))  xsec = 280.35*1.27;
        if(file.Contains("ZJetsToNuNu_HT-200To400"))  xsec = 77.67*1.27;
        if(file.Contains("ZJetsToNuNu_HT-400To600"))  xsec = 10.73*1.27;
        if(file.Contains("ZJetsToNuNu_HT-600To800"))  xsec = 2.536*1.27;
        if(file.Contains("ZJetsToNuNu_HT-800To1200"))  xsec = 1.161*1.27;
        if(file.Contains("ZJetsToNuNu_HT-1200To2500"))  xsec = 0.2824*1.27;
        if(file.Contains("ZJetsToNuNu_HT-2500ToInf"))  xsec = 0.006459*1.27;
        if(file.Contains("ZJetsToNuNu_HT-600ToInf"))  xsec = 3.986*1.27;

        if(file.Contains("TTZToQQ"))                xsec = 0.5297;
        if(file.Contains("TTZToLLNuNu_M-10"))       xsec = 0.2529;
        if(file.Contains("TTWJetsToQQ"))            xsec = 0.4062;
        if(file.Contains("TTWJetsToLNu"))           xsec = 0.2043;
       
        //https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#Diboson
        if(file.Contains("WWTo2L2Nu"))   xsec = 12.178; //NNLO
        if(file.Contains("WWToLNuQQ"))   xsec = 49.997; //NNLO
        if(file.Contains("ttHTobb_M125"))   xsec = 0.2934;

        if(file.Contains("WZTo1L3Nu"))   xsec = 3.05;
        if(file.Contains("WZTo1L1Nu2Q"))   xsec = 10.96;
        if(file.Contains("WZTo2L2Q"))   xsec = 5.595;
        if(file.Contains("WZTo3LNu"))   xsec = 4.42965;
        if(file.Contains("VVTo2L2Nu"))   xsec = 11.95;
        if(file.Contains("ZZ_Tune"))   xsec = 16.523;

        // Calculated at 13 TeV in
        // https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageAt1314TeV
        // Higgs branching ratios from
        // https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageBR
        if(file.Contains("ZH_HToBB_ZToLL_M-125"))      xsec = 0.883*Htobb*0.033658;
        if(file.Contains("ZH_HToBB_ZToNuNu_M-125"))    xsec = 0.883*Htobb*0.2;
        if(file.Contains("WH_HToBB_WToLNu_M-125"))     xsec = 1.373*Htobb*(0.1071+0.1063+0.1138);
        if(file.Contains("ZH_HToBB_ZToNuNu_M125"))    xsec = 0.883*Htobb*0.2;
        if(file.Contains("WH_HToBB_WToLNu_M125"))     xsec = 1.373*Htobb*(0.1071+0.1063+0.1138);
    } else {
        if(file.Contains("SMS-T1tttt_mGluino-1200_mLSP-800_Tune")) xsec = 0.0985;
        if(file.Contains("SMS-T1tttt_mGluino-2000_mLSP-100_Tune")) xsec = 0.00101;

        //  Cross-section taken from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/TtbarNNLO
        // Alternative option: https://twiki.cern.ch/twiki/bin/view/Sandbox/FullNNLOcrossSections#Top_cross_section_for_13_TeV
        if(file.Contains("TTJets_Tune") || file.Contains("TT_"))  xsec = 815.96;

        if(file.Contains("TTJets_DiLept") || file.Contains("TTTo2L2Nu")) xsec = 85.66; // (3*0.108)^2*815.96
        if(file.Contains("TTJets_SingleLept")) xsec = 178.7; //(1- ((1-3*0.108)^2+(3*0.108)^2))*815.96*0.5 per half
        
        if(file.Contains("TTJets_DiLept_genMET-150")) xsec = 0.0676543*85.66; // filter_eff*(3*0.108)^2*815.96
        if(file.Contains("TTJets_SingleLept") && file.Contains("genMET-150")) xsec = 0.0568246*178.7; //filter_eff*(1- ((1-3*0.108)^2+(3*0.108)^2))*815.96*0.5 per half

        if(file.Contains("TTJets_DiLept_genMET-80")) xsec = 0.412882*85.66; // filter_eff*(3*0.108)^2*815.96
        if(file.Contains("TTJets_SingleLept") && file.Contains("genMET-80")) xsec = 0.293137*178.7; //filter_eff*(1- ((1-3*0.108)^2+(3*0.108)^2))*815.96*0.5 per half

        // from cross XSDB
        if(file.Contains("TTG")) xsec = 4.078;                
        if(file.Contains("TTTT_Tune")) xsec = 0.008213;
        
        // From https://cms-pdmv.cern.ch/mcm
        // k-factors from https://mangano.web.cern.ch/mangano/public/MECCA/samples_50ns_miniaod.txt
        // k-factors are ratio of https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSectionsat13TeV
        // NLO/NNLO cross-sections to that of an inclusive sample in mcm at lower order (LO/NLO)

        if(file.Contains("WJetsToLNu_Tune")) xsec=20508.9*3; //NNLO from Lesya's summary table 

        //cross-section per slice based on inclusive sample, roughly 10% higher than 2016, less in extreme tail
        if(file.Contains("WJetsToLNu_HT-100To200"))  xsec = 0.0262096*20508.9*3;
        if(file.Contains("WJetsToLNu_HT-200To400"))  xsec = 0.00772818*20508.9*3;
        if(file.Contains("WJetsToLNu_HT-400To600"))  xsec = 0.00109366*20508.9*3;
        if(file.Contains("WJetsToLNu_HT-600To800"))  xsec = 0.000272388*20508.9*3;
        if(file.Contains("WJetsToLNu_HT-800To1200"))  xsec = 0.000122233*20508.9*3;
        if(file.Contains("WJetsToLNu_HT-1200To2500"))  xsec = 2.71060e-5*20508.9*3;
        if(file.Contains("WJetsToLNu_HT-2500ToInf"))  xsec = 3.94174e-07*20508.9*3;

        if(file.Contains("QCD_HT100to200_Tune")) xsec = 23700000;
        if(file.Contains("QCD_HT200to300_Tune"))   xsec = 1547000;
        if(file.Contains("QCD_HT300to500_Tune"))   xsec = 322600;
        if(file.Contains("QCD_HT500to700_Tune"))   xsec = 29980;
        if(file.Contains("QCD_HT700to1000_Tune"))  xsec = 6334;
        if(file.Contains("QCD_HT1000to1500_Tune")) xsec = 1088;
        if(file.Contains("QCD_HT1500to2000_Tune")) xsec = 99.11;
        if(file.Contains("QCD_HT2000toInf_Tune"))  xsec = 20.23;

        // Cross sections from https://twiki.cern.ch/twiki/bin/view/LHCPhysics/SingleTopRefXsec
        // multiplied by BF(W->mu,e,tau) = 0.324
        if (file.Contains("ST_s-channel_4f_leptonDecays"))     xsec = 3.34;
        if (file.Contains("ST_t-channel_antitop_4f_inclusiveDecays") ||
            file.Contains("ST_t-channel_antitop_4f_InclusiveDecays")) xsec = 80.95;
        if (file.Contains("ST_t-channel_top_4f_inclusiveDecays") || 
            file.Contains("ST_t-channel_top_4f_InclusiveDecays")) xsec = 136.02;
        if (file.Contains("ST_tW_antitop_5f_NoFullyHadronicDecays"))     xsec = 35.85*0.543;
        if (file.Contains("ST_tW_top_5f_NoFullyHadronicDecays"))     xsec = 35.85*0.543; 

        if(file.Contains("DYJetsToLL_M-50_Tune"))     xsec = 2075.14*3;

        if(file.Contains("DYJetsToLL_M-50_HT-100to200"))    xsec = 0.0302083*2075.14*3;
        if(file.Contains("DYJetsToLL_M-50_HT-200to400"))    xsec = 0.00907651*2075.14*3;
        if(file.Contains("DYJetsToLL_M-50_HT-400to600"))    xsec = 0.00129238*2075.14*3;
        if(file.Contains("DYJetsToLL_M-50_HT-600to800"))    xsec = 0.000316039*2075.14*3;
        if(file.Contains("DYJetsToLL_M-50_HT-800to1200"))    xsec = 0.000137432*2075.14*3;
        if(file.Contains("DYJetsToLL_M-50_HT-1200to2500"))    xsec = 3.09368e-05*2075.14*3;
        if(file.Contains("DYJetsToLL_M-50_HT-2500toInf"))    xsec = 4.39860e-07*2075.14*3;

        // k-factor from DYJets 1.165
        if(file.Contains("ZJetsToNuNu_HT-100To200"))  xsec = 302.8*1.165;
        if(file.Contains("ZJetsToNuNu_HT-200To400"))  xsec = 92.59*1.165;
        if(file.Contains("ZJetsToNuNu_HT-400To600"))  xsec = 13.18*1.165;
        if(file.Contains("ZJetsToNuNu_HT-600To800"))  xsec = 3.257*1.165;
        if(file.Contains("ZJetsToNuNu_HT-800To1200"))  xsec = 1.49*1.165;
        if(file.Contains("ZJetsToNuNu_HT-1200To2500"))  xsec = 0.3419*1.165;
        if(file.Contains("ZJetsToNuNu_HT-2500ToInf"))  xsec = 0.005146*1.165;

        if(file.Contains("TTZToQQ"))                xsec = 0.5104;
        if(file.Contains("TTZToLLNuNu_M-10"))       xsec = 0.2432;
        if(file.Contains("TTWJetsToQQ"))            xsec = 0.4316;
        if(file.Contains("TTWJetsToLNu"))           xsec = 0.2149;
       
        // https://twiki.cern.ch/twiki/bin/viewauth/CMS/SummaryTable1G25ns#Diboson
        if(file.Contains("WWTo2L2Nu"))   xsec = 12.178; //NNLO
        if(file.Contains("WWToLNuQQ"))   xsec = 49.997; //NNLO
        if(file.Contains("ttHTobb_M125"))   xsec = 0.2934;

        if(file.Contains("WZTo1L3Nu"))   xsec = 3.05;
        if(file.Contains("WZTo1L1Nu2Q"))   xsec = 10.73;
        if(file.Contains("WZTo2L2Q"))   xsec = 5.606;
        if(file.Contains("WZTo3LNu"))   xsec = 4.42965;
        if(file.Contains("VVTo2L2Nu"))   xsec = 11.95;
        if(file.Contains("ZZ_Tune"))   xsec = 16.523; //from twiki NLO

        // Calculated at 13 TeV in
        // https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageAt1314TeV
        // Higgs branching ratios from
        // https://twiki.cern.ch/twiki/bin/view/LHCPhysics/CERNYellowReportPageBR
        if(file.Contains("ZH_HToBB_ZToLL_M-125"))      xsec = 0.883*Htobb*0.033658;
        if(file.Contains("ZH_HToBB_ZToNuNu_M-125"))    xsec = 0.883*Htobb*0.2;
        if(file.Contains("WH_HToBB_WToLNu_M-125"))     xsec = 1.373*Htobb*(0.1071+0.1063+0.1138);
        if(file.Contains("ZH_HToBB_ZToNuNu_M125"))    xsec = 0.883*Htobb*0.2;
        if(file.Contains("WH_HToBB_WToLNu_M125"))     xsec = 1.373*Htobb*(0.1071+0.1063+0.1138);

    }
    if(xsec<=0) std::cout<<"BABYMAKER: Cross section not found for "<<file<<std::endl;

    return xsec;
  }

  float fractionNegWeights(const TString &file){
    float fneg(0.);

    // ttH, ttZ, ttW, ttGamma
    if(file.Contains("ttHJetTobb_M125_13TeV_amcatnloFXFX"))     fneg = 0.3515;
    if(file.Contains("TTZToQQ"))                                fneg = 0.2657;
    if(file.Contains("TTZToLLNuNu_M-10"))                       fneg = 0.2676;
    if(file.Contains("TTWJetsToQQ"))                            fneg = 0.2412;
    if(file.Contains("TTWJetsToLNu"))                           fneg = 0.2433;
    if(file.Contains("TTG"))                                    fneg = 0.342; // from MCM

    if(file.Contains("TTTT_TuneCUETP8M1_13TeV-amcatnlo"))       fneg = 0.41; // from MCM
    if(file.Contains("VVTo2L2Nu_13TeV_amcatnloFXFX"))       fneg = 0.20; // from MCM
    if(file.Contains("TTJets_Mtt-1000toInf"))                   fneg = 0.376996;

    // Single top
    if (file.Contains("ST_s-channel_4f_leptonDecays_13TeV-amcatnlo-pythia8")) fneg = 0.1884;

    return fneg;
  }


  void signalCrossSection(int glu_mass, double &xsec, double &xsec_unc){
    if      (glu_mass == 595  ) { xsec = 0.       ; xsec_unc = 0.       ; return; } // we shouldn't have these points
    else if (glu_mass == 600. ) { xsec = 0.113E+02; xsec_unc = 8.62 /100; return; }
    else if (glu_mass == 605. ) { xsec = 0.108E+02; xsec_unc = 8.66 /100; return; }
    else if (glu_mass == 610. ) { xsec = 0.102E+02; xsec_unc = 8.7 /100; return; }
    else if (glu_mass == 615. ) { xsec = 0.974E+01; xsec_unc = 8.74 /100; return; }
    else if (glu_mass == 620. ) { xsec = 0.926E+01; xsec_unc = 8.78 /100; return; }
    else if (glu_mass == 625. ) { xsec = 0.881E+01; xsec_unc = 8.82 /100; return; }
    else if (glu_mass == 630. ) { xsec = 0.839E+01; xsec_unc = 8.86 /100; return; }
    else if (glu_mass == 635. ) { xsec = 0.799E+01; xsec_unc = 8.9 /100; return; }
    else if (glu_mass == 640. ) { xsec = 0.761E+01; xsec_unc = 8.94 /100; return; }
    else if (glu_mass == 645. ) { xsec = 0.725E+01; xsec_unc = 8.98 /100; return; }
    else if (glu_mass == 650. ) { xsec = 0.690E+01; xsec_unc = 9.02 /100; return; }
    else if (glu_mass == 655. ) { xsec = 0.658E+01; xsec_unc = 9.06 /100; return; }
    else if (glu_mass == 660. ) { xsec = 0.627E+01; xsec_unc = 9.1 /100; return; }
    else if (glu_mass == 665. ) { xsec = 0.598E+01; xsec_unc = 9.15 /100; return; }
    else if (glu_mass == 670. ) { xsec = 0.571E+01; xsec_unc = 9.19 /100; return; }
    else if (glu_mass == 675. ) { xsec = 0.544E+01; xsec_unc = 9.23 /100; return; }
    else if (glu_mass == 680. ) { xsec = 0.520E+01; xsec_unc = 9.27 /100; return; }
    else if (glu_mass == 685. ) { xsec = 0.496E+01; xsec_unc = 9.31 /100; return; }
    else if (glu_mass == 690. ) { xsec = 0.474E+01; xsec_unc = 9.35 /100; return; }
    else if (glu_mass == 695. ) { xsec = 0.452E+01; xsec_unc = 9.39 /100; return; }
    else if (glu_mass == 700. ) { xsec = 0.432E+01; xsec_unc = 9.43 /100; return; }
    else if (glu_mass == 705. ) { xsec = 0.413E+01; xsec_unc = 9.46 /100; return; }
    else if (glu_mass == 710. ) { xsec = 0.395E+01; xsec_unc = 9.5 /100; return; }
    else if (glu_mass == 715. ) { xsec = 0.377E+01; xsec_unc = 9.54 /100; return; }
    else if (glu_mass == 720. ) { xsec = 0.361E+01; xsec_unc = 9.58 /100; return; }
    else if (glu_mass == 725. ) { xsec = 0.345E+01; xsec_unc = 9.61 /100; return; }
    else if (glu_mass == 730. ) { xsec = 0.330E+01; xsec_unc = 9.65 /100; return; }
    else if (glu_mass == 735. ) { xsec = 0.316E+01; xsec_unc = 9.69 /100; return; }
    else if (glu_mass == 740. ) { xsec = 0.302E+01; xsec_unc = 9.72 /100; return; }
    else if (glu_mass == 745. ) { xsec = 0.289E+01; xsec_unc = 9.76 /100; return; }
    else if (glu_mass == 750. ) { xsec = 0.277E+01; xsec_unc = 9.8 /100; return; }
    else if (glu_mass == 755. ) { xsec = 0.265E+01; xsec_unc = 9.83 /100; return; }
    else if (glu_mass == 760. ) { xsec = 0.254E+01; xsec_unc = 9.87 /100; return; }
    else if (glu_mass == 765. ) { xsec = 0.243E+01; xsec_unc = 9.91 /100; return; }
    else if (glu_mass == 770. ) { xsec = 0.233E+01; xsec_unc = 9.94 /100; return; }
    else if (glu_mass == 775. ) { xsec = 0.223E+01; xsec_unc = 9.98 /100; return; }
    else if (glu_mass == 780. ) { xsec = 0.214E+01; xsec_unc = 10.01 /100; return; }
    else if (glu_mass == 785. ) { xsec = 0.205E+01; xsec_unc = 10.05 /100; return; }
    else if (glu_mass == 790. ) { xsec = 0.197E+01; xsec_unc = 10.09 /100; return; }
    else if (glu_mass == 795. ) { xsec = 0.188E+01; xsec_unc = 10.12 /100; return; }
    else if (glu_mass == 800. ) { xsec = 0.181E+01; xsec_unc = 10.16 /100; return; }
    else if (glu_mass == 805. ) { xsec = 0.173E+01; xsec_unc = 10.2 /100; return; }
    else if (glu_mass == 810. ) { xsec = 0.166E+01; xsec_unc = 10.23 /100; return; }
    else if (glu_mass == 815. ) { xsec = 0.160E+01; xsec_unc = 10.27 /100; return; }
    else if (glu_mass == 820. ) { xsec = 0.153E+01; xsec_unc = 10.31 /100; return; }
    else if (glu_mass == 825. ) { xsec = 0.147E+01; xsec_unc = 10.34 /100; return; }
    else if (glu_mass == 830. ) { xsec = 0.141E+01; xsec_unc = 10.38 /100; return; }
    else if (glu_mass == 835. ) { xsec = 0.136E+01; xsec_unc = 10.42 /100; return; }
    else if (glu_mass == 840. ) { xsec = 0.130E+01; xsec_unc = 10.45 /100; return; }
    else if (glu_mass == 845. ) { xsec = 0.125E+01; xsec_unc = 10.49 /100; return; }
    else if (glu_mass == 850. ) { xsec = 0.120E+01; xsec_unc = 10.53 /100; return; }
    else if (glu_mass == 855. ) { xsec = 0.115E+01; xsec_unc = 10.57 /100; return; }
    else if (glu_mass == 860. ) { xsec = 0.111E+01; xsec_unc = 10.6 /100; return; }
    else if (glu_mass == 865. ) { xsec = 0.107E+01; xsec_unc = 10.64 /100; return; }
    else if (glu_mass == 870. ) { xsec = 0.103E+01; xsec_unc = 10.68 /100; return; }
    else if (glu_mass == 875. ) { xsec = 0.986E+00; xsec_unc = 10.71 /100; return; }
    else if (glu_mass == 880. ) { xsec = 0.948E+00; xsec_unc = 10.75 /100; return; }
    else if (glu_mass == 885. ) { xsec = 0.912E+00; xsec_unc = 10.79 /100; return; }
    else if (glu_mass == 890. ) { xsec = 0.877E+00; xsec_unc = 10.82 /100; return; }
    else if (glu_mass == 895. ) { xsec = 0.844E+00; xsec_unc = 10.86 /100; return; }
    else if (glu_mass == 900. ) { xsec = 0.812E+00; xsec_unc = 10.89 /100; return; }
    else if (glu_mass == 905. ) { xsec = 0.781E+00; xsec_unc = 10.93 /100; return; }
    else if (glu_mass == 910. ) { xsec = 0.752E+00; xsec_unc = 10.97 /100; return; }
    else if (glu_mass == 915. ) { xsec = 0.723E+00; xsec_unc = 11.0 /100; return; }
    else if (glu_mass == 920. ) { xsec = 0.696E+00; xsec_unc = 11.04 /100; return; }
    else if (glu_mass == 925. ) { xsec = 0.670E+00; xsec_unc = 11.07 /100; return; }
    else if (glu_mass == 930. ) { xsec = 0.646E+00; xsec_unc = 11.11 /100; return; }
    else if (glu_mass == 935. ) { xsec = 0.622E+00; xsec_unc = 11.14 /100; return; }
    else if (glu_mass == 940. ) { xsec = 0.599E+00; xsec_unc = 11.18 /100; return; }
    else if (glu_mass == 945. ) { xsec = 0.577E+00; xsec_unc = 11.21 /100; return; }
    else if (glu_mass == 950. ) { xsec = 0.556E+00; xsec_unc = 11.25 /100; return; }
    else if (glu_mass == 955. ) { xsec = 0.535E+00; xsec_unc = 11.28 /100; return; }
    else if (glu_mass == 960. ) { xsec = 0.516E+00; xsec_unc = 11.32 /100; return; }
    else if (glu_mass == 965. ) { xsec = 0.497E+00; xsec_unc = 11.35 /100; return; }
    else if (glu_mass == 970. ) { xsec = 0.479E+00; xsec_unc = 11.39 /100; return; }
    else if (glu_mass == 975. ) { xsec = 0.462E+00; xsec_unc = 11.42 /100; return; }
    else if (glu_mass == 980. ) { xsec = 0.445E+00; xsec_unc = 11.46 /100; return; }
    else if (glu_mass == 985. ) { xsec = 0.430E+00; xsec_unc = 11.49 /100; return; }
    else if (glu_mass == 990. ) { xsec = 0.414E+00; xsec_unc = 11.53 /100; return; }
    else if (glu_mass == 995. ) { xsec = 0.399E+00; xsec_unc = 11.56 /100; return; }
    else if (glu_mass == 1000 ) { xsec = 0.385E+00; xsec_unc = 11.6 /100; return; }
    else if (glu_mass == 1005 ) { xsec = 0.372E+00; xsec_unc = 11.63 /100; return; }
    else if (glu_mass == 1010 ) { xsec = 0.359E+00; xsec_unc = 11.67 /100; return; }
    else if (glu_mass == 1015 ) { xsec = 0.346E+00; xsec_unc = 11.7 /100; return; }
    else if (glu_mass == 1020 ) { xsec = 0.334E+00; xsec_unc = 11.74 /100; return; }
    else if (glu_mass == 1025 ) { xsec = 0.322E+00; xsec_unc = 11.78 /100; return; }
    else if (glu_mass == 1030 ) { xsec = 0.311E+00; xsec_unc = 11.81 /100; return; }
    else if (glu_mass == 1035 ) { xsec = 0.300E+00; xsec_unc = 11.85 /100; return; }
    else if (glu_mass == 1040 ) { xsec = 0.290E+00; xsec_unc = 11.88 /100; return; }
    else if (glu_mass == 1045 ) { xsec = 0.280E+00; xsec_unc = 11.92 /100; return; }
    else if (glu_mass == 1050 ) { xsec = 0.270E+00; xsec_unc = 11.95 /100; return; }
    else if (glu_mass == 1055 ) { xsec = 0.261E+00; xsec_unc = 11.99 /100; return; }
    else if (glu_mass == 1060 ) { xsec = 0.252E+00; xsec_unc = 12.02 /100; return; }
    else if (glu_mass == 1065 ) { xsec = 0.243E+00; xsec_unc = 12.06 /100; return; }
    else if (glu_mass == 1070 ) { xsec = 0.235E+00; xsec_unc = 12.09 /100; return; }
    else if (glu_mass == 1075 ) { xsec = 0.227E+00; xsec_unc = 12.13 /100; return; }
    else if (glu_mass == 1080 ) { xsec = 0.219E+00; xsec_unc = 12.17 /100; return; }
    else if (glu_mass == 1085 ) { xsec = 0.212E+00; xsec_unc = 12.2 /100; return; }
    else if (glu_mass == 1090 ) { xsec = 0.205E+00; xsec_unc = 12.24 /100; return; }
    else if (glu_mass == 1095 ) { xsec = 0.198E+00; xsec_unc = 12.27 /100; return; }
    else if (glu_mass == 1100 ) { xsec = 0.191E+00; xsec_unc = 12.31 /100; return; }
    else if (glu_mass == 1105 ) { xsec = 0.185E+00; xsec_unc = 12.34 /100; return; }
    else if (glu_mass == 1110 ) { xsec = 0.179E+00; xsec_unc = 12.38 /100; return; }
    else if (glu_mass == 1115 ) { xsec = 0.173E+00; xsec_unc = 12.42 /100; return; }
    else if (glu_mass == 1120 ) { xsec = 0.167E+00; xsec_unc = 12.45 /100; return; }
    else if (glu_mass == 1125 ) { xsec = 0.162E+00; xsec_unc = 12.49 /100; return; }
    else if (glu_mass == 1130 ) { xsec = 0.156E+00; xsec_unc = 12.53 /100; return; }
    else if (glu_mass == 1135 ) { xsec = 0.151E+00; xsec_unc = 12.56 /100; return; }
    else if (glu_mass == 1140 ) { xsec = 0.146E+00; xsec_unc = 12.6 /100; return; }
    else if (glu_mass == 1145 ) { xsec = 0.141E+00; xsec_unc = 12.64 /100; return; }
    else if (glu_mass == 1150 ) { xsec = 0.137E+00; xsec_unc = 12.67 /100; return; }
    else if (glu_mass == 1155 ) { xsec = 0.132E+00; xsec_unc = 12.71 /100; return; }
    else if (glu_mass == 1160 ) { xsec = 0.128E+00; xsec_unc = 12.74 /100; return; }
    else if (glu_mass == 1165 ) { xsec = 0.124E+00; xsec_unc = 12.78 /100; return; }
    else if (glu_mass == 1170 ) { xsec = 0.120E+00; xsec_unc = 12.82 /100; return; }
    else if (glu_mass == 1175 ) { xsec = 0.116E+00; xsec_unc = 12.85 /100; return; }
    else if (glu_mass == 1180 ) { xsec = 0.112E+00; xsec_unc = 12.89 /100; return; }
    else if (glu_mass == 1185 ) { xsec = 0.109E+00; xsec_unc = 12.92 /100; return; }
    else if (glu_mass == 1190 ) { xsec = 0.105E+00; xsec_unc = 12.96 /100; return; }
    else if (glu_mass == 1195 ) { xsec = 0.102E+00; xsec_unc = 13.0 /100; return; }
    else if (glu_mass == 1200 ) { xsec = 0.985E-01; xsec_unc = 13.03 /100; return; }
    else if (glu_mass == 1205 ) { xsec = 0.953E-01; xsec_unc = 13.07 /100; return; }
    else if (glu_mass == 1210 ) { xsec = 0.923E-01; xsec_unc = 13.1 /100; return; }
    else if (glu_mass == 1215 ) { xsec = 0.894E-01; xsec_unc = 13.14 /100; return; }
    else if (glu_mass == 1220 ) { xsec = 0.866E-01; xsec_unc = 13.17 /100; return; }
    else if (glu_mass == 1225 ) { xsec = 0.838E-01; xsec_unc = 13.21 /100; return; }
    else if (glu_mass == 1230 ) { xsec = 0.812E-01; xsec_unc = 13.24 /100; return; }
    else if (glu_mass == 1235 ) { xsec = 0.786E-01; xsec_unc = 13.27 /100; return; }
    else if (glu_mass == 1240 ) { xsec = 0.762E-01; xsec_unc = 13.31 /100; return; }
    else if (glu_mass == 1245 ) { xsec = 0.738E-01; xsec_unc = 13.34 /100; return; }
    else if (glu_mass == 1250 ) { xsec = 0.715E-01; xsec_unc = 13.38 /100; return; }
    else if (glu_mass == 1255 ) { xsec = 0.692E-01; xsec_unc = 13.41 /100; return; }
    else if (glu_mass == 1260 ) { xsec = 0.671E-01; xsec_unc = 13.45 /100; return; }
    else if (glu_mass == 1265 ) { xsec = 0.650E-01; xsec_unc = 13.48 /100; return; }
    else if (glu_mass == 1270 ) { xsec = 0.630E-01; xsec_unc = 13.51 /100; return; }
    else if (glu_mass == 1275 ) { xsec = 0.610E-01; xsec_unc = 13.55 /100; return; }
    else if (glu_mass == 1280 ) { xsec = 0.591E-01; xsec_unc = 13.58 /100; return; }
    else if (glu_mass == 1285 ) { xsec = 0.573E-01; xsec_unc = 13.62 /100; return; }
    else if (glu_mass == 1290 ) { xsec = 0.556E-01; xsec_unc = 13.65 /100; return; }
    else if (glu_mass == 1295 ) { xsec = 0.539E-01; xsec_unc = 13.69 /100; return; }
    else if (glu_mass == 1300 ) { xsec = 0.522E-01; xsec_unc = 13.72 /100; return; }
    else if (glu_mass == 1305 ) { xsec = 0.506E-01; xsec_unc = 13.76 /100; return; }
    else if (glu_mass == 1310 ) { xsec = 0.491E-01; xsec_unc = 13.79 /100; return; }
    else if (glu_mass == 1315 ) { xsec = 0.476E-01; xsec_unc = 13.83 /100; return; }
    else if (glu_mass == 1320 ) { xsec = 0.461E-01; xsec_unc = 13.86 /100; return; }
    else if (glu_mass == 1325 ) { xsec = 0.447E-01; xsec_unc = 13.9 /100; return; }
    else if (glu_mass == 1330 ) { xsec = 0.434E-01; xsec_unc = 13.94 /100; return; }
    else if (glu_mass == 1335 ) { xsec = 0.421E-01; xsec_unc = 13.97 /100; return; }
    else if (glu_mass == 1340 ) { xsec = 0.408E-01; xsec_unc = 14.01 /100; return; }
    else if (glu_mass == 1345 ) { xsec = 0.396E-01; xsec_unc = 14.04 /100; return; }
    else if (glu_mass == 1350 ) { xsec = 0.384E-01; xsec_unc = 14.08 /100; return; }
    else if (glu_mass == 1355 ) { xsec = 0.372E-01; xsec_unc = 14.11 /100; return; }
    else if (glu_mass == 1360 ) { xsec = 0.361E-01; xsec_unc = 14.15 /100; return; }
    else if (glu_mass == 1365 ) { xsec = 0.350E-01; xsec_unc = 14.19 /100; return; }
    else if (glu_mass == 1370 ) { xsec = 0.340E-01; xsec_unc = 14.22 /100; return; }
    else if (glu_mass == 1375 ) { xsec = 0.330E-01; xsec_unc = 14.26 /100; return; }
    else if (glu_mass == 1380 ) { xsec = 0.320E-01; xsec_unc = 14.3 /100; return; }
    else if (glu_mass == 1385 ) { xsec = 0.310E-01; xsec_unc = 14.33 /100; return; }
    else if (glu_mass == 1390 ) { xsec = 0.301E-01; xsec_unc = 14.37 /100; return; }
    else if (glu_mass == 1395 ) { xsec = 0.292E-01; xsec_unc = 14.4 /100; return; }
    else if (glu_mass == 1400 ) { xsec = 0.284E-01; xsec_unc = 14.44 /100; return; }
    else if (glu_mass == 1405 ) { xsec = 0.275E-01; xsec_unc = 14.48 /100; return; }
    else if (glu_mass == 1410 ) { xsec = 0.267E-01; xsec_unc = 14.51 /100; return; }
    else if (glu_mass == 1415 ) { xsec = 0.259E-01; xsec_unc = 14.55 /100; return; }
    else if (glu_mass == 1420 ) { xsec = 0.252E-01; xsec_unc = 14.59 /100; return; }
    else if (glu_mass == 1425 ) { xsec = 0.244E-01; xsec_unc = 14.63 /100; return; }
    else if (glu_mass == 1430 ) { xsec = 0.237E-01; xsec_unc = 14.66 /100; return; }
    else if (glu_mass == 1435 ) { xsec = 0.230E-01; xsec_unc = 14.7 /100; return; }
    else if (glu_mass == 1440 ) { xsec = 0.224E-01; xsec_unc = 14.74 /100; return; }
    else if (glu_mass == 1445 ) { xsec = 0.217E-01; xsec_unc = 14.77 /100; return; }
    else if (glu_mass == 1450 ) { xsec = 0.211E-01; xsec_unc = 14.81 /100; return; }
    else if (glu_mass == 1455 ) { xsec = 0.205E-01; xsec_unc = 14.85 /100; return; }
    else if (glu_mass == 1460 ) { xsec = 0.199E-01; xsec_unc = 14.88 /100; return; }
    else if (glu_mass == 1465 ) { xsec = 0.193E-01; xsec_unc = 14.92 /100; return; }
    else if (glu_mass == 1470 ) { xsec = 0.187E-01; xsec_unc = 14.96 /100; return; }
    else if (glu_mass == 1475 ) { xsec = 0.182E-01; xsec_unc = 15.0 /100; return; }
    else if (glu_mass == 1480 ) { xsec = 0.177E-01; xsec_unc = 15.03 /100; return; }
    else if (glu_mass == 1485 ) { xsec = 0.172E-01; xsec_unc = 15.07 /100; return; }
    else if (glu_mass == 1490 ) { xsec = 0.167E-01; xsec_unc = 15.11 /100; return; }
    else if (glu_mass == 1495 ) { xsec = 0.162E-01; xsec_unc = 15.15 /100; return; }
    else if (glu_mass == 1500 ) { xsec = 0.157E-01; xsec_unc = 15.18 /100; return; }
    else if (glu_mass == 1505 ) { xsec = 0.153E-01; xsec_unc = 15.22 /100; return; }
    else if (glu_mass == 1510 ) { xsec = 0.148E-01; xsec_unc = 15.26 /100; return; }
    else if (glu_mass == 1515 ) { xsec = 0.144E-01; xsec_unc = 15.3 /100; return; }
    else if (glu_mass == 1520 ) { xsec = 0.140E-01; xsec_unc = 15.33 /100; return; }
    else if (glu_mass == 1525 ) { xsec = 0.136E-01; xsec_unc = 15.37 /100; return; }
    else if (glu_mass == 1530 ) { xsec = 0.132E-01; xsec_unc = 15.41 /100; return; }
    else if (glu_mass == 1535 ) { xsec = 0.128E-01; xsec_unc = 15.45 /100; return; }
    else if (glu_mass == 1540 ) { xsec = 0.125E-01; xsec_unc = 15.48 /100; return; }
    else if (glu_mass == 1545 ) { xsec = 0.121E-01; xsec_unc = 15.52 /100; return; }
    else if (glu_mass == 1550 ) { xsec = 0.118E-01; xsec_unc = 15.56 /100; return; }
    else if (glu_mass == 1555 ) { xsec = 0.115E-01; xsec_unc = 15.6 /100; return; }
    else if (glu_mass == 1560 ) { xsec = 0.111E-01; xsec_unc = 15.64 /100; return; }
    else if (glu_mass == 1565 ) { xsec = 0.108E-01; xsec_unc = 15.67 /100; return; }
    else if (glu_mass == 1570 ) { xsec = 0.105E-01; xsec_unc = 15.71 /100; return; }
    else if (glu_mass == 1575 ) { xsec = 0.102E-01; xsec_unc = 15.75 /100; return; }
    else if (glu_mass == 1580 ) { xsec = 0.993E-02; xsec_unc = 15.79 /100; return; }
    else if (glu_mass == 1585 ) { xsec = 0.966E-02; xsec_unc = 15.83 /100; return; }
    else if (glu_mass == 1590 ) { xsec = 0.939E-02; xsec_unc = 15.87 /100; return; }
    else if (glu_mass == 1595 ) { xsec = 0.912E-02; xsec_unc = 15.9 /100; return; }
    else if (glu_mass == 1600 ) { xsec = 0.887E-02; xsec_unc = 15.94 /100; return; }
    else if (glu_mass == 1605 ) { xsec = 0.862E-02; xsec_unc = 15.98 /100; return; }
    else if (glu_mass == 1610 ) { xsec = 0.838E-02; xsec_unc = 16.02 /100; return; }
    else if (glu_mass == 1615 ) { xsec = 0.815E-02; xsec_unc = 16.06 /100; return; }
    else if (glu_mass == 1620 ) { xsec = 0.792E-02; xsec_unc = 16.1 /100; return; }
    else if (glu_mass == 1625 ) { xsec = 0.770E-02; xsec_unc = 16.13 /100; return; }
    else if (glu_mass == 1630 ) { xsec = 0.749E-02; xsec_unc = 16.17 /100; return; }
    else if (glu_mass == 1635 ) { xsec = 0.728E-02; xsec_unc = 16.21 /100; return; }
    else if (glu_mass == 1640 ) { xsec = 0.708E-02; xsec_unc = 16.25 /100; return; }
    else if (glu_mass == 1645 ) { xsec = 0.689E-02; xsec_unc = 16.29 /100; return; }
    else if (glu_mass == 1650 ) { xsec = 0.670E-02; xsec_unc = 16.33 /100; return; }
    else if (glu_mass == 1655 ) { xsec = 0.651E-02; xsec_unc = 16.37 /100; return; }
    else if (glu_mass == 1660 ) { xsec = 0.633E-02; xsec_unc = 16.41 /100; return; }
    else if (glu_mass == 1665 ) { xsec = 0.616E-02; xsec_unc = 16.45 /100; return; }
    else if (glu_mass == 1670 ) { xsec = 0.599E-02; xsec_unc = 16.49 /100; return; }
    else if (glu_mass == 1675 ) { xsec = 0.583E-02; xsec_unc = 16.53 /100; return; }
    else if (glu_mass == 1680 ) { xsec = 0.567E-02; xsec_unc = 16.56 /100; return; }
    else if (glu_mass == 1685 ) { xsec = 0.551E-02; xsec_unc = 16.6 /100; return; }
    else if (glu_mass == 1690 ) { xsec = 0.536E-02; xsec_unc = 16.64 /100; return; }
    else if (glu_mass == 1695 ) { xsec = 0.521E-02; xsec_unc = 16.68 /100; return; }
    else if (glu_mass == 1700 ) { xsec = 0.507E-02; xsec_unc = 16.72 /100; return; }
    else if (glu_mass == 1705 ) { xsec = 0.493E-02; xsec_unc = 16.76 /100; return; }
    else if (glu_mass == 1710 ) { xsec = 0.480E-02; xsec_unc = 16.81 /100; return; }
    else if (glu_mass == 1715 ) { xsec = 0.467E-02; xsec_unc = 16.85 /100; return; }
    else if (glu_mass == 1720 ) { xsec = 0.454E-02; xsec_unc = 16.89 /100; return; }
    else if (glu_mass == 1725 ) { xsec = 0.442E-02; xsec_unc = 16.93 /100; return; }
    else if (glu_mass == 1730 ) { xsec = 0.430E-02; xsec_unc = 16.97 /100; return; }
    else if (glu_mass == 1735 ) { xsec = 0.418E-02; xsec_unc = 17.01 /100; return; }
    else if (glu_mass == 1740 ) { xsec = 0.407E-02; xsec_unc = 17.05 /100; return; }
    else if (glu_mass == 1745 ) { xsec = 0.396E-02; xsec_unc = 17.09 /100; return; }
    else if (glu_mass == 1750 ) { xsec = 0.385E-02; xsec_unc = 17.13 /100; return; }
    else if (glu_mass == 1755 ) { xsec = 0.375E-02; xsec_unc = 17.18 /100; return; }
    else if (glu_mass == 1760 ) { xsec = 0.365E-02; xsec_unc = 17.22 /100; return; }
    else if (glu_mass == 1765 ) { xsec = 0.355E-02; xsec_unc = 17.26 /100; return; }
    else if (glu_mass == 1770 ) { xsec = 0.345E-02; xsec_unc = 17.3 /100; return; }
    else if (glu_mass == 1775 ) { xsec = 0.336E-02; xsec_unc = 17.34 /100; return; }
    else if (glu_mass == 1780 ) { xsec = 0.327E-02; xsec_unc = 17.39 /100; return; }
    else if (glu_mass == 1785 ) { xsec = 0.318E-02; xsec_unc = 17.43 /100; return; }
    else if (glu_mass == 1790 ) { xsec = 0.310E-02; xsec_unc = 17.47 /100; return; }
    else if (glu_mass == 1795 ) { xsec = 0.301E-02; xsec_unc = 17.51 /100; return; }
    else if (glu_mass == 1800 ) { xsec = 0.293E-02; xsec_unc = 17.56 /100; return; }
    else if (glu_mass == 1805 ) { xsec = 0.286E-02; xsec_unc = 17.6 /100; return; }
    else if (glu_mass == 1810 ) { xsec = 0.278E-02; xsec_unc = 17.64 /100; return; }
    else if (glu_mass == 1815 ) { xsec = 0.271E-02; xsec_unc = 17.69 /100; return; }
    else if (glu_mass == 1820 ) { xsec = 0.263E-02; xsec_unc = 17.73 /100; return; }
    else if (glu_mass == 1825 ) { xsec = 0.256E-02; xsec_unc = 17.77 /100; return; }
    else if (glu_mass == 1830 ) { xsec = 0.249E-02; xsec_unc = 17.82 /100; return; }
    else if (glu_mass == 1835 ) { xsec = 0.243E-02; xsec_unc = 17.86 /100; return; }
    else if (glu_mass == 1840 ) { xsec = 0.236E-02; xsec_unc = 17.9 /100; return; }
    else if (glu_mass == 1845 ) { xsec = 0.230E-02; xsec_unc = 17.95 /100; return; }
    else if (glu_mass == 1850 ) { xsec = 0.224E-02; xsec_unc = 17.99 /100; return; }
    else if (glu_mass == 1855 ) { xsec = 0.218E-02; xsec_unc = 18.04 /100; return; }
    else if (glu_mass == 1860 ) { xsec = 0.212E-02; xsec_unc = 18.08 /100; return; }
    else if (glu_mass == 1865 ) { xsec = 0.207E-02; xsec_unc = 18.13 /100; return; }
    else if (glu_mass == 1870 ) { xsec = 0.201E-02; xsec_unc = 18.17 /100; return; }
    else if (glu_mass == 1875 ) { xsec = 0.196E-02; xsec_unc = 18.22 /100; return; }
    else if (glu_mass == 1880 ) { xsec = 0.191E-02; xsec_unc = 18.26 /100; return; }
    else if (glu_mass == 1885 ) { xsec = 0.186E-02; xsec_unc = 18.31 /100; return; }
    else if (glu_mass == 1890 ) { xsec = 0.181E-02; xsec_unc = 18.35 /100; return; }
    else if (glu_mass == 1895 ) { xsec = 0.176E-02; xsec_unc = 18.4 /100; return; }
    else if (glu_mass == 1900 ) { xsec = 0.171E-02; xsec_unc = 18.45 /100; return; }
    else if (glu_mass == 1905 ) { xsec = 0.167E-02; xsec_unc = 18.49 /100; return; }
    else if (glu_mass == 1910 ) { xsec = 0.163E-02; xsec_unc = 18.54 /100; return; }
    else if (glu_mass == 1915 ) { xsec = 0.158E-02; xsec_unc = 18.59 /100; return; }
    else if (glu_mass == 1920 ) { xsec = 0.154E-02; xsec_unc = 18.63 /100; return; }
    else if (glu_mass == 1925 ) { xsec = 0.150E-02; xsec_unc = 18.68 /100; return; }
    else if (glu_mass == 1930 ) { xsec = 0.146E-02; xsec_unc = 18.73 /100; return; }
    else if (glu_mass == 1935 ) { xsec = 0.142E-02; xsec_unc = 18.78 /100; return; }
    else if (glu_mass == 1940 ) { xsec = 0.139E-02; xsec_unc = 18.82 /100; return; }
    else if (glu_mass == 1945 ) { xsec = 0.135E-02; xsec_unc = 18.87 /100; return; }
    else if (glu_mass == 1950 ) { xsec = 0.131E-02; xsec_unc = 18.92 /100; return; }
    else if (glu_mass == 1955 ) { xsec = 0.128E-02; xsec_unc = 18.97 /100; return; }
    else if (glu_mass == 1960 ) { xsec = 0.125E-02; xsec_unc = 19.02 /100; return; }
    else if (glu_mass == 1965 ) { xsec = 0.121E-02; xsec_unc = 19.07 /100; return; }
    else if (glu_mass == 1970 ) { xsec = 0.118E-02; xsec_unc = 19.12 /100; return; }
    else if (glu_mass == 1975 ) { xsec = 0.115E-02; xsec_unc = 19.17 /100; return; }
    else if (glu_mass == 1980 ) { xsec = 0.112E-02; xsec_unc = 19.22 /100; return; }
    else if (glu_mass == 1985 ) { xsec = 0.109E-02; xsec_unc = 19.27 /100; return; }
    else if (glu_mass == 1990 ) { xsec = 0.106E-02; xsec_unc = 19.32 /100; return; }
    else if (glu_mass == 1995 ) { xsec = 0.104E-02; xsec_unc = 19.37 /100; return; }
    else if (glu_mass == 2000 ) { xsec = 0.101E-02; xsec_unc = 19.42 /100; return; }
    else if (glu_mass == 2005 ) { xsec = 0.983E-03; xsec_unc = 19.48 /100; return; }
    else if (glu_mass == 2010 ) { xsec = 0.957E-03; xsec_unc = 19.53 /100; return; }
    else if (glu_mass == 2015 ) { xsec = 0.933E-03; xsec_unc = 19.58 /100; return; }
    else if (glu_mass == 2020 ) { xsec = 0.908E-03; xsec_unc = 19.64 /100; return; }
    else if (glu_mass == 2025 ) { xsec = 0.885E-03; xsec_unc = 19.69 /100; return; }
    else if (glu_mass == 2030 ) { xsec = 0.862E-03; xsec_unc = 19.74 /100; return; }
    else if (glu_mass == 2035 ) { xsec = 0.840E-03; xsec_unc = 19.8 /100; return; }
    else if (glu_mass == 2040 ) { xsec = 0.818E-03; xsec_unc = 19.85 /100; return; }
    else if (glu_mass == 2045 ) { xsec = 0.797E-03; xsec_unc = 19.91 /100; return; }
    else if (glu_mass == 2050 ) { xsec = 0.776E-03; xsec_unc = 19.96 /100; return; }
    else if (glu_mass == 2055 ) { xsec = 0.756E-03; xsec_unc = 20.02 /100; return; }
    else if (glu_mass == 2060 ) { xsec = 0.737E-03; xsec_unc = 20.07 /100; return; }
    else if (glu_mass == 2065 ) { xsec = 0.718E-03; xsec_unc = 20.13 /100; return; }
    else if (glu_mass == 2070 ) { xsec = 0.699E-03; xsec_unc = 20.19 /100; return; }
    else if (glu_mass == 2075 ) { xsec = 0.681E-03; xsec_unc = 20.25 /100; return; }
    else if (glu_mass == 2080 ) { xsec = 0.664E-03; xsec_unc = 20.3 /100; return; }
    else if (glu_mass == 2085 ) { xsec = 0.647E-03; xsec_unc = 20.36 /100; return; }
    else if (glu_mass == 2090 ) { xsec = 0.630E-03; xsec_unc = 20.42 /100; return; }
    else if (glu_mass == 2095 ) { xsec = 0.614E-03; xsec_unc = 20.48 /100; return; }
    else if (glu_mass == 2100 ) { xsec = 0.598E-03; xsec_unc = 20.54 /100; return; }
    else if (glu_mass == 2105 ) { xsec = 0.583E-03; xsec_unc = 20.6 /100; return; }
    else if (glu_mass == 2110 ) { xsec = 0.568E-03; xsec_unc = 20.66 /100; return; }
    else if (glu_mass == 2115 ) { xsec = 0.553E-03; xsec_unc = 20.72 /100; return; }
    else if (glu_mass == 2120 ) { xsec = 0.539E-03; xsec_unc = 20.78 /100; return; }
    else if (glu_mass == 2125 ) { xsec = 0.525E-03; xsec_unc = 20.84 /100; return; }
    else if (glu_mass == 2130 ) { xsec = 0.512E-03; xsec_unc = 20.9 /100; return; }
    else if (glu_mass == 2135 ) { xsec = 0.499E-03; xsec_unc = 20.97 /100; return; }
    else if (glu_mass == 2140 ) { xsec = 0.486E-03; xsec_unc = 21.03 /100; return; }
    else if (glu_mass == 2145 ) { xsec = 0.473E-03; xsec_unc = 21.09 /100; return; }
    else if (glu_mass == 2150 ) { xsec = 0.461E-03; xsec_unc = 21.16 /100; return; }
    else if (glu_mass == 2155 ) { xsec = 0.449E-03; xsec_unc = 21.22 /100; return; }
    else if (glu_mass == 2160 ) { xsec = 0.438E-03; xsec_unc = 21.29 /100; return; }
    else if (glu_mass == 2165 ) { xsec = 0.427E-03; xsec_unc = 21.35 /100; return; }
    else if (glu_mass == 2170 ) { xsec = 0.416E-03; xsec_unc = 21.42 /100; return; }
    else if (glu_mass == 2175 ) { xsec = 0.405E-03; xsec_unc = 21.48 /100; return; }
    else if (glu_mass == 2180 ) { xsec = 0.395E-03; xsec_unc = 21.55 /100; return; }
    else if (glu_mass == 2185 ) { xsec = 0.385E-03; xsec_unc = 21.62 /100; return; }
    else if (glu_mass == 2190 ) { xsec = 0.375E-03; xsec_unc = 21.69 /100; return; }
    else if (glu_mass == 2195 ) { xsec = 0.365E-03; xsec_unc = 21.76 /100; return; }
    else if (glu_mass == 2200 ) { xsec = 0.356E-03; xsec_unc = 21.83 /100; return; }
    else if (glu_mass == 2205 ) { xsec = 0.347E-03; xsec_unc = 21.9 /100; return; }
    else if (glu_mass == 2210 ) { xsec = 0.338E-03; xsec_unc = 21.97 /100; return; }
    else if (glu_mass == 2215 ) { xsec = 0.330E-03; xsec_unc = 22.04 /100; return; }
    else if (glu_mass == 2220 ) { xsec = 0.321E-03; xsec_unc = 22.11 /100; return; }
    else if (glu_mass == 2225 ) { xsec = 0.313E-03; xsec_unc = 22.18 /100; return; }
    else if (glu_mass == 2230 ) { xsec = 0.305E-03; xsec_unc = 22.25 /100; return; }
    else if (glu_mass == 2235 ) { xsec = 0.297E-03; xsec_unc = 22.33 /100; return; }
    else if (glu_mass == 2240 ) { xsec = 0.290E-03; xsec_unc = 22.4 /100; return; }
    else if (glu_mass == 2245 ) { xsec = 0.283E-03; xsec_unc = 22.47 /100; return; }
    else if (glu_mass == 2250 ) { xsec = 0.275E-03; xsec_unc = 22.55 /100; return; }
    else if (glu_mass == 2255 ) { xsec = 0.268E-03; xsec_unc = 22.63 /100; return; }
    else if (glu_mass == 2260 ) { xsec = 0.262E-03; xsec_unc = 22.7 /100; return; }
    else if (glu_mass == 2265 ) { xsec = 0.255E-03; xsec_unc = 22.78 /100; return; }
    else if (glu_mass == 2270 ) { xsec = 0.248E-03; xsec_unc = 22.86 /100; return; }
    else if (glu_mass == 2275 ) { xsec = 0.242E-03; xsec_unc = 22.94 /100; return; }
    else if (glu_mass == 2280 ) { xsec = 0.236E-03; xsec_unc = 23.02 /100; return; }
    else if (glu_mass == 2285 ) { xsec = 0.230E-03; xsec_unc = 23.1 /100; return; }
    else if (glu_mass == 2290 ) { xsec = 0.224E-03; xsec_unc = 23.18 /100; return; }
    else if (glu_mass == 2295 ) { xsec = 0.219E-03; xsec_unc = 23.26 /100; return; }
    else if (glu_mass == 2300 ) { xsec = 0.213E-03; xsec_unc = 23.34 /100; return; }
    else if (glu_mass == 2305 ) { xsec = 0.208E-03; xsec_unc = 23.43 /100; return; }
    else if (glu_mass == 2310 ) { xsec = 0.202E-03; xsec_unc = 23.51 /100; return; }
    else if (glu_mass == 2315 ) { xsec = 0.197E-03; xsec_unc = 23.6 /100; return; }
    else if (glu_mass == 2320 ) { xsec = 0.192E-03; xsec_unc = 23.68 /100; return; }
    else if (glu_mass == 2325 ) { xsec = 0.187E-03; xsec_unc = 23.77 /100; return; }
    else if (glu_mass == 2330 ) { xsec = 0.183E-03; xsec_unc = 23.86 /100; return; }
    else if (glu_mass == 2335 ) { xsec = 0.178E-03; xsec_unc = 23.95 /100; return; }
    else if (glu_mass == 2340 ) { xsec = 0.174E-03; xsec_unc = 24.04 /100; return; }
    else if (glu_mass == 2345 ) { xsec = 0.169E-03; xsec_unc = 24.13 /100; return; }
    else if (glu_mass == 2350 ) { xsec = 0.165E-03; xsec_unc = 24.22 /100; return; }
    else if (glu_mass == 2355 ) { xsec = 0.161E-03; xsec_unc = 24.31 /100; return; }
    else if (glu_mass == 2360 ) { xsec = 0.157E-03; xsec_unc = 24.41 /100; return; }
    else if (glu_mass == 2365 ) { xsec = 0.153E-03; xsec_unc = 24.5 /100; return; }
    else if (glu_mass == 2370 ) { xsec = 0.149E-03; xsec_unc = 24.6 /100; return; }
    else if (glu_mass == 2375 ) { xsec = 0.145E-03; xsec_unc = 24.7 /100; return; }
    else if (glu_mass == 2380 ) { xsec = 0.142E-03; xsec_unc = 24.79 /100; return; }
    else if (glu_mass == 2385 ) { xsec = 0.138E-03; xsec_unc = 24.89 /100; return; }
    else if (glu_mass == 2390 ) { xsec = 0.134E-03; xsec_unc = 24.99 /100; return; }
    else if (glu_mass == 2395 ) { xsec = 0.131E-03; xsec_unc = 25.09 /100; return; }
    else if (glu_mass == 2400 ) { xsec = 0.128E-03; xsec_unc = 25.19 /100; return; }
    else if (glu_mass == 2405 ) { xsec = 0.125E-03; xsec_unc = 25.3 /100; return; }
    else if (glu_mass == 2410 ) { xsec = 0.121E-03; xsec_unc = 25.4 /100; return; }
    else if (glu_mass == 2415 ) { xsec = 0.118E-03; xsec_unc = 25.5 /100; return; }
    else if (glu_mass == 2420 ) { xsec = 0.115E-03; xsec_unc = 25.61 /100; return; }
    else if (glu_mass == 2425 ) { xsec = 0.113E-03; xsec_unc = 25.71 /100; return; }
    else if (glu_mass == 2430 ) { xsec = 0.110E-03; xsec_unc = 25.82 /100; return; }
    else if (glu_mass == 2435 ) { xsec = 0.107E-03; xsec_unc = 25.93 /100; return; }
    else if (glu_mass == 2440 ) { xsec = 0.104E-03; xsec_unc = 26.04 /100; return; }
    else if (glu_mass == 2445 ) { xsec = 0.102E-03; xsec_unc = 26.15 /100; return; }
    else if (glu_mass == 2450 ) { xsec = 0.991E-04; xsec_unc = 26.26 /100; return; }
    else if (glu_mass == 2455 ) { xsec = 0.966E-04; xsec_unc = 26.37 /100; return; }
    else if (glu_mass == 2460 ) { xsec = 0.941E-04; xsec_unc = 26.49 /100; return; }
    else if (glu_mass == 2465 ) { xsec = 0.918E-04; xsec_unc = 26.6 /100; return; }
    else if (glu_mass == 2470 ) { xsec = 0.895E-04; xsec_unc = 26.72 /100; return; }
    else if (glu_mass == 2475 ) { xsec = 0.872E-04; xsec_unc = 26.84 /100; return; }
    else if (glu_mass == 2480 ) { xsec = 0.850E-04; xsec_unc = 26.96 /100; return; }
    else if (glu_mass == 2485 ) { xsec = 0.829E-04; xsec_unc = 27.08 /100; return; }
    else if (glu_mass == 2490 ) { xsec = 0.808E-04; xsec_unc = 27.2 /100; return; }
    else if (glu_mass == 2495 ) { xsec = 0.788E-04; xsec_unc = 27.33 /100; return; }
    else if (glu_mass == 2500 ) { xsec = 0.768E-04; xsec_unc = 27.45 /100; return; }
    else if (glu_mass == 2505 ) { xsec = 0.749E-04; xsec_unc = 27.58 /100; return; }
    else if (glu_mass == 2510 ) { xsec = 0.730E-04; xsec_unc = 27.71 /100; return; }
    else if (glu_mass == 2515 ) { xsec = 0.712E-04; xsec_unc = 27.84 /100; return; }
    else if (glu_mass == 2520 ) { xsec = 0.694E-04; xsec_unc = 27.97 /100; return; }
    else if (glu_mass == 2525 ) { xsec = 0.677E-04; xsec_unc = 28.11 /100; return; }
    else if (glu_mass == 2530 ) { xsec = 0.660E-04; xsec_unc = 28.24 /100; return; }
    else if (glu_mass == 2535 ) { xsec = 0.643E-04; xsec_unc = 28.38 /100; return; }
    else if (glu_mass == 2540 ) { xsec = 0.627E-04; xsec_unc = 28.52 /100; return; }
    else if (glu_mass == 2545 ) { xsec = 0.611E-04; xsec_unc = 28.66 /100; return; }
    else if (glu_mass == 2550 ) { xsec = 0.596E-04; xsec_unc = 28.8 /100; return; }
    else if (glu_mass == 2555 ) { xsec = 0.581E-04; xsec_unc = 28.94 /100; return; }
    else if (glu_mass == 2560 ) { xsec = 0.566E-04; xsec_unc = 29.09 /100; return; }
    else if (glu_mass == 2565 ) { xsec = 0.552E-04; xsec_unc = 29.23 /100; return; }
    else if (glu_mass == 2570 ) { xsec = 0.538E-04; xsec_unc = 29.38 /100; return; }
    else if (glu_mass == 2575 ) { xsec = 0.525E-04; xsec_unc = 29.53 /100; return; }
    else if (glu_mass == 2580 ) { xsec = 0.512E-04; xsec_unc = 29.68 /100; return; }
    else if (glu_mass == 2585 ) { xsec = 0.499E-04; xsec_unc = 29.84 /100; return; }
    else if (glu_mass == 2590 ) { xsec = 0.486E-04; xsec_unc = 29.99 /100; return; }
    else if (glu_mass == 2595 ) { xsec = 0.474E-04; xsec_unc = 30.15 /100; return; }
    else if (glu_mass == 2600 ) { xsec = 0.462E-04; xsec_unc = 30.31 /100; return; }
    else if (glu_mass == 2605 ) { xsec = 0.451E-04; xsec_unc = 30.47 /100; return; }
    else if (glu_mass == 2610 ) { xsec = 0.439E-04; xsec_unc = 30.63 /100; return; }
    else if (glu_mass == 2615 ) { xsec = 0.428E-04; xsec_unc = 30.8 /100; return; }
    else if (glu_mass == 2620 ) { xsec = 0.418E-04; xsec_unc = 30.97 /100; return; }
    else if (glu_mass == 2625 ) { xsec = 0.407E-04; xsec_unc = 31.13 /100; return; }
    else if (glu_mass == 2630 ) { xsec = 0.397E-04; xsec_unc = 31.3 /100; return; }
    else if (glu_mass == 2635 ) { xsec = 0.387E-04; xsec_unc = 31.48 /100; return; }
    else if (glu_mass == 2640 ) { xsec = 0.377E-04; xsec_unc = 31.65 /100; return; }
    else if (glu_mass == 2645 ) { xsec = 0.368E-04; xsec_unc = 31.83 /100; return; }
    else if (glu_mass == 2650 ) { xsec = 0.359E-04; xsec_unc = 32.01 /100; return; }
    else if (glu_mass == 2655 ) { xsec = 0.350E-04; xsec_unc = 32.19 /100; return; }
    else if (glu_mass == 2660 ) { xsec = 0.341E-04; xsec_unc = 32.37 /100; return; }
    else if (glu_mass == 2665 ) { xsec = 0.332E-04; xsec_unc = 32.56 /100; return; }
    else if (glu_mass == 2670 ) { xsec = 0.324E-04; xsec_unc = 32.74 /100; return; }
    else if (glu_mass == 2675 ) { xsec = 0.316E-04; xsec_unc = 32.93 /100; return; }
    else if (glu_mass == 2680 ) { xsec = 0.308E-04; xsec_unc = 33.12 /100; return; }
    else if (glu_mass == 2685 ) { xsec = 0.300E-04; xsec_unc = 33.32 /100; return; }
    else if (glu_mass == 2690 ) { xsec = 0.293E-04; xsec_unc = 33.52 /100; return; }
    else if (glu_mass == 2695 ) { xsec = 0.285E-04; xsec_unc = 33.71 /100; return; }
    else if (glu_mass == 2700 ) { xsec = 0.278E-04; xsec_unc = 33.92 /100; return; }
    else if (glu_mass == 2705 ) { xsec = 0.271E-04; xsec_unc = 34.13 /100; return; }
    else if (glu_mass == 2710 ) { xsec = 0.265E-04; xsec_unc = 34.34 /100; return; }
    else if (glu_mass == 2715 ) { xsec = 0.258E-04; xsec_unc = 34.56 /100; return; }
    else if (glu_mass == 2720 ) { xsec = 0.251E-04; xsec_unc = 34.77 /100; return; }
    else if (glu_mass == 2725 ) { xsec = 0.245E-04; xsec_unc = 34.99 /100; return; }
    else if (glu_mass == 2730 ) { xsec = 0.239E-04; xsec_unc = 35.22 /100; return; }
    else if (glu_mass == 2735 ) { xsec = 0.233E-04; xsec_unc = 35.44 /100; return; }
    else if (glu_mass == 2740 ) { xsec = 0.227E-04; xsec_unc = 35.67 /100; return; }
    else if (glu_mass == 2745 ) { xsec = 0.221E-04; xsec_unc = 35.89 /100; return; }
    else if (glu_mass == 2750 ) { xsec = 0.216E-04; xsec_unc = 36.12 /100; return; }
    else if (glu_mass == 2755 ) { xsec = 0.211E-04; xsec_unc = 36.35 /100; return; }
    else if (glu_mass == 2760 ) { xsec = 0.205E-04; xsec_unc = 36.59 /100; return; }
    else if (glu_mass == 2765 ) { xsec = 0.200E-04; xsec_unc = 36.82 /100; return; }
    else if (glu_mass == 2770 ) { xsec = 0.195E-04; xsec_unc = 37.05 /100; return; }
    else if (glu_mass == 2775 ) { xsec = 0.190E-04; xsec_unc = 37.29 /100; return; }
    else if (glu_mass == 2780 ) { xsec = 0.185E-04; xsec_unc = 37.53 /100; return; }
    else if (glu_mass == 2785 ) { xsec = 0.181E-04; xsec_unc = 37.76 /100; return; }
    else if (glu_mass == 2790 ) { xsec = 0.176E-04; xsec_unc = 38.0 /100; return; }
    else if (glu_mass == 2795 ) { xsec = 0.172E-04; xsec_unc = 38.24 /100; return; }
    else if (glu_mass == 2800 ) { xsec = 0.168E-04; xsec_unc = 38.48 /100; return; }
    else if (glu_mass == 2805 ) { xsec = 0.163E-04; xsec_unc = 38.72 /100; return; }
    else if (glu_mass == 2810 ) { xsec = 0.159E-04; xsec_unc = 38.95 /100; return; }
    else if (glu_mass == 2815 ) { xsec = 0.155E-04; xsec_unc = 39.19 /100; return; }
    else if (glu_mass == 2820 ) { xsec = 0.151E-04; xsec_unc = 39.43 /100; return; }
    else if (glu_mass == 2825 ) { xsec = 0.148E-04; xsec_unc = 39.66 /100; return; }
    else if (glu_mass == 2830 ) { xsec = 0.144E-04; xsec_unc = 39.9 /100; return; }
    else if (glu_mass == 2835 ) { xsec = 0.140E-04; xsec_unc = 40.14 /100; return; }
    else if (glu_mass == 2840 ) { xsec = 0.137E-04; xsec_unc = 40.38 /100; return; }
    else if (glu_mass == 2845 ) { xsec = 0.133E-04; xsec_unc = 40.62 /100; return; }
    else if (glu_mass == 2850 ) { xsec = 0.130E-04; xsec_unc = 40.86 /100; return; }
    else if (glu_mass == 2855 ) { xsec = 0.127E-04; xsec_unc = 41.1 /100; return; }
    else if (glu_mass == 2860 ) { xsec = 0.124E-04; xsec_unc = 41.34 /100; return; }
    else if (glu_mass == 2865 ) { xsec = 0.121E-04; xsec_unc = 41.59 /100; return; }
    else if (glu_mass == 2870 ) { xsec = 0.118E-04; xsec_unc = 41.83 /100; return; }
    else if (glu_mass == 2875 ) { xsec = 0.115E-04; xsec_unc = 42.07 /100; return; }
    else if (glu_mass == 2880 ) { xsec = 0.112E-04; xsec_unc = 42.32 /100; return; }
    else if (glu_mass == 2885 ) { xsec = 0.109E-04; xsec_unc = 42.56 /100; return; }
    else if (glu_mass == 2890 ) { xsec = 0.106E-04; xsec_unc = 42.81 /100; return; }
    else if (glu_mass == 2895 ) { xsec = 0.104E-04; xsec_unc = 43.05 /100; return; }
    else if (glu_mass == 2900 ) { xsec = 0.101E-04; xsec_unc = 43.3 /100; return; }
    else if (glu_mass == 2905 ) { xsec = 0.986E-05; xsec_unc = 43.55 /100; return; }
    else if (glu_mass == 2910 ) { xsec = 0.961E-05; xsec_unc = 43.79 /100; return; }
    else if (glu_mass == 2915 ) { xsec = 0.937E-05; xsec_unc = 44.04 /100; return; }
    else if (glu_mass == 2920 ) { xsec = 0.914E-05; xsec_unc = 44.29 /100; return; }
    else if (glu_mass == 2925 ) { xsec = 0.891E-05; xsec_unc = 44.54 /100; return; }
    else if (glu_mass == 2930 ) { xsec = 0.869E-05; xsec_unc = 44.79 /100; return; }
    else if (glu_mass == 2935 ) { xsec = 0.848E-05; xsec_unc = 45.04 /100; return; }
    else if (glu_mass == 2940 ) { xsec = 0.827E-05; xsec_unc = 45.29 /100; return; }
    else if (glu_mass == 2945 ) { xsec = 0.806E-05; xsec_unc = 45.54 /100; return; }
    else if (glu_mass == 2950 ) { xsec = 0.786E-05; xsec_unc = 45.8 /100; return; }
    else if (glu_mass == 2955 ) { xsec = 0.767E-05; xsec_unc = 46.05 /100; return; }
    else if (glu_mass == 2960 ) { xsec = 0.748E-05; xsec_unc = 46.3 /100; return; }
    else if (glu_mass == 2965 ) { xsec = 0.729E-05; xsec_unc = 46.56 /100; return; }
    else if (glu_mass == 2970 ) { xsec = 0.711E-05; xsec_unc = 46.81 /100; return; }
    else if (glu_mass == 2975 ) { xsec = 0.694E-05; xsec_unc = 47.07 /100; return; }
    else if (glu_mass == 2980 ) { xsec = 0.677E-05; xsec_unc = 47.32 /100; return; }
    else if (glu_mass == 2985 ) { xsec = 0.660E-05; xsec_unc = 47.58 /100; return; }
    else if (glu_mass == 2990 ) { xsec = 0.644E-05; xsec_unc = 47.84 /100; return; }
    else if (glu_mass == 2995 ) { xsec = 0.628E-05; xsec_unc = 48.09 /100; return; }
    else if (glu_mass == 3000 ) { xsec = 0.612E-05; xsec_unc = 48.35 /100; return; }
    else {xsec = 0.; xsec_unc = 0.;} 
  }

  void higgsinoCrossSection(int hig_mass, double &xsec, double &xsec_unc) {
        if(hig_mass == 127) { xsec = .5824*.5824*7602.2/1000; xsec_unc = 0.0393921; return;}
        else if(hig_mass == 150) { xsec = .5824*.5824*3832.31/1000; xsec_unc = 0.0413612; return;}
        else if(hig_mass == 175) { xsec = .5824*.5824*2267.94/1000; xsec_unc = 0.044299;  return;}
        else if(hig_mass == 200) { xsec = .5824*.5824*1335.62/1000; xsec_unc = 0.0474362; return;}
        else if(hig_mass == 225) { xsec = .5824*.5824*860.597/1000; xsec_unc = 0.0504217; return;}
        else if(hig_mass == 250) { xsec = .5824*.5824*577.314/1000; xsec_unc = 0.0532731; return;}
        else if(hig_mass == 275) { xsec = .5824*.5824*400.107/1000; xsec_unc = 0.0560232; return;}
        else if(hig_mass == 300) { xsec = .5824*.5824*284.855/1000; xsec_unc = 0.0586867; return;}
        else if(hig_mass == 325) { xsec = .5824*.5824* 207.36/1000; xsec_unc = 0.0613554; return;}
        else if(hig_mass == 350) { xsec = .5824*.5824*153.841/1000; xsec_unc = 0.0640598; return;}
        else if(hig_mass == 375) { xsec = .5824*.5824*116.006/1000; xsec_unc = 0.066892;  return;}
        else if(hig_mass == 400) { xsec = .5824*.5824*88.7325/1000; xsec_unc = 0.0697517; return;}
        else if(hig_mass == 425) { xsec = .5824*.5824*68.6963/1000; xsec_unc = 0.0723531; return;}
        else if(hig_mass == 450) { xsec = .5824*.5824*53.7702/1000; xsec_unc = 0.0748325; return;}
        else if(hig_mass == 475) { xsec = .5824*.5824*42.4699/1000; xsec_unc = 0.0775146; return;}
        else if(hig_mass == 500) { xsec = .5824*.5824*33.8387/1000; xsec_unc = 0.0802572; return;}
        else if(hig_mass == 525) { xsec = .5824*.5824*27.1867/1000; xsec_unc = 0.0825803; return;}
        else if(hig_mass == 550) { xsec = .5824*.5824*21.9868/1000; xsec_unc = 0.0849278; return;}
        else if(hig_mass == 575) { xsec = .5824*.5824*17.9062/1000; xsec_unc = 0.087561;  return;}
        else if(hig_mass == 600) { xsec = .5824*.5824*14.6677/1000; xsec_unc = 0.0900693; return;}
        else if(hig_mass == 625) { xsec = .5824*.5824* 12.062/1000; xsec_unc = 0.091959;  return;}
        else if(hig_mass == 650) { xsec = .5824*.5824*9.96406/1000; xsec_unc = 0.094065;  return;}
        else if(hig_mass == 675) { xsec = .5824*.5824*8.28246/1000; xsec_unc = 0.0957436; return;}
        else if(hig_mass == 700) { xsec = .5824*.5824*6.89981/1000; xsec_unc = 0.0982894; return;}
        else if(hig_mass == 725) { xsec = .5824*.5824*5.78355/1000; xsec_unc = 0.0999915; return;}
        else if(hig_mass == 750) { xsec = .5824*.5824* 4.8731/1000; xsec_unc = 0.101211;  return;}
        else if(hig_mass == 775) { xsec = .5824*.5824*4.09781/1000; xsec_unc = 0.104646;  return;}
        else if(hig_mass == 800) { xsec = .5824*.5824*3.46143/1000; xsec_unc = 0.107618;  return;}
        else if(hig_mass == 825) { xsec = .5824*.5824* 2.9337/1000; xsec_unc = 0.108353;  return;}
        else if(hig_mass == 850) { xsec = .5824*.5824* 2.4923/1000; xsec_unc = 0.110016;  return;}
        else if(hig_mass == 875) { xsec = .5824*.5824*2.13679/1000; xsec_unc = 0.112636;  return;}
        else if(hig_mass == 900) { xsec = .5824*.5824*1.80616/1000; xsec_unc = 0.1134;    return;}
        else if(hig_mass == 925) { xsec = .5824*.5824*1.55453/1000; xsec_unc = 0.116949;  return;}
        else if(hig_mass == 950) { xsec = .5824*.5824*1.32692/1000; xsec_unc = 0.117027;  return;}
        else if(hig_mass == 975) { xsec = .5824*.5824*1.12975/1000; xsec_unc = 0.121244;  return;}
        else if(hig_mass ==1000) { xsec = .5824*.5824*0.968853/1000; xsec_unc = 0.126209; return;}
        else{ xsec = 0; xsec_unc = 0;}
  }

  void stopCrossSection(int stop_mass, double &xsec, double &xsec_unc){ // approx. NNLO + NNLL
    if(stop_mass == 100)  { xsec =  0.177e4; xsec_unc = 6.77e-2; return; }
    else if(stop_mass == 125)  { xsec =  0.697e3; xsec_unc = 6.65e-2; return; }
    else if(stop_mass == 150)  { xsec =  0.304e3; xsec_unc = 6.57e-2; return; }
    else if(stop_mass == 175)  { xsec =  0.146e3; xsec_unc = 6.52e-2; return; }
    else if(stop_mass == 200)  { xsec =  0.755e2; xsec_unc = 6.5e-2; return; }
    else if(stop_mass == 225)  { xsec =  0.420e2; xsec_unc = 6.51e-2; return; }
    else if(stop_mass == 250)  { xsec =  0.248e2; xsec_unc = 6.54e-2; return; }
    else if(stop_mass == 275)  { xsec =  0.155e2; xsec_unc = 6.58e-2; return; }
    else if(stop_mass == 300)  { xsec =  0.100e2; xsec_unc = 6.65e-2; return; }
    else if(stop_mass == 325)  { xsec =  0.657e1; xsec_unc = 6.71e-2; return; }
    else if(stop_mass == 350)  { xsec =  0.443e1; xsec_unc = 6.79e-2; return; }
    else if(stop_mass == 375)  { xsec =  0.306e1; xsec_unc = 6.89e-2; return; }
    else if(stop_mass == 400)  { xsec =  0.215e1; xsec_unc = 6.99e-2; return; }
    else if(stop_mass == 425)  { xsec =  0.154e1; xsec_unc = 7.11e-2; return; }
    else if(stop_mass == 450)  { xsec =  0.111e1; xsec_unc = 7.25e-2; return; }
    else if(stop_mass == 475)  { xsec =  0.819; xsec_unc = 7.38e-2; return; }
    else if(stop_mass == 500)  { xsec =  0.609; xsec_unc = 7.53e-2; return; }
    else if(stop_mass == 525)  { xsec =  0.458; xsec_unc = 7.67e-2; return; }
    else if(stop_mass == 550)  { xsec =  0.347; xsec_unc = 7.81e-2; return; }
    else if(stop_mass == 575)  { xsec =  0.266; xsec_unc = 7.96e-2; return; }
    else if(stop_mass == 600)  { xsec =  0.205; xsec_unc = 8.12e-2; return; }
    else if(stop_mass == 625)  { xsec =  0.160; xsec_unc = 8.28e-2; return; }
    else if(stop_mass == 650)  { xsec =  0.125; xsec_unc = 8.45e-2; return; }
    else if(stop_mass == 675)  { xsec =  0.987e-1; xsec_unc = 8.63e-2; return; }
    else if(stop_mass == 700)  { xsec =  0.783e-1; xsec_unc = 8.8e-2; return; }
    else if(stop_mass == 725)  { xsec =  0.624e-1; xsec_unc = 8.98e-2; return; }
    else if(stop_mass == 750)  { xsec =  0.500e-1; xsec_unc = 9.16e-2; return; }
    else if(stop_mass == 885)  { xsec =  0.164e-1; xsec_unc = 10.21e-2; return; }
    else if(stop_mass == 950)  { xsec =  0.991e-2; xsec_unc = 10.76e-2; return; }
    else if(stop_mass == 975)  { xsec =  0.822e-2; xsec_unc = 10.97e-2; return; }
    else if(stop_mass == 1000)  { xsec = 0.683e-2; xsec_unc = 11.2e-2; return; }
    else if(stop_mass == 1030)  { xsec = 0.550e-2; xsec_unc = 11.47e-2; return; }
    else if(stop_mass == 1055)  { xsec = 0.460e-2; xsec_unc = 11.7e-2; return; }
    else if(stop_mass == 1080)  { xsec = 0.385e-2; xsec_unc = 11.93e-2; return; }
    else if(stop_mass == 1125)  { xsec = 0.282e-2; xsec_unc = 12.37e-2; return; }
    else if(stop_mass == 1150)  { xsec = 0.238e-2; xsec_unc = 12.62e-2; return; }
    else if(stop_mass == 1175)  { xsec = 0.201e-2; xsec_unc = 12.87e-2; return; }
    else { xsec = 0.; xsec_unc = 0.; }
  }
}
