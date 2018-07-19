//
//  Assignment5.cpp
//  
//  DESCRIPTION:
//
//
//

#include <iostream>
#include "Assignment5_Hash.h"
#include <string>
using namespace std;


int main() {
    hashTable* hTable;
    hTable = new hashTable;
    string s1;
    
    // sets the size of the hash table
    cout << "Please enter a hash table size: " << endl;
    cin >> hTable->tableSize;
    cin.ignore(10000, '\n');
    getline(cin, s1);
    // insert Courses into hash table
    while(s1 != "Search"){
        if(s1 != "Insertion" && s1 != "Search"){
            hashInsertCourse(s1, hTable);
        }
        getline(cin, s1);
    }
    // display the hash table after insertion
    displayHashTable(*hTable);
    
    // search for Courses in hash table
    while(s1 != "Delete"){
        if(s1 != "Delete" && s1 != "Search"){
            searchHashTable(s1, *hTable);
        }
        getline(cin, s1);
    }
    // delete Courses in hash table
    while(s1 != "Display"){
        if(s1 != "Display" && s1 != "Delete"){
            hashDeleteCourse(s1, hTable);
        }
        getline(cin, s1);
    }
    // Display the hash table after deletion
    displayHashTable(*hTable);
    

    
   /*
    char x = 'a';
    while(x != 'x'){
        cout << "Enter 'a' to insert, 'b' to delete, 'c' to search, 'd' to display, and 'x' to exit" << endl;
        cin >> x;
        cin.ignore(10000, '\n');
        switch(x){
            case 'a':
                cout << "Enter a course to insert. " << endl;
                getline(cin, s1);
                hashInsertCourse(s1, hTable);
                break;
                
            case 'b':
                cout << "Enter a course to delete. " << endl;
                getline(cin, s1);
                hashDeleteCourse(s1, hTable);
                break;
                
            case 'c':
                cout << "Enter a course to search. " << endl;
                getline(cin, s1);
                searchHashTable(s1, *hTable);
                break;
                
            case 'd':
                displayHashTable(*hTable);
                break;
                
            case 'x':
                
                break;
                
            default:
                cout << "Invalid input." << endl;
        }
        
        
    }
    
    */
    
    return 0;
}
