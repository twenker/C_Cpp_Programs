#include <stdio.h>
#include <string.h>
#include <ctype.h>

#pragma warning(disable: 4996)

#define max 100

struct contact {				       // a node to hold personal details
	char name[30];
	int phone;
	char email[30];
};

struct contact contactbook[max];	             // an array of structures, 100 entries
int tail = 0;					      // global variable

void flush();					      // forward declaration of functions
void branching(char c);
int insertion();
int print_all();
int search_contact();
int delete_contact();


int main() {  // print a menu for selection
	char ch = 'i';

	ungetc('\n', stdin); // inject input buffer with a return character

	do {
		printf("Enter your selection\n");
		printf("\ti: insert a new entry\n");
		printf("\td: delete an entry\n");
		printf("\ts: search an entry\n");
		printf("\tp: print all entries\n");
		printf("\tq: quit \n");

		flush();					// flush input buffer
		ch = tolower(getchar());
		branching(ch);
	} while (ch != 113);

	return 0;
}

void flush() {
	int c;
	do {
		c = getchar();
	} while (c != '\n' && c != EOF);
}


void branching(char c) {    // branch to different tasks
	switch (c) {
	case 'i':
		insertion();
		break;
	case 's':
		search_contact();
		break;
	case 'd':
		delete_contact();
		break;
	case 'p':
		print_all();
		break;
	case 'q':
		break;
	default:
		printf("Invalid input\n");
	}
}

int insertion() {    // insert a new entry at the end
	if (tail == max) {
		printf("There are no more places to insert.\n");
		return -1;
	}
	else {
		printf("Enter name, phone, email:\n");
		scanf("%s", contactbook[tail].name);
		// &contactbook[tail].name is an array. No "&" is needed
		scanf("%d", &contactbook[tail].phone, sizeof(contactbook[tail].phone));
		scanf("%s", contactbook[tail].email);
		tail++;
		printf("The number of entries = %d\n", tail);
		return 0;
	}
}

int print_all() {
// print name, phone, and email for each contact in the contactbook
	int i;

	if (tail == 0) {
		printf("No entries found.");
	}
	else {
		for (i = 0; i < tail; i++) {
			printf("\n\nname = %s\n", contactbook[i].name);
			printf("phone = %d\n", contactbook[i].phone);
			printf("email = %s\n", contactbook[i].email);
		}
	}

	return 0;
}

int search_contact() {      // print phone and email via name
	char sname[30]; int  i;

	printf("Please enter the name to be searched for:\n");
	scanf("%s", sname); 	//sname is an array, no & needed

	for (i = 0; i<tail; i++)
		if (strcmp(sname, contactbook[i].name) == 0) {
			printf("phone = %d\n", contactbook[i].phone);
			printf("email = %s\n", contactbook[i].email);
			return i;
		}

	printf("The name does not exist.\n");
	return -1;
}

int delete_contact() {
	int i, k;
	k = search_contact();
	if (k == -1) {
		printf("The name does not exist.\n"); return -1;
	}
	else {
		for (i = k; i<tail; i++) {
			strcpy(contactbook[i].name, contactbook[i + 1].name);
			contactbook[i].phone = contactbook[i + 1].phone;
			strcpy(contactbook[i].email, contactbook[i + 1].email);
			printf("The index deleted is: %d\n", k);
		}
		tail--;
		return k;
	}
}
