#include<stdio.h>
#include<stdlib.h>
#pragma warning(disable : 4996)
int main() {
	int *pointer, i, j, array[6][4];
	pointer = malloc(24 * sizeof(int));
 	for (i = 0; i < 6; i++) {
		for (j = 0; j < 4; j++)
			*(pointer + (4 * i) + j) = 4 * i + j;
	}
 	printf("%d at %u\n", *(pointer + 6), pointer + 6);
	printf("%d at %u\n", *(pointer + 7), pointer + 7);
	for (i = 0; i < 6; i++) {
		for (j = 0; j < 4; j++)
			array[i][j] = i * 4 + j;
	}
	printf("%d at %u\n", **(array + 5), &array[5][0]);
	printf("%d at %u\n", array[5][1], *(array + 5) + 1);
	return 0;
}
