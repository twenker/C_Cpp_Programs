//
//  hw1q1-1.c
//  CSE240Homework

/* This C program demonstrates the switch statement without using breaks. */
/* The program is tested on MS Visual C++ platform                        */

#include <stdio.h>

#pragma warning(disable : 4996)

void main() {
	char ch = '+';
	int f, a = 10, b = 20;
	printf("ch = %c\n", ch);
	switch (ch) {
        case '+': f = a + b; printf("f = %d\n", f);break;
        case '-': f = a - b; printf("f = %d\n", f);break;
        case '*': f = a * b; printf("f = %d\n", f);break;
        case '/': f = a / b; printf("f = %d\n", f);break;
        default: printf("invalid operator\n");
	}
	ch = '-';
	printf("ch = %c\n", ch);
	switch (ch) {
        case '+': f = a + b; printf("f = %d\n", f);break;
        case '-': f = a - b; printf("f = %d\n", f);break;
        case '*': f = a * b; printf("f = %d\n", f);break;
        case '/': f = a / b; printf("f = %d\n", f);break;
        default: printf("invalid operator\n");
	}
	ch = '*';
	printf("ch = %c\n", ch);
	switch (ch) {
        case '+': f = a + b; printf("f = %d\n", f);break;
        case '-': f = a - b; printf("f = %d\n", f);break;
        case '*': f = a * b; printf("f = %d\n", f);break;
        case '/': f = a / b; printf("f = %d\n", f);break;
        default: printf("invalid operator\n");
	}
	ch = '/';
	printf("ch = %c\n", ch);
	switch (ch) {
        case '+': f = a + b; printf("f = %d\n", f);break;
        case '-': f = a - b; printf("f = %d\n", f);break;
        case '*': f = a * b; printf("f = %d\n", f);break;
        case '/': f = a / b; printf("f = %d\n", f);break;
        default: printf("invalid operator\n");
	}
}
