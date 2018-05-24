#include <set>
#include <iterator>
#include <iostream>
using namespace std;

class DoubleLinear
{
public:
    static int dblLinear(int n);
};

int DoubleLinear::dblLinear(int n){
  int cntng = 0;
  set<int> mySet;
  mySet.insert(1);
  set<int>::iterator it2 = mySet.begin(), it3 = mySet.begin();
  while (cntng < n){
    int i2 = *it2 * 2 + 1, i3 = *it3 * 3 + 1;
    if (i2 < i3){
      mySet.insert(i2);
      it2++;
    } else
    if (i2 > i3){
      mySet.insert(i3);
      it3++;
    } else {
      mySet.insert(i2);
      it2++;
      it3++;
    }
    cntng++;
  }
  return *(--mySet.end());
}

int main(){
  DoubleLinear myDoubleLinear;
  cout << myDoubleLinear.dblLinear(1000) << endl;
  return 0;
}
