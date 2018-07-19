// Assignment #6
// Name: Todd Wenker
// ASU Email Address:twenker@asu.edu
// Arizona State University CSE310
// Description: Implements the methods of the Course and BinarySearchTree classes.


#ifndef Assignment6_cpp_BinarySearchTree_h
#define Assignment6_cpp_BinarySearchTree_h
#include <iostream> //to use cout
#include <string> //to use strings
#include <cstdlib> //to use atoi

using namespace std;

//Course represents some course information
class Course
{
private:
    string courseNumber, courseTitle;
    Course * left, * right, * parent;
    
    
public:
    //The following two functions are constructors
    Course()
    {
        courseNumber = "?";
        courseTitle = "?";
        parent = NULL;
        left = NULL;
        right = NULL;
    }
    
    Course(string courseNumber1, string courseTitle1)
    {
        courseNumber = courseNumber1;
        courseTitle = courseTitle1;
        parent = NULL;
        left = NULL;
        right = NULL;
    }
    
    //The following functions are accessor functions
    Course * getLeft()
    {
        return left;
    }
    
    Course * getRight()
    {
        return right;
    }
    
    Course * getParent()
    {
        return parent;
    }
    
    string getCourseNumber()
    {
        return courseNumber;
    }
    
    string getCourseTitle()
    {
        return courseTitle;
    }
    
    
    //The following functions are mutator functions
    void setLeft(Course * other)
    {
        left = other;
    }
    
    void setRight(Course * other)
    {
        right = other;
    }
    
    void setParent(Course * other)
    {
        parent = other;
    }
    
    //The print method prints the courseNumber and courseTitle
    void print()
    {
        cout << "Course Number: " << courseNumber << ", Course Title: " << courseTitle << endl;
    }
    
    //you can add more functions
    
};

//class BinarySearchTree will contains courses
class BinarySearchTree
{
private:
    Course * root;
    
public:
    BinarySearchTree();
    ~BinarySearchTree();
    int countNodes(Course *);
    bool isEmpty();
    void inOrderTreePrint();
    void preOrderTreePrint();
    void postOrderTreePrint();
    void inOrderTreeWalk(Course *);
    void preOrderTreeWalk(Course *);
    void postOrderTreeWalk(Course *);
    Course * treeMinimum();
    Course * treeMaximum();
    Course * treeSuccessor(string);
    Course * treePredecessor(string);
    Course * treeSearch(string);
    bool treeInsert(string,string);
    bool rightRotate(string);
    bool leftRotate(string);
};

//The constructor initialize the root to be NULL
BinarySearchTree::BinarySearchTree()
{
    root = NULL;
}

//The descurtor should delete all nodes in the tree
//and perform garbage collections starting from leaves (bottom-up order).
//The destructor should also print "The number of nodes deleted: X"
//where X is the number of nodes deleted
BinarySearchTree::~BinarySearchTree()
{
    //TO BE COMPLETED
    Course * conducter = root;
    
    int counter = countNodes(root);
    cout << "The number nodes deleted: " << counter;
    //deletes the tree nodes
    if( conducter == NULL){
        return;
    }
    delete conducter->getLeft();
    delete conducter->getRight();
    delete conducter;
    
    return;
}
//used to count the nodes in the tree before they are deleted
int BinarySearchTree::countNodes(Course *x){
    
    int counter = 0;
    
    if(x == NULL){
        return counter;
    }
    else{
        counter = 1;
        counter += countNodes(x->getLeft());
        counter += countNodes(x->getRight());
        return counter;
    }
    
}

//Checks if the tree is empty
bool BinarySearchTree::isEmpty()
{
    if (root == NULL)
        return true;
    else
        return false;
}

//prints all nodes using in-order starting from root
void BinarySearchTree::inOrderTreePrint()
{
    if (isEmpty())
        cout << "tree is empty" << endl;
    else
    {
        cout << "\n";
        inOrderTreeWalk(root);
        cout << "\n";
    }
}

//prints all nodes using pre-order starting from root
void BinarySearchTree::preOrderTreePrint()
{
    if (isEmpty())
        cout << "tree is empty" << endl;
    else
    {
        cout << "\n";
        preOrderTreeWalk(root);
        cout << "\n";
    }
}

//prints all nodes using post-order starting from root
void BinarySearchTree::postOrderTreePrint()
{
    if (isEmpty())
        cout << "tree is empty" << endl;
    else
    {
        cout << "\n";
        postOrderTreeWalk(root);
        cout << "\n";
    }
}

//print nodes under the parameter x using in-order
void BinarySearchTree::inOrderTreeWalk(Course * x)
{
    //TO BE COMPLETED
    //inOrderTreeWalk goes left, root node, then right node
    if(x != NULL){
        inOrderTreeWalk(x->getLeft());
        cout << "Course Number: " << x->getCourseNumber() << ", Course Title: " << x->getCourseTitle() << endl;
        inOrderTreeWalk(x->getRight());
    }
    return;
}

//print nodes under the parameter x using pre-order
void BinarySearchTree::preOrderTreeWalk(Course * x)
{
    //TO BE COMPLETED
    //preOrderTreeWalk moves root node, left node, then right node
    if(x != NULL){
        cout << "Course Number: " << x->getCourseNumber() << ", Course Title: " << x->getCourseTitle() << endl;
        inOrderTreeWalk(x->getLeft());
        inOrderTreeWalk(x->getRight());
    }
    return;
}

//print nodes under the parameter x using post-order
void BinarySearchTree::postOrderTreeWalk(Course * x)
{
    //TO BE COMPLETED
    //postOrderTreeWalk moves left node, right node, than root node
    if(x != NULL){
        inOrderTreeWalk(x->getLeft());
        inOrderTreeWalk(x->getRight());
        cout << "Course Number: " << x->getCourseNumber() << ", Course Title: " << x->getCourseTitle() << endl;
    }
    return;
}

//Using the string parameter of courseNumber,
//it searches and returns the node of the given course number.
//It returns NULL if not found
Course * BinarySearchTree::treeSearch(string courseNumber)
{
    //TO BE COMPLETED
    Course *conductor = root;
    
    while(conductor != NULL){
        //if the courseNumber matches the conductor, then it is returned
        if(courseNumber == conductor->getCourseNumber()){
            return conductor;
        }
        //if the courseNumber does not equal the conductor, then it needs to be compared to see if the
        //conductor should alternate laft or right
        else{
            
            if(courseNumber < conductor->getCourseNumber()){
                conductor = conductor->getLeft();
            }
            else{
                conductor = conductor->getRight();
            }
        }
    }
    return conductor;
}

//Searches and returns the node with the minimum key within the tree
Course * BinarySearchTree::treeMinimum()
{
    //TO BE COMPLETED
    Course *conductor = root;
    
    //the smallest node lies in the bottom left corner of the tree so the conductor has to keep moving
    //left
    while(conductor->getLeft() != NULL){
        conductor = conductor->getLeft();
    }
    return conductor;
}

//Searches and returns the node with the maximum key within the tree
Course * BinarySearchTree::treeMaximum()
{
    //TO BE COMPLETED
    Course *conductor = root;
    
    //the largest node lies in the bottom right corner of the tree so the conductor has to keep moving
    //right
    while(conductor->getRight() != NULL){
        conductor = conductor->getRight();
    }
    return conductor;
}

//Searches and returns the successor of the parameters courseNumber and courseTitle
Course * BinarySearchTree::treeSuccessor(string courseNumber)
{
    //TO BE COMPLETED
    Course *x = treeSearch(courseNumber);
    
    if(x != NULL){
        if(x->getRight() != NULL){
            while(x->getLeft() != NULL){
                x = x->getLeft();
            }
            return x;
        }
        else{
            Course *y = x->getParent();
            while(y != NULL && x == y->getRight()){
                x = y;
                y = y->getParent();
            }
            return y;
        }
    }
    return x;
}

//Searches and returns the predecessor of the parameter courseNumber
Course * BinarySearchTree::treePredecessor(string courseNumber)
{
    //TO BE COMPLETED
    Course *x = treeSearch(courseNumber);
    if(x == NULL){
        return x;
    }
    if(x->getLeft() != NULL){
        while(x->getRight() != NULL){
            x = x->getRight();
        }
        return x;
    }
    else{
        Course *y = x->getParent();
        while(y != NULL && x == y->getLeft()){
            x = y;
            y = y->getParent();
        }
        return y;
    }
}


//It inserts the parameter courseNumber and courseTitle,
//and returns true if successful.
//it does not insert if the same course number already exists in the tree
//or not enough memory and returns false.
bool BinarySearchTree::treeInsert(string courseNumber, string courseTitle)
{
    //TO BE COMPLETED
    Course *newCourse = new Course(courseNumber, courseTitle);
    Course *previous = NULL;
    Course *conductor = root;
    
    //if the tree already has a copy of the newCourse, then it returns false, else it adds the newCourse
    if(treeSearch(courseNumber) == NULL){
        
        //if the tree is empty, then the newCourse becomes the new root.
        if(isEmpty()){
            root = newCourse;
            return true;
        }
        //else, the newCourse has to be placed in the correct place
        else{
            //new nodes are placed at the end of the list so the conductor has to reach a null node
            while(conductor != NULL){
                previous = conductor;
                //conductor moves left if the new courseNumber is less than the current one
                if(newCourse->getCourseNumber() < previous->getCourseNumber()){
                    conductor = conductor->getLeft();
                }
                //conductor moves right if the new courseNumber is greater than the current one
                else{
                    conductor = conductor->getRight();
                }
            }
            //sets the parent of the new node to the previous
            newCourse->setParent(previous);
            //sets the new node as either the left or the right child of the previous node depending on if
            //the new courseNumber is less than or greater than the previous one
            if(newCourse->getCourseNumber() < previous->getCourseNumber()){
                previous->setLeft(newCourse);
            }
            else{
                previous->setRight(newCourse);
            }
            return true;
        }
        
    }
    //if the treeSearch returns not null, then there is already a copy in the tree and the method returns
    //false
    
    return false;
}

//It performs a left rotation around the node with the parameter
//course number. It returns true if successful, and false if
//the node with the parameter course number is not found
//or the course does not have a right child

bool BinarySearchTree::leftRotate(string number)
{
    //TO BE COMPLETED
    Course *x = treeSearch(number);
    if(x != NULL){
        return false;
    }
    Course *y = x->getRight();
    
    x->setRight(y->getLeft());
    
    if(y->getLeft() != NULL){
        y->getLeft()->setParent(x);
    }
    y->setParent(x->getParent());
    if(x->getParent() == NULL){
        root = y;
    }
    else{
        if(x == x->getParent()->getLeft()){
            x->getParent()->setLeft(y);
        }
        else{
            x->getParent()->setRight(y);
        }
    }
    y->setLeft(x);
    x->setParent(y);
    return true;
}

//It performs a right rotation around the node with the parameter
//course number. It returns true if successul, and false if
//the node with the parameter course number is not found
//or the course does not have a left child
bool BinarySearchTree::rightRotate(string number)
{
    //TO BE COMPLETED
    Course *x = treeSearch(number);
    if(x == NULL){
        return false;
    }
    Course *y = x->getLeft();
    
    x->setLeft(y->getRight());
    
    if(y->getRight() != NULL){
        y->getRight()->setParent(x);
    }
    y->setParent(x->getParent());
    if(x->getParent() == NULL){
        root = y;
    }
    else{
        if(x == x->getParent()->getRight()){
            x->getParent()->setRight(y);
        }
        else{
            x->getParent()->setLeft(y);
        }
    }
    y->setRight(x);
    x->setParent(y);
    return true;
}

#endif
