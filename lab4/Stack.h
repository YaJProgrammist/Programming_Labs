struct Node{
    int val;
    Node *next;
};

class Stack{
public:
    Node *head;
    Stack(): head(NULL){};
    ~Stack();
    void add(int x);
    int first();
    int pop();
    void write();
};
