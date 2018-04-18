#include <iostream>
#include "Stack.h"

using namespace std;

Stack::~Stack(){
    Node *temp = new Node;
    while (head){
        temp = head -> next;
        delete head;
        head = temp;
    }
}

void Stack::add(int x){
    Node *temp = new Node;
    temp -> val = x;
    temp -> next = head;
    head = temp;
};

int Stack::first(){
    if (!head){
        return 5;
    }
    return head -> val;
}

int Stack::pop(){
    int ret = head -> val;
    Node *temp = head -> next;
    delete head;
    head = temp;
    return ret;
}

void Stack::write(){
    Node *temp = head;
    while (temp){
        cout << temp -> val << ' ';
        temp = temp -> next;
    }
    cout << endl;
}
