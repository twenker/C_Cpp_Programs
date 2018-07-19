#include <stdio.h>
#include<string.h>
#include<stdlib.h>

#pragma warning(disable : 4996)

#define NUM_OF_CHUNKS 4
#define CHUNK_LENGTH 16

#define ENCRYPTION_SHIFT 5

char chunks[NUM_OF_CHUNKS][CHUNK_LENGTH];

// reset the chunks
void reset()
{
	int row, col;

	for (row = 0; row < NUM_OF_CHUNKS; row++)
	{
		for (col = 0; col < CHUNK_LENGTH; col++)
		{
			chunks[row][col] = '\0';
		}
	}
}

// decompartmentalize a string into different chunks
void decompartmentalize(char value[(NUM_OF_CHUNKS * CHUNK_LENGTH) - (NUM_OF_CHUNKS * 2)])
{
	//TODO: Rewrite decompartmentialize(char *value) using
	//      pointer operations. It must retain the row number
	//      at the beginning of each chunk and each chuck
	//      must end with a null terminator.

	int index = 0;
	int row, col;

	for (row = 0; row < NUM_OF_CHUNKS; row++)
	{
		// insert an index for each chunk. Use for sorting
		chunks[row][0] = (char)(row + 49);

		for (col = 1; col < CHUNK_LENGTH - 1; col++)
		{
			if (index < (NUM_OF_CHUNKS * CHUNK_LENGTH) - (NUM_OF_CHUNKS * 2))
				chunks[row][col] = value[index++];
		}

		// each chunk will end with a null terminator
		chunks[row][CHUNK_LENGTH - 1] = '\0';
	}
}

// given two strings, swaps their contents in memory.
void swap_strings(char *x, char *y)
{
	//create a temporary holding place for the data so we don't lose it.
	char temp[CHUNK_LENGTH];

	strcpy(temp, x);
	strcpy(x, y);
	strcpy(y, temp);
}

// randomize the blocks
void randomization_chunks()
{
	int number_of_times, index_a, index_b;

	srand(time(NULL));  // initialize random seed

	for (number_of_times = 0; number_of_times < 10; number_of_times++)
	{
		// generate a random index between 0 and 3.
		index_a = rand() % NUM_OF_CHUNKS;
		index_b = rand() % NUM_OF_CHUNKS;

		// Swap the 2 string blocks
		swap_strings(chunks + index_a, chunks + index_b);
	}
}

// for each string in the global chunks variable,
// shifts the characters in it by ENCRYPTION_SHIFT.
void encrypt_chunks()
{
	//TODO: Implement encrypt_chunks() using pointer operations.
	//     Loop over each string in the array and shift the
	//     characters in it by adding ENCRYPTION_SHIFT value
	//     to each character. Remember that C-style strings
	//     have a null terminator at the end. Do not apply the
	//	shift to the terminator.
    int i, j;
    char *pointer;
	pointer = malloc(64 * sizeof(char));
	pointer = chunks;
	for(i = 0; i < NUM_OF_CHUNKS; ++i){
        for(j = 0; j < CHUNK_LENGTH; ++j){
            *(pointer + (16 * i) + j) = *pointer + ENCRYPTION_SHIFT;
        }
	}
}

// for each string in the global chunks variable,
// de-shifts the characters in it by ENCRYPTION_SHIFT.
void decrypt_chunks()
{
	//TODO: Implement decrypt_chunks() using pointer operations.
	//     Loop over each string in the array and shift the
	//     characters in it by subtracting ENCRYPTION_SHIFT value
	//	from each character. Remember that C-style strings have
	//     a null terminator at the end. Do not apply the shift
	//     to the terminator.
	int i, j;
    char *pointer;
	pointer = malloc(64 * sizeof(char));
	pointer = chunks[0][0];
	for(i = 0; i < NUM_OF_CHUNKS; ++i){
        for(j = 0; j < CHUNK_LENGTH; ++j){
            *(pointer + (16 * i) + j) = *pointer - ENCRYPTION_SHIFT;
        }
	}
}

// sorts the strings the global chunks variable by the
// first character they contain.
void sort_chunks()
{
	//TODO: Implement sort_chunks() using pointer operations.
	//	Using your favorite sorting algorithm (we suggest
	//	selection sort), sort the array containing the
	//	message chunks.  Sort based on the first character
	//	in the chunk - it will always be a number. We provide
	//	a swap_strings function that you may use. Example
	//     usage: swap_strings(chunks[0], chunks[1]) will swap
	//	the contents of the first and second string.
	 char *pointer;

	 for (int i = 0; i < NUM_OF_CHUNKS - 1; ++i)
     {
          pointer = chunks[i];
          for (int j = i + 1; j < NUM_OF_CHUNKS; ++j){


                if (chunks[j][0] < *pointer)
                    pointer = chunks[j];

          }
          swap_strings(chunks[i], pointer);
     }

}

// displays the strings in the global chunks variable
void display_chunks()
{
	char *p;

	int index = 0;

	// Calls for the first element address and then increment byte addressable memory by 1.
	p = &chunks[0][0] + 1;

	for (; index < NUM_OF_CHUNKS; index++) {

		// Print each chunks
		printf("%s", p);

		// Increment to the next chiper block
		p += CHUNK_LENGTH;
	}
}


int main()
{
	// reserve 8 bytes for future processing
	char input[(NUM_OF_CHUNKS * CHUNK_LENGTH) - (NUM_OF_CHUNKS * 2)];

	reset(chunks);

	printf("Please enter a phrase (Max %d Characters): \n", (NUM_OF_CHUNKS * CHUNK_LENGTH) - (NUM_OF_CHUNKS * 2));
	fgets(input, (NUM_OF_CHUNKS * CHUNK_LENGTH) - (NUM_OF_CHUNKS * 2), stdin);

	decompartmentalize(input, chunks);

	randomization_chunks();

	encrypt_chunks();

	sort_chunks();

	decrypt_chunks();

	display_chunks();

	return 0;
}

// for each string in the global chunks variable,
// de-shifts the characters in it by ENCRYPTION_SHIFT.
void decrypt_chunks()
{
	//TODO: Implement decrypt_chunks() using pointer operations.
	//     Loop over each string in the array and shift the
	//     characters in it by subtracting ENCRYPTION_SHIFT value
	//	from each character. Remember that C-style strings have
	//     a null terminator at the end. Do not apply the shift
	//     to the terminator.
}

// sorts the strings the global chunks variable by the
// first character they contain.
void sort_chunks()
{
	//TODO: Implement sort_chunks() using pointer operations.
	//	Using your favorite sorting algorithm (we suggest
	//	selection sort), sort the array containing the
	//	message chunks.  Sort based on the first character
	//	in the chunk - it will always be a number. We provide
	//	a swap_strings function that you may use. Example
	//     usage: swap_strings(chunks[0], chunks[1]) will swap
	//	the contents of the first and second string.
}

// displays the strings in the global chunks variable
void display_chunks()
{
	char *p;

	int index = 0;

	// Calls for the first element address and then increment byte addressable memory by 1.
	p = &chunks[0][0] + 1;

	for (; index < NUM_OF_CHUNKS; index++) {

		// Print each chunks
		printf("%s", p);

		// Increment to the next chiper block
		p += CHUNK_LENGTH;
	}
}


int main()
{
	// reserve 8 bytes for future processing
	char input[(NUM_OF_CHUNKS * CHUNK_LENGTH) - (NUM_OF_CHUNKS * 2)];

	reset(chunks);

	printf("Please enter a phrase (Max %d Characters): \n", (NUM_OF_CHUNKS * CHUNK_LENGTH) - (NUM_OF_CHUNKS * 2));
	fgets(input, (NUM_OF_CHUNKS * CHUNK_LENGTH) - (NUM_OF_CHUNKS * 2), stdin);

	decompartmentalize(input, chunks);

	randomization_chunks();

	encrypt_chunks();

	sort_chunks();

	decrypt_chunks();

	display_chunks();

	return 0;
}
