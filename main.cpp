//  Copyright © 2019 Andrew Lorber. All rights reserved.
//
// This is DSA 1 Project 1 - An implementation of stacks and queues using a SimpleList abstract base class
// -std=c++11

#include <iostream>
#include <string>
#include <list>
#include <fstream>

using namespace std;

//Implements a SimpleList class Template
template <typename T>
class SimpleList {
    string name;
    class Node;
protected:
    Node* head = nullptr;
    Node* tail = nullptr;
    void insertStart(T& newT);
    void insertEnd(T& newT);
    T removeStart();
public:
    string getName(){ return name; }
    void setName(string& str){ name = str; }
    virtual void push(T& newT) = 0;
    virtual T pop() = 0;
    bool isEmpty(){ return (head == nullptr) ? true : false; };   //Checks if list is empty by checking if head points to a nullptr
};

//Defines the Node class in the SimpleList class
template <typename T>
class SimpleList<T>::Node {
public:
    T data; //Data in node
    Node* next;
    Node(T& d = T {}, Node* n = nullptr) : data{d}, next{n} {}
};

//Defines the SimpleList insertStart Method
template <typename T>
void SimpleList<T>::insertStart(T& newT) {
    //If the list is empty, makes the new node head and tail
    if (!head) {
        tail = head = new Node(newT);
    } else {
        head = new Node(newT, head);
    }
}

//Defines the SimpleList insertEnd Method
template <typename T>
void SimpleList<T>::insertEnd(T& newT) {
    //If the list is empty, makes the new node head and tail
    if(!tail){
        head = tail = new Node(newT);
    }
    else {
        tail = tail -> next = new Node(newT);
    }
}

//Defines the SimpleList removeStart Method
template <typename T>
T SimpleList<T>::removeStart(){
    T retData = (head -> data);   //Store data to return
    Node* currentNode = head;   //Stores current node for deletion
    head = (head -> next);
    delete currentNode;
    return retData;
}

//Implements a Stack - LIFO so add to start and remove from start
template <typename T>
class Stack : public SimpleList<T> {
public:
    Stack(string sName){ SimpleList<T>::setName(sName); }
    virtual void push(T& newT){ SimpleList<T>::insertStart(newT); }
    virtual T pop(){ return SimpleList<T>::removeStart(); }
};

//Implements a Queue - FIFO so add to end and remove from start
template <typename T>
class Queue : public SimpleList<T> {
public:
    Queue(string qName){ SimpleList<T>::setName(qName); }
    virtual void push(T& newT){ SimpleList<T>::insertEnd(newT); }
    virtual T pop(){ return SimpleList<T>::removeStart(); }
};

//Defines the function to search if stack/queue already exists and returns a pointer to it (or nullptr)
template <typename T>
SimpleList<T>* getloc (list<SimpleList<T> *>& lists, string name) {
    for(typename list<SimpleList<T> *>::iterator lIterator = lists.begin(); lIterator != lists.end(); ++lIterator){
        if ((*lIterator) -> getName() == name) {
            return *lIterator;
        }
    }
    return nullptr;
}

//Defines the function that runs the commands in the input file and writes response to output file
void runCommands(string inputf, string outputf){
    list<SimpleList<int> *> listSLi; // all integer stacks and queues
    list<SimpleList<double> *> listSLd; // all double stacks and queues
    list<SimpleList<string> *> listSLs; // all string stacks and queues
    //Opens files
    ifstream inFile(inputf);
    ofstream  outFile(outputf);
    //Variables to split the command into
    string command;
    string action;
    string lName;
    string lType;
    string value;
    
    //Reads in each line from the input file
    while(getline(inFile, command)){
        outFile << "PROCESSING COMMAND: " << command << '\n';
        
        //Parses Command
        action = command.substr(0, command.find(" "));
        command = command.substr(command.find(" ") + 1);
        lName = command.substr(0, command.find(" "));
        lType = lName.substr(0,1);
        command = command.substr(command.find(" ") + 1);
        value = command.substr(0, command.find(" "));
        
        if(action == "create"){
            if (lType == "i") {
                if (getloc(listSLi, lName)) {
                    outFile << "ERROR: This name already exists!\n";
                } else {
                    SimpleList<int>* psLi;
                    if (value == "stack") {
                        psLi = new Stack<int>(lName);
                    } else /* Queue */{
                        psLi = new Queue<int>(lName);
                    }
                    listSLi.push_front(psLi);
                }
            } else if (lType == "d") {
                if (getloc(listSLd, lName)) {
                    outFile << "ERROR: This name already exists!\n";
                } else {
                    SimpleList<double>* psLd;
                    if (value == "stack") {
                        psLd = new Stack<double>(lName);
                    } else /* Queue */{
                        psLd = new Queue<double>(lName);
                    }
                    listSLd.push_front(psLd);
                }
            } else /* "s" */ {
                if (getloc(listSLs, lName)) {
                    outFile << "ERROR: This name already exists!\n";
                } else {
                    SimpleList<string>* psLs;
                    if (value == "stack") {
                        psLs = new Stack<string>(lName);
                    } else /* Queue */{
                        psLs = new Queue<string>(lName);
                    }
                    listSLs.push_front(psLs);
                }
            }
        } else if(action == "push"){
            if (lType == "i") {
                if (SimpleList<int>* lPointer = getloc(listSLi, lName)) {
                    int tmp = stoi(value);
                    lPointer -> push(tmp);
                } else {
                    outFile << "ERROR: This name does not exist!\n";
                }
            } else if (lType == "d") {
                if (SimpleList<double>* lPointer = getloc(listSLd, lName)) {
                    double tmp = stod(value);
                    lPointer -> push(tmp);
                } else {
                    outFile << "ERROR: This name does not exist!\n";
                }
            } else /* "s" */ {
                if (SimpleList<string>* lPointer = getloc(listSLs, lName)) {
                    lPointer -> push(value);
                } else {
                    outFile << "ERROR: This name does not exist!\n";
                }
            }
        } else /* pop */ {
            if (lType == "i") {
                if (SimpleList<int>* lPointer = getloc(listSLi, lName)) {
                    if (lPointer -> isEmpty()) {
                        outFile << "ERROR: This list is empty!\n";
                    } else {
                        int i = lPointer -> pop();
                        outFile << "Value popped: " << i << '\n';
                    }
                } else {
                    outFile << "ERROR: This name does not exist!\n";
                }
            } else if (lType == "d") {
                if (SimpleList<double>* lPointer = getloc(listSLd, lName)) {
                    if(lPointer -> isEmpty()){
                        outFile << "ERROR: This list is empty!\n";
                    } else {
                        double d = lPointer -> pop();
                        outFile << "Value popped: " << d << '\n';
                    }
                } else {
                    outFile << "ERROR: This name does not exist!\n";
                }
            } else /* "s" */ {
                if (SimpleList<string>* lPointer = getloc(listSLs, lName)) {
                    if(lPointer -> isEmpty()){
                        outFile << "ERROR: This list is empty!\n";
                    } else {
                        string s = lPointer -> pop();
                        outFile << "Value popped: " << s << '\n';
                    }
                } else {
                    outFile << "ERROR: This name does not exist!\n";
                }
            }
        }
    }
    outFile.close();
}

//Asks user for filenames and runs command executing function
int main(int argc, const char * argv[]) {
    
    string inf;
    string outf;
    cout << "Enter name of input file: ";
    cin >> inf;
    cout << "Enter name of output file: ";
    cin >> outf;
    runCommands(inf, outf);
    
    return 0;
}
