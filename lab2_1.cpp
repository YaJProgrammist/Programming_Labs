#include <algorithm>
#include <vector>
#include <iostream>

class Mix
{
public:
  struct strct;
  static bool comp(strct a, strct b);
  static std::string mix(const std::string &s1, const std::string &s2);
};

struct Mix::strct{
    int f, s, t;
};

bool Mix::comp(strct a, strct b){
  if (a.f < b.f){
    return true;
  }
  if (a.f > b.f){
    return false;
  }
  if (a.t > b.t){
    return true;
  }
  if (a.t < b.t){
    return false;
  }
  if (a.s > b.s){
    return true;
  }
  return false;
}

std::string Mix::mix(const std::string &s1, const std::string &s2){
  std::vector<strct> p;
  int num1[26] = {0}, num2[26] = {0};
  std::string ans;
  for (int i = 0; i < s1.length(); i++){
    int number = s1[i] - 97;
    if (number >= 0 && number < 26){
      num1[number]++;
    }
  }
  for (int i = 0; i < s2.length(); i++){
    int number = s2[i] - 97;
    if (number >= 0 && number < 26){
      num2[number]++;
    }
  }
  for (int i = 0; i < 26; i++){
    if (num1[i] >= num2[i]){
      if (num1[i] > 1){
        strct newStrct = {num1[i], i};
        if (num1[i] > num2[i]){
          newStrct.t = 1;
        } else {
          newStrct.t = 3;
        }
        p.push_back(newStrct);
      }
    } else {
      if (num2[i] > 1){
        strct newStrct = {num2[i], i, 2};
        p.push_back(newStrct);
      }
    }
  }
  std::sort(p.begin(), p.end(), comp);
  for (int i = p.size() - 1; i >= 0; i--){
    if (i < p.size() - 1){
      ans += '/';
    }
    if (p[i].t == 3){
      ans += "=:";
    } else {
      ans += p[i].t + '0';
      ans += ':';
    }
    char ch = p[i].s + 'a';
    for (int j = 0; j < p[i].f; j++){
      ans += ch;
    }
  }
  return ans;
}

int main(){
  Mix myMix;
  std::string str = myMix.mix("a aaa bbb c d", "A aaaa bb c");
  std::cout << str;
  return 0;
}
