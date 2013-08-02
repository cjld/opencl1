#define FOR(i,l,r) for (int i=(l);i<=(r);i++)

kernel void adder(constant int* a, constant int* b, global int* result)
{
  int idx = get_global_id(0);
  int x=1,y=2;
  result[0]=0;
  local int mem;
  barrier(CLK_LOCAL_MEM_FENCE);
  mem=idx;
  FOR(i,1,10) {
    atomic_add(result,idx);
  }
  barrier(CLK_LOCAL_MEM_FENCE);
  result[idx]=mem;
}
