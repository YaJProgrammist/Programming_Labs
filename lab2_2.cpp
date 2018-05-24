#include <cmath>
#include <iostream>

int sum_of_squares(int n) {
  int i = sqrt(n), ans = 4, mul4 = 0;
  if (i*i == n){
    return 1;
  }
  while (n % 4 == 0){
    mul4++;
    n /= 4;
  }
  if ((n - 7) % 8 == 0){
    return 4;
  }
  i = 3;
  while (i <= n){
    int s = 0;
    while (n % i == 0){
        s++;
        n /= i;
    }
    if ((i - 3) % 4 == 0 && s % 2 == 1){
      return 3;
    }
    i += 4;
  }
  return 2;
}

int main(){
  std::cout << sum_of_squares(19);
  return 0;
}
