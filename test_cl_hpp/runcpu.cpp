#include <iostream>
#include <ctime>

using namespace std;

#define FOR(i,l,r) for (int i=(l);i<=(r);i++)

void adder(int* a,int* b,int* result,int idx)
{
  int x=1,y=2;
  FOR(i,1,100000) {
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

int res[100000];

int main() {
  long long tt=clock();
  FOR(i,1,2048) adder(res,res,res,i);
  cout<<clock()-tt<<endl;
}
