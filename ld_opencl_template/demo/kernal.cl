#define FOR(i,l,r) for (int i=(l);i<=(r);i++)

kernel void adder(constant int* a, constant int* b, global int* result)
{
  int idx = get_global_id(0);
  int x=1,y=2;
  FOR(i,1,100000) {
    atomic_add(result,result[1]);
    atomic_add(result+1,result[0]);
    atomic_add(result,result[1]);
    atomic_add(result+1,result[0]);
    atomic_add(result,result[1]);
    atomic_add(result+1,result[0]);
    atomic_add(result,result[1]);
    atomic_add(result+1,result[0]);
    atomic_add(result,result[1]);
    atomic_add(result+1,result[0]);
  }
}

