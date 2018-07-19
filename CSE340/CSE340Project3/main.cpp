//
//  main.cpp
//  cse340_Project3
//
//  Created by Todd Wenker on 10/2/15.

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <ctype.h>
#include "symbols.h"


using namespace std;

//symbol nonTerminalList[10];
vector <symbol *> nonTerminalList;
vector < vector <symbol *> > cfgMatrix;
vector < vector <string> > firstSets;
vector < vector <string> > followSets;

string current_token;
token_type t_type;

int active_token = 0;

int main(int argc, const char * argv[]) {
    
    int task;
    
    if(argc < 2){
        cout << "Missing argument.";
        return 1;
    }
    
    task = atoi(argv[1]);
    
    
    //reading non terminals
    while(t_type != HASH){
        getToken();
        if(t_type != HASH){
            nonTerminalList.push_back(createNonTerminal());
        }
    }

    // TEST: printing non terminal vector
    //printNonTerminals();
    

    
    //filling the cfgMatrix
    while(t_type != DOUBLEHASH){
        
        vector <symbol *> grammarLine;
        getToken();
        
        if(t_type != DOUBLEHASH){
            grammarLine.push_back(createNonTerminal());
        }
        
        while(t_type != HASH && t_type != DOUBLEHASH){
            
            getToken();
            
            if(t_type != HASH && t_type != ARROW && t_type != DOUBLEHASH){
                
                if(isNonTerminal(current_token) == -1){
                    grammarLine.push_back(createTerminal());
                }
                else{
                    grammarLine.push_back(createNonTerminal());
                }
            }
        }
        cfgMatrix.push_back(grammarLine);
    }
    
    //TEST: printing cfg Matrix
    //printCFGMatrix();
    
    switch (task) {
            case 1:
            // Calculate FIRST sets for the input grammar
            // Output the FIRST sets in the exact order and format required
            
            break;
            case 2:
            // Calculate FOLLOW sets for the input grammar
            // Output the FOLLOW sets in the exact order and format required
            
            break;
        default:
            cout << "Error: unrecognized task number " << task << endl;
            break;
    }
    
    return 0;
   
}
/*
int main ()
{
    int c;
    puts ("Enter text. Include a dot ('.') in a sentence to exit:");
    do {
        c=getchar();
        putchar (c);
    } while (c != '.');
    return 0;
}
*/
/*
int main(){
    nonTerminals[0] = *new symbol;
    nonTerminals[0].name = new string;
    *nonTerminals[0].name = "hello";
    if(isNonTerminal(nonTerminals, "hello")){
        cout << "yes" << endl;
    }
    else{
        cout << "no" << endl;
    }
    return 0;
    
}
*/





