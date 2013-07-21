__kernel void adder(__global const int* a, __global const int* b, __global int* result)
{
  int idx = get_global_id(0);
  int x=idx,y=2*idx;
  for (int i=0;i<=10000000;i++) {
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
