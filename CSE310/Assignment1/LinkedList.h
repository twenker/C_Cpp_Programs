// Assignment #1
// Name: Todd Wenker
// ASU Email Address: twenker@asu.edu
// Description: Contains the struct Course and LinkedList class along with the
//                 

#include <iostream> //to use cout
#include <iomanip> //to format output
#include <string> //to use strings

using namespace std;

//struct Course represents some course information
struct Course
{
    string number;
    string title;
    struct Course * next;
};

//class LinkedList will contains a linked list of courses
class LinkedList
{
private:
    struct Course * head;
    
public:
    LinkedList();
    ~LinkedList();
    bool addElement(string number, string title, int index);
    bool removeElement(string number, string title);
    void printList();
};

//Constructor to initialize the linked list
LinkedList::LinkedList()
{
    head = NULL;
}

//Destructor
//Description: The deconstructor will iterate throught the LinkedList and delete each course
LinkedList::~LinkedList()
{
    //To be completed
    Course *temp = this->head;
    Course *prev = new Course;
    while(temp != 0){
        prev = temp->next;
        delete temp;
        temp = prev;
    }
    this->head = 0;
}

//Description: addElement inserts a new Course struct into the LinkedList based on the index
bool LinkedList::addElement(string newNumber, string newTitle, int index)
{
    //To be completed
    Course *newCourse;
    newCourse->number = newNumber;
    newCourse->title = newTitle;
    
    if(index == 0){
        newCourse->next = this->head;
        this->head = newCourse;
        cout << "The course has been added." << endl;
        return true;
    }
    
    if(index < 0){
        cout << "The index is out of range." << endl;
        return false;
    }
    
    Course *temp = this->head;
    Course *prev;
    
    for(int i = 0; i < index; i++){
        prev = temp;
        temp = temp->next;
    }
    prev->next = newCourse;
    newCourse->next = temp;
    return true;
    
}

//Description: removeElement deletes an element in the LinkedList that matches the input number and title
bool LinkedList::removeElement(string someNumber, string someTitle){
    //To be complete
    Course *temp = this->head;
    Course *prev;
    if(this->head == 0){
        return false;
    }
    while(temp != 0){
        if(someNumber.compare(temp->number) == 0 && someTitle.compare(temp->title) == 0){
            delete temp;
            prev->next = temp->next;
            return true;
        }
        prev = temp;
        temp = temp->next;
    }
    return false;
    
}

//Description: Prints every element in the LinkedList
void LinkedList::printList()
{
    //To be complete
    
    if(this->head == 0){
        cout << "The list is empty\n";
        return;
    }
    else{
        Course *temp = this->head;
        while(temp->next != 0){
            cout << "Course Number: " << temp->number << ", Course Title: " << temp->title << endl;
            temp = temp->next;
        }
        return;
    }
}
