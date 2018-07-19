//
//  Triangulation.h
//  Assignment8
//
//  Created by Todd Wenker on 4/27/15.
//  Copyright (c) 2015 Todd Wenker. All rights reserved.
//

#ifndef Assignment8_Triangulation_h
#define Assignment8_Triangulation_h
#include<cmath>
#include <iomanip>
using namespace std;
// a basic structure for recording the x and y values of a point
struct Point{
    int x;
    int y;
    
};

Point * newPoint(int x, int y){
    Point * p = new Point;
    p->x = x;
    p->y = y;
    return p;
}


//#include <iomanip> to use setw(), setprecision() functions
void printM(double **m, int size)
{
    int n = size;
    cout << "\nm matrix\n";
    cout << " i \n";
    cout << fixed << setprecision(2); //using 2 digits for fractions
    
    for (int i = n; i > 0; i--)
    {
        cout << left << " " << setw(2) << i << "|";
        cout << right; //right alignment
        for (int j = n; j > 0; j--)
        {
            if (j >= i)
            {
                cout << setw(8) << m[i][j] << "|";
            }
        }
        
        cout << "\n";
    }
    
    for (int i = n; i > 0; i--)
    {
        cout << setw(9) << i;
    }
    cout << setw(9) << " j \n";
}

//#include <iomanip> to use setw(), setprecision() functions
void printS(int **s, int size)
{
    int n = size;
    cout << "\ns matrix\n";
    cout << " i\n";
    
    for (int i = n-1; i > 0; i--)
    {
        cout << " " << left << setw(2) << i << "|";
        cout << right; //right alignment
        for (int j = n; j > 0; j--)
        {
            if (j > i)
            {
                cout << setw(2) << s[i][j] << "|";
            }
        }
        cout << "\n";
    }
    
    cout << "    ";
    for (int i = n; i > 1; i--)
    {
        cout << setw(2) << i << " ";
    }
    cout << " j \n";
    
}


// returns the distance between two points by using the Euclidean distance algorithm
float euclideanDistance(Point a, Point b){
    
    float x = (a.x - b.x); // x = (x1-x2)
    x = pow(x, 2); //x = (x1-x2)^2
    float y = (a.y - b.y); // y = (y1-y2)
    y = pow(y, 2); // y = (y1 - y2)^2
    float distance = x + y;// distance = (x1-x2)^2 + (y1-y2)^2)
    distance = sqrt(distance);
    return distance;
}

// returns the total weight of the three points connected
float weightFunction(Point a, Point b, Point c){
    float totalWeight = euclideanDistance(a, b) + euclideanDistance(a, c) + euclideanDistance(b, c);
    return totalWeight;
}

// based off the MATRIX-CHAIN-ORDER algorithm with a few changes
void optimalTriangulation(vector<Point *> list){
    
    double m[list.size()] [list.size()];
    int s[list.size()] [list.size()];
    int n = list.size()-1;
    
    for(int i = 0; i < n; i++){// sets the spots of the m array that are always 0 to 0
        m[i][i] = 0;
    }
    
    for (int L = 2; L <= n; L++)
    {
        for (int i = 1; i <= n-L+1; i++)
        {
            int j = i + L - 1;
            m[i][j] = INFINITY;
            
            cout << "\nComputing m[" << i << "," << j <<"]\n";
            for (int k = i; k <= j-1; k++)
            {
                double computedWeight =  weightFunction(*list[i-1],*list[k],*list[j]);
                double q = m[i][k]+m[k+1][j] + computedWeight;
                
                cout << "m[" << i << "," << k << "]+m["
                << (k+1) << "," << j << "]+"
                << "w(v" << (i-1) << ",v" << k << ",v" << j
                << ")="
                << fixed << setprecision(2) << m[i][k] << "+"
                << m[k+1][j] << "+"
                << computedWeight
                << "=" << q << endl;
                
                //TO BE COMPLETED
                if(q < m[i][j]){
                    m[i][j] = q;
                    s[i][j] = k;
                }
            }
            cout << "s[" << i << "," << j << "]=" << s[i][j] << endl;
        }
    }
    double **mPointer;
    mPointer = new double *[list.size()];
    for(int i = 0; i < list.size(); i++){
        mPointer[i] = m[i];
    }
    
    int **sPointer;
    sPointer = new int *[list.size()];
    for(int i = 0; i < list.size(); i++){
        sPointer[i] = s[i];
    }
    
    printM(mPointer, list.size());
    printS(sPointer, list.size());
    
    
}


#endif
