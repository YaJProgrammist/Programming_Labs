#include <iostream>
#include <string>
#include <cmath>
using namespace std;

string decomp(int n) {
  int facNum[4005];
  string ans;
  bool firstFac = false;
  for (int i = 2; i <= n; i++){
    int num = i, k = 2;
    facNum[i] = 0;
    while(k <= sqrt(num)){
      while (num % k == 0) {
        facNum[k]++;
        num /= k;
      }
      k++;
    }
    if (num > 1){
      facNum[num]++;
    }
  }
  for (int i = 2; i <= n; i++){
    if (facNum[i] > 0){
      if (firstFac){
        ans += " * ";
      }
      ans += to_string(i);
      if (facNum[i] > 1){
        ans += '^';
        ans += to_string(facNum[i]);
      }
      firstFac = true;
    }
  }
  return ans;
}

int main(){
  int n;
  cin >> n;
  cout << decomp(n);
  return 0;
}
