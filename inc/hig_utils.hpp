//----------------------------------------------------------------------------
// utilities - Various functions used accross the code
//----------------------------------------------------------------------------

#ifndef H_HIG_UTILS
#define H_HIG_UTILS

#include <cstddef>
#include <cstdio>
#include <cmath>

#include <iostream>
#include <string>
#include <vector>
#include <set>

#include <unistd.h>

#include "TString.h"
#include "TTree.h"
#include "TGraph.h"

#include "baby_plus.hpp"

namespace hig_utils{

  int mchi(baby_plus &b);
  float eff_higtrig(baby_plus &b);
  float effunc_higtrig(baby_plus &b);

}

#endif
