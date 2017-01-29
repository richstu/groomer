#include <iostream>
#include <ctime>

#include "baby_base.hpp"

using namespace std;

int main(){ 
  time_t begtime, endtime;
  time(&begtime);
  baby_base b("/cms2r0/aovcharova/data/test.root");
  long nent = b.GetEntries();
  nent = 10;
  for(long entry(0); entry<nent; entry++){
    b.GetEntry(entry);
    b.new_w_btag() = b.w_btag()*100;
    b.new_boo() = 2*entry;
    if (entry%2==0) b.new_vboo() = vector<float>{1.1, 1.2};
    else b.new_vboo() = vector<float>{0.1, 0.2};

    
    for (unsigned i(0); i<b.sys_bctag().size(); i++) b.new_sys_bctag()[i] = b.sys_bctag()[i]-1;

    b.Fill();
  } 
  b.Write();

  cout<<endl;
  
  time(&endtime); 
  cout<<"Time passed: "<<difftime(endtime, begtime)<<" seconds"<<endl<<endl;  

}
