//Todd Wenker
//CSE 240 Chen
//11/2/14

#include<iostream>
#define _USE_MATH_DEFINES
#include<cmath>

using namespace std;

class Shape{
    public:
        virtual float area(){return 0;}
        virtual void displayInfo(){}
};

class Rectangle: public Shape{
    protected:
        float base;
        float height;
    public:
        virtual float area(){return (base*height);}
        virtual void displayInfo(){
            cout << "Recatangle with " << base << " base and " << height << " height and an area of " << area() << endl;
        }
        int getBase(){return base;}
        int getHeight(){return height;}
        Rectangle(float, float);

};
Rectangle::Rectangle(float x, float y){
    base = x;
    height = y;
}

class Square: public Rectangle{
    public:
        Square(float n) : Rectangle(n, n){};
        float area(){return base*height;}
        void displayInfo(){
            cout << "Square with " << base << " length sides and and area of " << area() << endl;
        }

};

class Circle: public Shape{
    private:
        float radius;
    public:
        Circle(float);
        float area(){return (M_PI*radius*radius);};
        void displayInfo(){
            cout << "Circle with a radius of " << radius << " and an area of " << area() << endl;
        }
};
Circle::Circle(float x){
    radius = x;
}

struct ShapeCollection{
    Shape *node;
    ShapeCollection *next;
};

Shape * newShape();
void insertion(Shape*);
ShapeCollection *ShapeList = NULL;
int main(){
    char x;
    while(x != 'q'){
        cout << "Enter a to insert a shape, b to display ShapeList and q to quit. " << endl;
        cin >> x;
        Shape *s;
        ShapeCollection *temp;
        switch(x){
            case 'a':
                s = newShape();
                insertion(s);
                break;
            case 'b':
                temp = ShapeList;
                while(temp->next != NULL){
                    temp->node->displayInfo();
                    temp = temp->next;
                }
                break;
            case 'q':
                break;
            default:
                cout << "Incorrect command." << endl;
                break;
        }
    }

}

Shape * newShape(){
    Shape *n;
    char x;
    cout << "a to insert rectangle.\nb to insert square.\nc to insert circle." << endl;
    cin >> x;
    switch(x){
        case 'a':
            float x, y;
            cout << "Enter the base then height: " << endl;
            cin >> x;
            cin >> y;
            n = new Rectangle(x, y);
            break;
        case 'b':
            float z;
            cout << "Enter the length of the sides: " << endl;
            cin >> z;
            n = new Square(z);
            break;
        case 'c':
            float r;
            cout << "Enter the radius: " << endl;
            cin >> r;
            n = new Circle(r);
            break;
    }
    return n;
}

void insertion(Shape *s){
    ShapeCollection *temp1, *temp2;
    temp1->node = s;
    temp1->next = NULL;
    if(ShapeList == NULL){
        ShapeList = temp1;
    }
    else{
        temp2 = ShapeList;
        while(temp2->next != NULL){
            temp2 = temp2->next;
        }
        temp2->next = temp1;
    }
}

