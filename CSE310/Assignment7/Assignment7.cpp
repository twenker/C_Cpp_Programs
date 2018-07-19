//
//  main.cpp
//  Assignment7
//
//  Created by Todd Wenker on 4/4/15.
//
//  Description: The file that utilizes the Graph and LinkedList classes from the LinkedList header file. 
//

#include <iostream> //to use cin and cout
#include <string> //to use strings
#include <cstdlib> //to use string functions
#include "vector"
#include "LinkedList.h"

using namespace std;

int main() {
    LinkedList adj[15];
    LinkedList *topologicalList = new LinkedList;
    
    int counter = 0;
    cin >> counter;
    cin.ignore(10000, '\n');
    string entry;
    
    Graph dfsGraph = *new Graph();
    
    while(counter > 0){
        getline(cin, entry);
        dfsGraph.insertNode(entry);
        counter--;
    }
    
    cin >> counter;
    cin.ignore(10000, '\n');

    while(counter > 0){
        getline(cin, entry);
        dfsGraph.insertAdj(entry, adj);
        counter--;
    }
    
    dfsGraph.DFS(adj, topologicalList);
    dfsGraph.printPiArray();
    dfsGraph.printDArray();
    dfsGraph.printFArray();
    dfsGraph.printTopological(topologicalList);
   
    
    
    return 0;
    
}
