//
//  hw1q3.c
//  CSE240Homework
//
//  Created by Todd Wenker on 9/7/14.
//  Copyright (c) 2014 Todd Wenker. All rights reserved.
//

#include <stdio.h>

void main(){
    int n; int counter = 0;
    
    scanf("%d", &n);
    
    while(n != 1){
        if(n % 2 == 0){
            n = n / 2;
            
        }
        else{
            n = 3*n + 1;
        }
        printf("%d\n", n);
        counter++;
    }
    printf("%d iterations\n", counter);
}
