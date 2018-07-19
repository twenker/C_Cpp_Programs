
#include <stdio.h>
#include <time.h>
#pragma warning(disable : 4996);

void termination(int n){
    while(n != 1){
        if(n%2 == 0){
            n = n/2;
        }
        else{
            n = 3*n+1;
        }
        printf("%d \n",n);
    }
}

main(){
    clock_t start, end;
	start = clock();
 	   
    int n;
	printf("Enter an integer ...\n");
	scanf("%d", &n);
    termination(n);
	
	end = clock();
	double timeDifference = (double) end - start;
	double change = timeDifference/CLOCKS_PER_SEC;
	
    printf("%f seconds have elapsed", change);
}
