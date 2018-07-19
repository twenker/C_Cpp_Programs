// Assignment #1
// Name: Todd Wenker
// ASU Email Address: Todd Wenker
// Description: This program displays a menu of choices to a user
//        and performs the chosen task. It will keep asking a user to
//        enter the next choice until the choice of 'Q' (Quit) is entered.

#include <iostream>
#include <string>
#include "LinkedList.h"

using namespace std;

void printMenu();

int main()
{
    // local variables, can be accessed anywhere from the main method
    char input1 = 'Z';
    string inputInfo;
    string courseNumber, courseTitle;
    int index;
    string line;
    bool success = false;
    
    // instantiate a Linked List object
    LinkedList * list1 = new LinkedList();
    
    printMenu();
    
    
    do  // will ask for user input
    {
        cout << "What action would you like to perform?\n";
        cin.get(input1);
        input1 = toupper(input1);
        cin.ignore(20, '\n'); //flush the buffer
        // matches one of the case statement
        switch (input1)
        {
            case 'A':   //Add Course
                cout << "Please enter some course information:\n";
                cout << "Enter a course number:\n";
                getline(cin, courseNumber);
                
                cout << "Enter a course title:\n";
                getline(cin, courseTitle);
                
                cout << "Enter an index to add:\n";
                cin >> index;
                cin.ignore(20, '\n'); //flush the buffer
                
                success = list1->addElement(courseNumber, courseTitle, index);
                
                if (success == true)
                    cout << "The course " << courseNumber << " with title " << courseTitle << " added\n";
                else
                    cout << "The index is out of bounds\n";
                break;
            case 'D':   //Display Courses
                list1->printList();
                break;
            case 'Q':   //Quit
                delete list1;
                break;
            case 'R':  //Remove Course
                cout << "Please enter a course to remove:\n";
                cout << "Enter a course number:\n";
                getline(cin, courseNumber);
                
                cout << "Enter a course title:\n";
                getline(cin, courseTitle);
                
                success = list1->removeElement(courseNumber, courseTitle);
                if (success == true)
                    cout << "The course " << courseNumber << " with title " << courseTitle << " removed\n";
                else
                    cout << "The course " << courseNumber << " with title " << courseTitle << " does not exist\n";
                break;
            case '?':   //Display Menu
                printMenu();
                break;
            default:
                cout << "Unknown action\n";
                break;
        }
        
    } while (input1 != 'Q');
    
    return 0;
}


/** The method printMenu displays the menu to a user**/
void printMenu()
{
    cout << "Choice\t\tAction\n";
    cout << "------\t\t------\n";
    cout << "A\t\tAdd Course\n";
    cout << "D\t\tDisplay Courses\n";
    cout << "Q\t\tQuit\n";
    cout << "R\t\tRemove Course\n";
    cout << "?\t\tDisplay Help\n\n";
}

