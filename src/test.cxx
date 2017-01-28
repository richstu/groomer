// plot_t5tttt: Compares kinematic distributions of T1tttt and T5tttt

#include <iostream>
#include <ctime>

#include "baby_base.hpp"

using namespace std;

int main(){ 
  time_t begtime, endtime;
  time(&begtime);
  baby_base baby("/cms2r0/aovcharova/data/test.root");
  long nent = baby.GetEntries();
  nent = 10;
  for(long entry(0); entry<nent; entry++){
    baby.GetEntry(entry);
    cout<<baby.w_btag()<<" ";
  } 
  cout<<endl;
  
  time(&endtime); 
  cout<<"Time passed: "<<difftime(endtime, begtime)<<" seconds"<<endl<<endl;  

}
