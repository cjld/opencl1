#define N 2048
#define FOR(i,l,r) for (int i=(l);i<=(r);i++)

void globalSync0(volatile global int *s) {
  barrier(CLK_GLOBAL_MEM_FENCE | CLK_LOCAL_MEM_FENCE);
}

void globalSync1(volatile global int *s) {
  atomic_inc(s);
  while (*s!=get_global_size(0));
  if (get_global_id(0)==0) atomic_sub(s,get_global_size(0));
}

void globalSync2(volatile global int *s) {
  if (get_local_id(0)==0) {
    atomic_inc(s);
    while (*s!=get_num_groups(0));
  };
  barrier(CLK_GLOBAL_MEM_FENCE | CLK_LOCAL_MEM_FENCE);
  if (get_global_id(0)==0) atomic_sub(s,get_num_groups(0));
}

void globalSync3(volatile global int *s) {
  const int lid=get_local_id(0);
  const int gid=get_group_id(0);
  if (!lid) s[gid]=1;
  if (!gid) {
    if (lid<get_num_groups(0))
      while (!s[lid]);
    barrier(CLK_GLOBAL_MEM_FENCE | CLK_LOCAL_MEM_FENCE);
    s[lid]=0;
  }
  while (!lid && s[gid]);
  barrier(CLK_GLOBAL_MEM_FENCE | CLK_LOCAL_MEM_FENCE);
}

kernel void main(volatile global int *a, global int *flags) {
  int i=get_global_id(0);
  
  FOR(j,0,500000) {
    int temp=a[i]+a[(i+1)&(N-1)]+a[(i+2)&(N-1)];
    globalSync3(flags);
    a[i]=temp;
    globalSync3(flags);
  }
}
