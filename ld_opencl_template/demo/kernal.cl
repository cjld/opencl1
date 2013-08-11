#define FOR(i,l,r) for (int i=(l);i<=(r);i++)

kernel void adder(constant int* a, constant int* b, global int* result)
{
  int idx = get_global_id(0);
  int x=1,y=idx;
  FOR(i,1,10000000) {
    x+=y;
    y+=x;
    x+=y;
    y+=x;
    x+=y;
    y+=x;
    x+=y;
    y+=x;
    x+=y;
    y+=x;
  }
  result[idx]=x;
}

