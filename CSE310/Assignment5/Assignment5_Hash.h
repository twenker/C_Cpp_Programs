//
//  Assignment5_Hash.h
//  Assignment5.cpp
//
//  Created by Todd Wenker on 3/1/15.
//  Copyright (c) 2015 Todd Wenker. All rights reserved.
//

#ifndef Assignment5_cpp_Assignment5_Hash_h
#define Assignment5_cpp_Assignment5_Hash_h
#include <iostream>
#include <string>

using namespace std;

// the course structure contains a courseNumber string and a courseName string and a next pointer
struct Course{
    string courseNumber;
    string courseName;
    Course* next;
    
};

// creates an array of Course pointers and an int array to keep track of the size
struct hashTable{
    int tableSize;
    Course* table[50];
    int linkedListSize[50];
};

//Inserts a course in the hashTable at the appropriate index
void hashInsertCourse(string s1, hashTable* hTable){
    Course * newCourse;
    newCourse = new Course;
    string delimiter = "/"; // the character the indicates the split between courseNumber and courseName
    
    string classNumber = s1.substr(0, s1.find(delimiter));
    string className = s1.substr(s1.find(delimiter)+1 , s1.length());   // splits the string input
    
    newCourse->courseNumber = classNumber;
    newCourse->courseName = className;
    
    string number = classNumber.substr(3,5);
    int index = stoi(number,nullptr,10); // extract the actual number part of the courseNumber
    
    // the hash function places the Course at mod hashTableSize of the its course number
    index = index % hTable->tableSize;
    
    if(hTable->table[index] == 0){   // if the hashTable is empty at that index
        hTable->table[index] = newCourse;
        hTable->linkedListSize[index]++; //increment the array holding the size of the linked list
        return;
    }
    else{ // if the hashTable already has a Course, the new Course is placed at the front of the list
        newCourse->next = hTable->table[index];
        hTable->table[index] = newCourse;
        hTable->linkedListSize[index]++; //increment the array holding the size of the linked list
        return;
    }
    return;
}

// Recieves a string that needs to be split between courseNumber and courseName and before the hash table
// is searched and if a Course with matching attributes is found, it is deleted.
void hashDeleteCourse(string s1, hashTable* hTable){
    
    bool flag = false;  // boolean flag to keep see if the Course is found
    string delimiter = "/"; // the character the indicates the split between courseNumber and courseName
    
    string classNumber = s1.substr(0, s1.find(delimiter));
    string className = s1.substr(s1.find(delimiter)+1 , s1.length());   // splits the string input
    
    string number;
    number = s1.substr(3, 5);
    int index = stoi(number, nullptr, 10);
    index = index % hTable->tableSize; // finds the hash index of the string input
    
    Course* preCourse;
    Course* delCourse;
    delCourse = hTable->table[index];  //sets the Course pointer that will iterate through the linked list
    
    if(delCourse->courseNumber.compare(classNumber) == 0 && delCourse->courseName.compare(className) == 0){
        hTable->table[index] = delCourse->next;
        cout << "Course number " << classNumber << " with " << className << " has been removed." << endl;
        hTable->linkedListSize[index]--;// increments the linkedListSize to match the delete
        flag = true;
        delete delCourse;
    }
    
    // If the Course to be deleted isn't the first, then the list must be traversed to find it
    preCourse = delCourse;
    delCourse = delCourse->next;
    while(delCourse != 0){
        if(delCourse->courseNumber.compare(classNumber) == 0 && delCourse->courseName.compare(className) == 0){
            preCourse->next = delCourse->next;
            cout << "Course number " << classNumber << " with " << className << " has been removed." << endl;
            hTable->linkedListSize[index]--;// increments the linkedListSize to match the delete
            flag = true;
            delete delCourse;
        }
        preCourse = delCourse;
        delCourse = delCourse->next;
            
    }
    //If the previous while loop didn't find a match, then the following statement is printed.
    if(flag == false){
        cout << "Course number " << classNumber << " with " << className << " was not found." << endl;
    }
    
    return;
}

// returns a true statement if the Course is in the hashTable and false if not
void searchHashTable(string s1, hashTable hTable){
    string classNumber;
    classNumber = s1.substr(3, 5);
    int index = stoi(classNumber, nullptr, 10);
    index = index % hTable.tableSize;   // finds where a Course with s1 as a class number would hash to
    
    // iterates through the linked list at that specific index
    Course* conductor;
    conductor = hTable.table[index];
    while(conductor != NULL){
        // if a Course with the matching Course number is found, the function prints that its true
        if(conductor->courseNumber.compare(s1) == 0){
            cout << "The course " << s1 << " was found." << endl;
            return;
        }
        conductor = conductor->next;
    }
    // if the Course was not found, this text prints
    cout << "Course number " << s1 << " not found." << endl;
    return;
}

//prints the index and linked list size along with the contents of the linked list at that index
void displayHashTable(hashTable hTable){
    int index = 0;
    Course* conductor;
    while(index <= hTable.tableSize){ //while the index is less than the size of the hash table
        
        if(hTable.table[index] == 0){   //If the hash table is empty at the index.
            cout <<"Index: " << index << ", " << "linked list size: " << hTable.linkedListSize[index] <<endl;   //lists the index number and the size of the linked list
            
            cout << "The list is empty." << endl;
        }
        
        else{   //If the hash table is NOT empty at the index.
            cout <<"Index: " << index << ", " << "linked list size: " << hTable.linkedListSize[index] <<endl;   //lists the index number and the size of the linked list
            
            conductor = hTable.table[index];
            while(conductor->next != 0){    //iterates through the linked list
                cout << "Course Number: " << conductor->courseNumber << ", " << "Course Title: "<<conductor->courseName << endl;   //prints the course number and name
                conductor = conductor->next;
            }
            
            cout << "Course Number: " << conductor->courseNumber << ", " << "Course Title: "<<conductor->courseName << endl;
        }
        index++;
    }
    return;
}


#endif
