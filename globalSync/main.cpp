#include <ldCLtemplate.h>
#include <iostream>
#include <ctime>

using namespace std;

#define N 2048
#define LN 1024
#define FOR(i,l,r) for (int i=(l);i<=(r);i++)

int a[N],b[N];

int main() {
  LDCL ldcl("kernel.cl","main",cl::NDRange(N),cl::NDRange(LN));
  FOR(i,0,N-1) a[i]=1;
  ldcl.setArg(a,N*sizeof(a[0]),CL_MEM_READ_WRITE,LDCL_NEED_READ);
  ldcl.setArg(b,N*sizeof(int),CL_MEM_READ_WRITE);
  long long tt=clock();
  ldcl.run();
  cout<<"Cost Time : "<<clock()-tt<<endl;
  FOR(i,0,N-1) if (a[i]!=a[0]) {
    cout<<"Global Sync ERROR!"<<endl;
    break;
  }
}
