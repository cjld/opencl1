#include "ldCLtemplate.h"
#include <ctime>

#define N 2048
#define FOR(i,l,r) for (int i=(l);i<=(r);i++)

int a[N],b[N],c[N];

int main() {
  FOR(i,0,N-1) a[i]=b[i]=i;

  LDCL ldcl;
  ldcl.loadFunc("kernal.cl","adder",cl::NDRange(N));
  ldcl.setArg(a, N*sizeof(a[0]), CL_MEM_READ_ONLY);
  ldcl.setArg(b, N*sizeof(b[0]), CL_MEM_READ_ONLY);
  ldcl.setArg(c, N*sizeof(b[0]), CL_MEM_WRITE_ONLY, LDCL_NEED_READ);
  long long tt=clock();
  ldcl.run();
  cout<<clock()-tt<<endl;
  cout<<ldcl.calcUnit()<<endl;
}
