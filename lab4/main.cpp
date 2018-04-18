#include <iostream>
#include <stdlib.h>
#include <cmath>
#include "Stack.h"
using namespace std;

void calculateLastPair(int first, Stack &nums, Stack &operators){
    if (first == 1){
        nums.add(nums.pop() + nums.pop());
    } else
    if (first == -1){
        int num1 = nums.pop(), num2 = nums.pop();
        nums.add(num2 - num1);
    } else
    if (first == 2){
        nums.add(nums.pop() * nums.pop());
    } else
    if (first == -2){
        int num1 = nums.pop(), num2 = nums.pop();
        nums.add(num2 / num1);
    } else
    if (first == 3){
        int num1 = nums.pop(), num2 = nums.pop();
        nums.add(pow(num2, num1));
    }
    operators.pop();
}

int calculate(string str){
    Stack nums, operators;
    bool prevNum = false;
    for (int i = 0; i < str.length(); i++){
        if ((str[i] >= '0' && str[i] <= '9') || (str[i] == '-' && !prevNum)){
            int num;
            if (str[i] == '-'){
                num = -1;
            } else {
                num = str[i] - '0';
            }
            i++;
            while (i < str.length() && str[i]  >= '0' && str[i] <= '9'){
                num = num * 10 + str[i] -'0';
                i++;
            }
            if (i < str.length()){
                i--;
            }
            nums.add(num);
            prevNum = true;
        } else
        if (str[i] != ' '){
            int prior = 0;
            if (str[i] == '+'){
                prior = 1;
            } else
            if (str[i] == '-'){
                prior = -1;
            } else
            if (str[i] == '*'){
                prior = 2;
            } else
            if (str[i] == '/'){
                prior = -2;
            } else
            if (str[i] == '^'){
                prior = 3;
            } else
            if (str[i] == '('){
                prior = 4;
            }
            int first = operators.first();
            while (first != 5 && abs(first) < 3 && abs(first) > abs(prior)){
                calculateLastPair(first, nums, operators);
                first = operators.first();
            }
            if (first == 4 && prior == 0){
                operators.pop();
            } else
            if (prior != 0){
                operators.add(prior);
            }
            prevNum = false;
        }
    }
    int first = operators.first();
    while (first != 5){
        calculateLastPair(first, nums, operators);
        first = operators.first();
    }
    return nums.pop();
}

int main(int argc, char* argv[]){
    string inp;
    if (argc > 1){
        for (int i = 1; i < argc; i++){
            if (argv[i][0] == 'l'){
                inp += '*';
            } else
            {
                inp += argv[i];
            }
        }
    } else {
        getline(cin, inp);
    }
    cout << "Result: " << calculate(inp) << endl;
    //system("pause");
}
