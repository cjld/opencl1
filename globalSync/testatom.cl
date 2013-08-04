#define FOR(i,l,r) for (int i=(l);i<=(r);i++)

kernel void main(global int *a, global int *b) {
  int i=get_global_id(0);
 // i=0;
  FOR(t,1,1000000) {
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
