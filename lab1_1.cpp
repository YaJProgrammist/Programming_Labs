#include <string>
#include <iostream>
using namespace std;

string myStr;

string highestScoringWord(const string &str)
{
  int maxScore = 0, curScore = 0;
  string maxWord = "", curWord = "";
  for (int i = 0; i < str.length(); i++){
    if (str[i] == ' '){
      if (curScore > maxScore){
        maxScore = curScore;
        maxWord = curWord;
      }
      curScore = 0;
      curWord = "";
    } else {
      curScore += str[i] - 'a' + 1;
      curWord += str[i];
    }
  }
  if (curScore > maxScore){
    maxScore = curScore;
    maxWord = curWord;
  }
  return maxWord;
}

int main(){
  getline(cin, myStr);
  cout << highestScoringWord(myStr);
  return 0;
}
