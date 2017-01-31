#include <iostream>
#include <ctime>

#include "baby_base.hpp"

using namespace std;

int main(){ 
  time_t begtime, endtime;
  time(&begtime);
  baby_base b("/cms2r0/aovcharova/data/test.root", "test_renorm.root");
  long nent = b.GetEntries();
  nent = 10;
  for(long entry(0); entry<nent; entry++){
    b.GetEntry(entry);
    b.out_w_btag() = b.w_btag()*100;
    b.out_boo() = 2*entry;
    if (entry%2==0) b.out_vboo() = vector<float>{1.1, 1.2};
    else b.out_vboo() = vector<float>{0.1, 0.2};
    for (unsigned i(0); i<b.sys_bctag().size(); i++) b.out_sys_bctag()[i] = b.sys_bctag()[i]-1;
    b.Fill();
  } 
  b.Write();

  bool doSumOutputTree = true;
  baby_base bsum("/cms2r0/aovcharova/data/test.root", "sum_tree.root", doSumOutputTree);
  long nent_sum = bsum.GetEntries();
  nent_sum = 10;
  for(long entry(0); entry<nent_sum; entry++){
    bsum.GetEntry(entry);
    cout<<"this = "<<bsum.w_btag()<<" out = "<<bsum.out_w_btag()<<endl;
    bsum.out_w_btag() += bsum.w_btag();
    for (unsigned i(0); i<bsum.sys_bctag().size(); i++) bsum.out_sys_bctag()[i] += bsum.sys_bctag()[i];
    bsum.out_nevents()++;
  } 
  bsum.Fill();
  bsum.Write();

  cout<<endl;
  
  time(&endtime); 
  cout<<"Time passed: "<<difftime(endtime, begtime)<<" seconds"<<endl<<endl;  

}
