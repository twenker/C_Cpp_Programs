//
//  hw1q2.c
//  CSE240Homework

/* This C program demonstrates the switch statement without using breaks. */
/* The program is tested on MS Visual C++ platform                        */

#include <stdio.h>
#include <iostream>

#pragma warning(disable : 4996)

int main() {
	char ch = '+';
	int f, a = 10, b = 20;
	std::cout << "ch = " << ch << std::endl;
	switch (ch) {
        case '+': f = a + b; std::cout << "f = " << f << std::endl; break;
        case '-': f = a - b; std::cout << "f = " << f << std::endl; break;
        case '*': f = a * b; std::cout << "f = " << f << std::endl; break;
        case '/': f = a / b; std::cout << "f = " << f << std::endl; break;
        default: std::cout << "invalid operator\n" << std::endl;
	}
	ch = '-';
	std::cout << "ch = " << ch << std::endl;
	switch (ch) {
        case '+': f = a + b; std::cout << "f = " << f << std::endl; break;
        case '-': f = a - b; std::cout << "f = " << f << std::endl; break;
        case '*': f = a * b; std::cout << "f = " << f << std::endl; break;
        case '/': f = a / b; std::cout << "f = " << f << std::endl; break;
        default: std::cout << "invalid operator\n" << std::endl;
	}
	ch = '*';
	std::cout << "ch = " << ch << std::endl;
	switch (ch) {
        case '+': f = a + b; std::cout << "f = " << f << std::endl; break;
        case '-': f = a - b; std::cout << "f = " << f << std::endl; break;
        case '*': f = a * b; std::cout << "f = " << f << std::endl; break;
        case '/': f = a / b; std::cout << "f = " << f << std::endl; break;
        default: std::cout << "invalid operator\n" << std::endl;
	}
	ch = '/';
	std::cout << "ch = " << ch << std::endl;
	switch (ch) {
        case '+': f = a + b; std::cout << "f = " << f << std::endl; break;
        case '-': f = a - b; std::cout << "f = " << f << std::endl; break;
        case '*': f = a * b; std::cout << "f = " << f << std::endl; break;
        case '/': f = a / b; std::cout << "f = " << f << std::endl; break;
        default: std::cout <<"invalid operator\n" << std::endl;
	}
    return 0;
}
