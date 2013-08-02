#define FOR(i,l,r) for (int i=(l);i<=(r);i++)

kernel void adder(global int* a, global int* b, global int* result)
{
  int idx = get_global_id(0);
  int x=1,y=2;
  FOR(i,1,1000000) {
    x=x+y;
    y=x+y;
    x=x+y;
    y=x+y;
    x=x+y;
    y=x+y;
    x=x+y;
    y=x+y;
    x=x+y;
    y=x+y;
  }
  result[idx]=x;
}

