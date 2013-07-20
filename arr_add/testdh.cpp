#include <ctime>
#include <iostream>

using namespace std;

int main() {
  int x=1,y=2;
  long long tt=clock();
  for (int i=0;i<100000000;i++) {
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
  cout<<x<<' '<<y<<endl;
  cout<<clock()-tt<<endl;
}
