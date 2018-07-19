//Todd Wenker
//CSE240 Chen
//Homework 7
//10/26/14
#include<iostream>

using namespace std;

typedef enum{
    Triangle = 0, Rectangle, Square
}Shape;
class Polygon{
        int side1;
        int side2;
        int side3;
        int side4;
        Shape shape;
    public:
        void set_values(int, int, int, int);
        Shape ShapeValidator(){
            if(side4 == 0) {
                shape = Triangle;
                return Triangle;
            }
            if(side1==side2 && side1==side3 && side1==side4){
                shape = Square;
                return Square;
            }
            else{
                shape = Rectangle; return Rectangle;
            }
        };
        int Area(){
            if(shape == Triangle) return (.5*side1*side2);
            else return (side1*side2);};
        Shape getShape() {
            return shape;
        }
};
void Polygon::set_values(int a, int b, int c, int d){
    side1 = a;
    side2 = b;
    side3 = c;
    side4 = d;
}

int main(){
    int area, w, x, y, z;
    Shape shape;
    cout << "Please enter the object sides: \n" << endl;
    cout << "Side 1: \n" << endl;
    cin >> w;
    cout << "Side 2: \n" << endl;
    cin >> x;
    cout << "Side 3: \n" << endl;
    cin >> y;
    cout << "Side 4: \n" << endl;
    cin >> z;
    Polygon poly;
    poly.set_values(w, x, y, z);
    shape = poly.ShapeValidator();
    area = poly.Area();
    if(poly.getShape() == 0){
        cout << "This is a triangle.\n" << endl;
    }
    if(poly.getShape() == 1){
        cout << "This is a rectangle.\n" << endl;
    }
    if(poly.getShape() == 2){
        cout << "This is a square.\n" << endl;
    }
    cout << "The area for this shape is: " << area;
    return 0;
}



