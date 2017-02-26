//----------------------------------------------------------------------------
// utilities - Various functions used accross the code
//----------------------------------------------------------------------------

#include "hig_utils.hpp"

#include <cmath>

#include <deque>
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <iomanip>   // setw

#include <libgen.h>

#include "TCollection.h"
#include "TFile.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TList.h"
#include "TString.h"
#include "TSystemDirectory.h"
#include "TSystemFile.h"
#include "TSystem.h"
#include "TTree.h"
#include "TChain.h"
#include "TRegexp.h"

using namespace std;

namespace hig_utils{

  int mchi(baby_plus &b){
    int mass = -999;
    for (unsigned i(0); i<b.mc_mass().size(); i++){
      if (b.mc_id().at(i)==1000023) {
        mass = floor(b.mc_mass().at(i)+0.1);
        break;
      }
    }
    return mass;
  }


  float eff_higtrig(baby_plus &b){
      float errup(0), errdown(0); // Not used, but for reference
      errup+=errdown;
      float eff = 1., met = b.met(), ht = b.ht();

      if(ht>   0 && ht<= 200 && met> 150 && met<= 155) {eff = 0.490; errup = 0.011; errdown = 0.011;}
      else if(ht> 200 && ht<= 600 && met> 150 && met<= 155) {eff = 0.563; errup = 0.004; errdown = 0.004;}
      else if(ht> 600 && ht<= 800 && met> 150 && met<= 155) {eff = 0.529; errup = 0.016; errdown = 0.016;}
      else if(ht> 800 && ht<=1000 && met> 150 && met<= 155) {eff = 0.454; errup = 0.027; errdown = 0.027;}
      else if(ht>1000 && ht<=9999 && met> 150 && met<= 155) {eff = 0.493; errup = 0.031; errdown = 0.031;}
      else if(ht>   0 && ht<= 200 && met> 155 && met<= 160) {eff = 0.564; errup = 0.013; errdown = 0.013;}
      else if(ht> 200 && ht<= 600 && met> 155 && met<= 160) {eff = 0.628; errup = 0.004; errdown = 0.004;}
      else if(ht> 600 && ht<= 800 && met> 155 && met<= 160) {eff = 0.577; errup = 0.015; errdown = 0.015;}
      else if(ht> 800 && ht<=1000 && met> 155 && met<= 160) {eff = 0.475; errup = 0.026; errdown = 0.026;}
      else if(ht>1000 && ht<=9999 && met> 155 && met<= 160) {eff = 0.468; errup = 0.032; errdown = 0.032;}
      else if(ht>   0 && ht<= 200 && met> 160 && met<= 165) {eff = 0.579; errup = 0.014; errdown = 0.014;}
      else if(ht> 200 && ht<= 600 && met> 160 && met<= 165) {eff = 0.673; errup = 0.005; errdown = 0.005;}
      else if(ht> 600 && ht<= 800 && met> 160 && met<= 165) {eff = 0.626; errup = 0.016; errdown = 0.016;}
      else if(ht> 800 && ht<=1000 && met> 160 && met<= 165) {eff = 0.616; errup = 0.027; errdown = 0.027;}
      else if(ht>1000 && ht<=9999 && met> 160 && met<= 165) {eff = 0.498; errup = 0.031; errdown = 0.031;}
      else if(ht>   0 && ht<= 200 && met> 165 && met<= 170) {eff = 0.632; errup = 0.016; errdown = 0.016;}
      else if(ht> 200 && ht<= 600 && met> 165 && met<= 170) {eff = 0.717; errup = 0.005; errdown = 0.005;}
      else if(ht> 600 && ht<= 800 && met> 165 && met<= 170) {eff = 0.629; errup = 0.016; errdown = 0.016;}
      else if(ht> 800 && ht<=1000 && met> 165 && met<= 170) {eff = 0.582; errup = 0.027; errdown = 0.027;}
      else if(ht>1000 && ht<=9999 && met> 165 && met<= 170) {eff = 0.539; errup = 0.037; errdown = 0.037;}
      else if(ht>   0 && ht<= 200 && met> 170 && met<= 175) {eff = 0.629; errup = 0.017; errdown = 0.018;}
      else if(ht> 200 && ht<= 600 && met> 170 && met<= 175) {eff = 0.758; errup = 0.005; errdown = 0.005;}
      else if(ht> 600 && ht<= 800 && met> 170 && met<= 175) {eff = 0.686; errup = 0.016; errdown = 0.017;}
      else if(ht> 800 && ht<=1000 && met> 170 && met<= 175) {eff = 0.665; errup = 0.027; errdown = 0.028;}
      else if(ht>1000 && ht<=9999 && met> 170 && met<= 175) {eff = 0.593; errup = 0.034; errdown = 0.035;}
      else if(ht>   0 && ht<= 200 && met> 175 && met<= 180) {eff = 0.676; errup = 0.019; errdown = 0.019;}
      else if(ht> 200 && ht<= 600 && met> 175 && met<= 180) {eff = 0.795; errup = 0.005; errdown = 0.005;}
      else if(ht> 600 && ht<= 800 && met> 175 && met<= 180) {eff = 0.713; errup = 0.016; errdown = 0.017;}
      else if(ht> 800 && ht<=1000 && met> 175 && met<= 180) {eff = 0.740; errup = 0.026; errdown = 0.028;}
      else if(ht>1000 && ht<=9999 && met> 175 && met<= 180) {eff = 0.636; errup = 0.035; errdown = 0.036;}
      else if(ht>   0 && ht<= 200 && met> 180 && met<= 185) {eff = 0.707; errup = 0.021; errdown = 0.022;}
      else if(ht> 200 && ht<= 600 && met> 180 && met<= 185) {eff = 0.823; errup = 0.005; errdown = 0.005;}
      else if(ht> 600 && ht<= 800 && met> 180 && met<= 185) {eff = 0.788; errup = 0.015; errdown = 0.016;}
      else if(ht> 800 && ht<=1000 && met> 180 && met<= 185) {eff = 0.731; errup = 0.028; errdown = 0.030;}
      else if(ht>1000 && ht<=9999 && met> 180 && met<= 185) {eff = 0.678; errup = 0.033; errdown = 0.035;}
      else if(ht>   0 && ht<= 200 && met> 185 && met<= 190) {eff = 0.750; errup = 0.022; errdown = 0.024;}
      else if(ht> 200 && ht<= 600 && met> 185 && met<= 190) {eff = 0.858; errup = 0.004; errdown = 0.004;}
      else if(ht> 600 && ht<= 800 && met> 185 && met<= 190) {eff = 0.788; errup = 0.015; errdown = 0.016;}
      else if(ht> 800 && ht<=1000 && met> 185 && met<= 190) {eff = 0.759; errup = 0.028; errdown = 0.030;}
      else if(ht>1000 && ht<=9999 && met> 185 && met<= 190) {eff = 0.735; errup = 0.031; errdown = 0.033;}
      else if(ht>   0 && ht<= 200 && met> 190 && met<= 195) {eff = 0.757; errup = 0.025; errdown = 0.027;}
      else if(ht> 200 && ht<= 600 && met> 190 && met<= 195) {eff = 0.862; errup = 0.005; errdown = 0.005;}
      else if(ht> 600 && ht<= 800 && met> 190 && met<= 195) {eff = 0.802; errup = 0.015; errdown = 0.016;}
      else if(ht> 800 && ht<=1000 && met> 190 && met<= 195) {eff = 0.769; errup = 0.028; errdown = 0.030;}
      else if(ht>1000 && ht<=9999 && met> 190 && met<= 195) {eff = 0.649; errup = 0.037; errdown = 0.039;}
      else if(ht>   0 && ht<= 200 && met> 195 && met<= 200) {eff = 0.723; errup = 0.030; errdown = 0.032;}
      else if(ht> 200 && ht<= 600 && met> 195 && met<= 200) {eff = 0.890; errup = 0.004; errdown = 0.005;}
      else if(ht> 600 && ht<= 800 && met> 195 && met<= 200) {eff = 0.861; errup = 0.014; errdown = 0.015;}
      else if(ht> 800 && ht<=1000 && met> 195 && met<= 200) {eff = 0.867; errup = 0.023; errdown = 0.026;}
      else if(ht>1000 && ht<=9999 && met> 195 && met<= 200) {eff = 0.780; errup = 0.031; errdown = 0.035;}
      else if(ht>   0 && ht<= 200 && met> 200 && met<= 210) {eff = 0.805; errup = 0.021; errdown = 0.023;}
      else if(ht> 200 && ht<= 600 && met> 200 && met<= 210) {eff = 0.911; errup = 0.003; errdown = 0.003;}
      else if(ht> 600 && ht<= 800 && met> 200 && met<= 210) {eff = 0.865; errup = 0.010; errdown = 0.011;}
      else if(ht> 800 && ht<=1000 && met> 200 && met<= 210) {eff = 0.842; errup = 0.017; errdown = 0.019;}
      else if(ht>1000 && ht<=9999 && met> 200 && met<= 210) {eff = 0.744; errup = 0.024; errdown = 0.025;}
      else if(ht>   0 && ht<= 200 && met> 210 && met<= 220) {eff = 0.811; errup = 0.026; errdown = 0.029;}
      else if(ht> 200 && ht<= 600 && met> 210 && met<= 220) {eff = 0.936; errup = 0.003; errdown = 0.003;}
      else if(ht> 600 && ht<= 800 && met> 210 && met<= 220) {eff = 0.894; errup = 0.010; errdown = 0.011;}
      else if(ht> 800 && ht<=1000 && met> 210 && met<= 220) {eff = 0.889; errup = 0.017; errdown = 0.019;}
      else if(ht>1000 && ht<=9999 && met> 210 && met<= 220) {eff = 0.832; errup = 0.020; errdown = 0.022;}
      else if(ht>   0 && ht<= 200 && met> 220 && met<= 230) {eff = 0.815; errup = 0.031; errdown = 0.035;}
      else if(ht> 200 && ht<= 600 && met> 220 && met<= 230) {eff = 0.948; errup = 0.003; errdown = 0.003;}
      else if(ht> 600 && ht<= 800 && met> 220 && met<= 230) {eff = 0.922; errup = 0.009; errdown = 0.010;}
      else if(ht> 800 && ht<=1000 && met> 220 && met<= 230) {eff = 0.929; errup = 0.013; errdown = 0.016;}
      else if(ht>1000 && ht<=9999 && met> 220 && met<= 230) {eff = 0.880; errup = 0.019; errdown = 0.022;}
      else if(ht>   0 && ht<= 200 && met> 230 && met<= 240) {eff = 0.810; errup = 0.040; errdown = 0.046;}
      else if(ht> 200 && ht<= 600 && met> 230 && met<= 240) {eff = 0.961; errup = 0.003; errdown = 0.003;}
      else if(ht> 600 && ht<= 800 && met> 230 && met<= 240) {eff = 0.948; errup = 0.008; errdown = 0.009;}
      else if(ht> 800 && ht<=1000 && met> 230 && met<= 240) {eff = 0.949; errup = 0.012; errdown = 0.016;}
      else if(ht>1000 && ht<=9999 && met> 230 && met<= 240) {eff = 0.875; errup = 0.022; errdown = 0.026;}
      else if(ht>   0 && ht<= 200 && met> 240 && met<= 250) {eff = 0.803; errup = 0.047; errdown = 0.056;}
      else if(ht> 200 && ht<= 600 && met> 240 && met<= 250) {eff = 0.970; errup = 0.003; errdown = 0.003;}
      else if(ht> 600 && ht<= 800 && met> 240 && met<= 250) {eff = 0.958; errup = 0.007; errdown = 0.009;}
      else if(ht> 800 && ht<=1000 && met> 240 && met<= 250) {eff = 0.940; errup = 0.015; errdown = 0.019;}
      else if(ht>1000 && ht<=9999 && met> 240 && met<= 250) {eff = 0.875; errup = 0.022; errdown = 0.026;}
      else if(ht>   0 && ht<= 200 && met> 250 && met<= 275) {eff = 0.873; errup = 0.032; errdown = 0.040;}
      else if(ht> 200 && ht<= 600 && met> 250 && met<= 275) {eff = 0.977; errup = 0.002; errdown = 0.002;}
      else if(ht> 600 && ht<= 800 && met> 250 && met<= 275) {eff = 0.972; errup = 0.004; errdown = 0.005;}
      else if(ht> 800 && ht<=1000 && met> 250 && met<= 275) {eff = 0.965; errup = 0.008; errdown = 0.009;}
      else if(ht>1000 && ht<=9999 && met> 250 && met<= 275) {eff = 0.910; errup = 0.014; errdown = 0.015;}
      else if(ht>   0 && ht<= 200 && met> 275 && met<= 300) {eff = 0.872; errup = 0.049; errdown = 0.068;}
      else if(ht> 200 && ht<= 600 && met> 275 && met<= 300) {eff = 0.986; errup = 0.002; errdown = 0.002;}
      else if(ht> 600 && ht<= 800 && met> 275 && met<= 300) {eff = 0.975; errup = 0.005; errdown = 0.006;}
      else if(ht> 800 && ht<=1000 && met> 275 && met<= 300) {eff = 0.964; errup = 0.009; errdown = 0.011;}
      else if(ht>1000 && ht<=9999 && met> 275 && met<= 300) {eff = 0.950; errup = 0.011; errdown = 0.013;}
      else if(ht>   0 && ht<= 200 && met> 300 && met<=9999) {eff = 0.824; errup = 0.055; errdown = 0.070;}
      else if(ht> 200 && ht<= 600 && met> 300 && met<=9999) {eff = 0.989; errup = 0.001; errdown = 0.001;}
      else if(ht> 600 && ht<= 800 && met> 300 && met<=9999) {eff = 0.995; errup = 0.001; errdown = 0.001;}
      else if(ht> 800 && ht<=1000 && met> 300 && met<=9999) {eff = 0.991; errup = 0.002; errdown = 0.003;}
      else if(ht>1000 && ht<=9999 && met> 300 && met<=9999) {eff = 0.970; errup = 0.004; errdown = 0.004;}
        
      return eff;
  }
}