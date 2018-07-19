//
//  LinkedList.h
//  Assignment7
//
//  Created by Todd Wenker on 4/4/15.
//
//  Description: Header file that contains the location structure, the linked list class, and the graph
//  class. The goal of the graph is to conduct a Depth First Search by using the DFS and DFS-View
//  algorithms.
//

#ifndef Assignment7_LinkedList_h
#define Assignment7_LinkedList_h
#include <iostream>
#include <string>
using namespace std;

// a simple structure for the LinkedList class that stores a string and a pointer to the next node
struct location{
    string name;
    location *next;
};

// A linked list of locations that stores a head pointer and has the methods to insert an element and
// print
class LinkedList{
private:
    location * head;
public:
    LinkedList();
    location * getHead();
    void insertElement(location *);
    void printList();
};
//constructor. sets head to null
LinkedList::LinkedList(){
    head = NULL;
}
// returns the linked list head
location * LinkedList::getHead(){
    return head;
}

// inserts an element into the linked list
void LinkedList::insertElement(location * input){
    // if the list is empty, the new element becomes the head
    if(head == NULL){
        head = input;
    }
    // otherwise, the element is placed at the end of the list
    else{
        location * conductor = head;
        while(conductor->next != NULL){
            conductor = conductor->next;
        }
        conductor->next = input;
        
    }
    return;
}

// prints the elements of the linked list
void LinkedList::printList(){
    //iterates through the linked list and prints the name of each node
    location * conductor = head;
    while(conductor != NULL){
        cout << conductor->name << endl;
        conductor = conductor->next;
    }
    return;
}

// the graph class. Has arrays to store the nodes, pi, d, f, and color and the time.
class Graph{
private:
    string nodes[15];
    string pi[15];
    int d[26];
    int f[26];
    int color[26]; // 0 = white, 1 = gray, 2 = black
    int time;
    
public:
    Graph();
    ~Graph();
    void insertNode(string);
    int getIndex(string);
    void insertAdj(string, LinkedList []);
    void DFS(LinkedList [], LinkedList *);
    void DFS_Visit(int, LinkedList [], LinkedList *);
    void printPiArray();
    void printDArray();
    void printFArray();
    void printTopological(LinkedList *);
    void printReverse(location *);
    void printNodes();
    void printAdj(LinkedList []);
    
};
// Graph constructor, initializes everything to null
Graph::Graph(){
    
}
// deconstructor
Graph::~Graph(){
    
}
// recieves a string input and places it in the nodes array sequentially
void Graph::insertNode(string input){
    // iterates through the graph to find the next empty place in the array
    int counter = 0;
    while(nodes[counter].empty() != true){
        counter++;
    }
    nodes[counter] = input;
    return;
}

// searches through nodes array till the string input is found before the index is returned
int Graph::getIndex(string input){
    // iterates through the array till there is a match and the index is returned
    int index = 0;
    while(nodes[index].compare(input) != 0){
        index++;
    }
    return index;
}

//inserts a location object into the adj linkedlist at the correct index
void Graph::insertAdj(string input, LinkedList adj[]){
    string delimiter = "->"; // the character the indicates the split between source and destination
    
    // splits the input between the location source and the location destination
    string source = input.substr(0, input.find(delimiter));
    string destination = input.substr(input.find(delimiter)+2 , input.length());
    //create a location object to be inserted in the adj linkedlist
    location * destLoc = new location;
    destLoc->name = destination;
    
    // finds the index that the source node is stored at in the nodes array
    int index = getIndex(source);
    //stores the destination location object in the proper index in the adj linkedlist
    adj[index].insertElement(destLoc);
    
    return;
}

// Depth First Search that calls DFS_Visit
void Graph::DFS(LinkedList adj[], LinkedList * topologicalList){
    // set the color of all nodes to white and the their pi statistic to NULL
    for(int i = 0; nodes[i].empty() != true; i++){
        color[i] = 0;
        pi[i] = "none";
    }
    // if a color of a node is white, than DFS_Visit is called
    time = 0;
    for(int i = 0; nodes[i].empty() != true; i++){
        if(color[i] == 0){
            DFS_Visit(i, adj, topologicalList);
        }
    }
    return;
}

//when a node is white, this moves through the adjacent nodes
void Graph::DFS_Visit(int index, LinkedList adj[], LinkedList *topologicalList){
    color[index] = 1; // sets the node to gray
    time++;
    d[index] = time; //sets the time the node is discovered
    //the adj linked list is iterated through and each node has DFS_Visit performed on it if it is not
    // discovered. when there are no more adj nodes undiscovered, the color is set to black
    location * conductor = adj[index].getHead();
    while(conductor != NULL){
        // if the color of the adj node is white
        if(color[getIndex(conductor->name)] == 0){
            pi[getIndex(conductor->name)] = nodes[index];
            DFS_Visit(getIndex(conductor->name), adj, topologicalList);
        }
        conductor = conductor->next;
    }
    
    color[index] = 2; // color set to black
    time++;
    f[index] = time;
    location *newLoc = new location;
    newLoc->name = nodes[index];
    topologicalList->insertElement(newLoc);
    return;
}

// prints the contents of the pi array
void Graph::printPiArray(){
    cout << "The array pi content: \n" << endl;
    // iterates thorugh the array and prints
    for(int i = 0; nodes[i].empty() != true; i++){
        cout << "pi[" << nodes[i] << "]= " << pi[i] << endl;
    }
    cout << endl;
    return;
}

//prints the contents of the d array
void Graph::printDArray(){
    cout << "The array d content: \n" << endl;
    // iterates thorugh the array and prints
    for(int i = 0; nodes[i].empty() != true; i++){
        cout << "d[" << nodes[i] << "]=" << d[i] << endl;
    }
    cout << endl;
    return;
}

//prints the contents of the f array
void Graph::printFArray(){
    cout << "The array f content: \n" << endl;
    // iterates thorugh the array and prints
    for(int i = 0; nodes[i].empty() != true; i++){
        cout << "f[" << nodes[i] << "]=" << f[i] << endl;
    }
    cout << endl;
    return;
}

// calls the printReverse method on the topologicalList to print the topological order
void Graph::printTopological(LinkedList * topologicalList){
    cout << "Topological Order: \n" << endl;
    printReverse(topologicalList->getHead());
    return;
}
//the locations are added in reverse order to the topological list so it needs to be flipped
void Graph::printReverse(location * loc){
    if(loc != NULL){
        printReverse(loc->next);
        cout << loc->name << endl;
    }
    return;
}
// used for testing, prints the nodes array
void Graph::printNodes(){
    cout << "Nodes: " << endl;
    for(int i = 0; nodes[i].empty() != true; i++){
        cout << nodes[i] << endl;
    }
    return;
}
//used for testing, prints the adj linkedLists
void Graph::printAdj(LinkedList adj[]){
    cout << "adj list" << endl;
    for(int i = 0; adj[i].getHead() != NULL; i++){
        adj[i].printList();
    }
    return;
}




#endif
