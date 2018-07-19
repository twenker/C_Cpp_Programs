//
//  main.cpp
//  Assignment8
//
//  Created by Todd Wenker on 4/27/15.
//  Copyright (c) 2015 Todd Wenker. All rights reserved.
//

#include <iostream> //to use cin and cout
#include <string> //to use strings
#include <cstdlib> //to use string functions
#include <iomanip>
#include "vector"
#include "Triangulation.h"

using namespace std;

int main() {
    /*
    vector<Point> list (10);
    int input = 0;
    int counter = 0;
    string s1;
    string delimiter = " ";
    cin >> input;
    cin.ignore(10000, '\n');
    cout << "enter points" << endl;
    while(counter < input){
        int x, y;
        
        getline(cin, s1);
        
        x = stoi(s1.substr(0, s1.find(delimiter)));
        y = stoi(s1.substr(s1.find(delimiter)+1 , s1.length()));
        list[counter].x = x;
        list[counter].y = y;
        counter++;
    }
    
    for(int i = 0; i < list.size() && list[i].x != 0; i++){
        cout << "x: " << list[i].x << "\ny: " << list[i].y << endl;
    }
     */
    string s1;
    string delimiter = " ";
    
    int count = -1;
    while(count != 0){
        cout << "Enter a size" << endl;
        cin >> count;
        cin.ignore(10000, '\n');
        
        vector<Point *> list;
        
        int index = 0;
        while(index < count){
            
            int x,y;
            
            getline(cin, s1);
            
            x = stoi(s1.substr(0, s1.find(delimiter)));
            y = stoi(s1.substr(s1.find(delimiter)+1 , s1.length()));
            
            list.push_back(newPoint(x, y));
            index++;
        }
        if(count != 0){
            optimalTriangulation(list);
            list.clear();
        }
    }
   
    
    
    return 0;
    
}
