#include "../ldLIb/ldLib.cl"

kernel void main(global int *a, global int *b, global int *c) {
  int i=get_global_id(0);
 // i=0;
  FOR(t,1,10000) {
    /*
    a[i]+=b[i];
    b[i]+=a[i];
    a[i]+=b[i];
    b[i]+=a[i];
    a[i]+=b[i];
    b[i]+=a[i];
    a[i]+=b[i];
    b[i]+=a[i];
    a[i]+=b[i];
    b[i]+=a[i];
    */
    
    atomic_add(a+i,b[i]);
    atomic_add(b+i,a[i]);
    atomic_add(a+i,b[i]);
    atomic_add(b+i,a[i]);
    atomic_add(a+i,b[i]);
    atomic_add(b+i,a[i]);
    atomic_add(a+i,b[i]);
    atomic_add(b+i,a[i]);
    atomic_add(a+i,b[i]);
    atomic_add(b+i,a[i]);
    
  }
}
