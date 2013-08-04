#include "nbody.h"
#include <iostream>
#include <ctime>

using namespace std;

#define N 200000

Body a[N];

double rdFloat() {
    return ((rand()*1ll<<15)+(rand()*1ll<<30)+(rand()*1ll<<45)+rand()*1ll)%(1ll<<60)*1./(1ll<<60);
}


int main() {
  FOR(i,0,N-1) a[i]=Body(rdFloat(),rdFloat(),1);
  long long tt=clock();
  BVHTree tree(a,N);
  cout<<clock()-tt<<endl;
  FOR(i,0,N-1) {
    Point f=tree.cal(a[i]);
//    FOR(i,0,DIM-1) cout<<f[i]<<' ';
 //   cout<<endl;
  }
  cout<<clock()-tt<<endl;
  cout<<tree.t<<endl;
  cout<<tree.sdep<<endl;
}
