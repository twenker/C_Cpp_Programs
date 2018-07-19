//  Todd Wenker
//  CSE240 Chen
//  hw6.c
//  10/5/14

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#pragma warning(disable: 4996)

typedef struct Item{
    char name[255];
    float price;
    int quantity;
    struct Item *next;
    };
struct Item *ShoppingList = NULL;
void insert_item();
void delete_item();
void print_ShoppingList();
int main(){
    char ch;
	do {
		printf("\ti: insert a new item\n");
        printf("\td: delete an item\n");
        printf("\tp: print shopping list\n");
        printf("\tq: quit \n");
        printf("Enter your selection\n");
		scanf("%c",&ch);

        switch(ch){
            case 'i':
                insert_item();
                break;
            case 'd':
                delete_item();
                break;
            case 'p':
                print_ShoppingList();
                break;
            case 'q':
                break;
            default:
                printf("Incorrect command\n");
        }
        fflush(stdout);
	} while (ch != 'q');

	return 0;
}
void insert_item(){
    struct Item p;
    char name[255];
    int x;
    float y;
    printf("Enter the name of the item: \n");
    scanf("%s", name);
    printf("Enter the quantity: \n");
    scanf("%d", &x);
    printf("Enter price: \n");
    scanf("%f", &y);
    strcpy(p.name, name);
    p.quantity = x;
    p.price = y;
    if(ShoppingList == NULL||strcmp(ShoppingList->name, p.name) > 0){
        p.next = ShoppingList;
        ShoppingList = &p;
        return;
    }
    else{
        struct Item *temp = ShoppingList;
        while(temp->next != NULL){
            if(strcmp(p.name, temp->next->name) <= 0){
                p.next = temp->next;
                temp->next = &p;
                return;
            }
            temp = temp->next;
        }
    }

}
void delete_item(){
    struct Item *temp = ShoppingList;
    char name[255];
    printf("Enter the name of the item to delete: \n");
    scanf("%s", name);
    if(strcmp(temp->name, name) == 0){
        temp = temp->next;
        free(ShoppingList);
        ShoppingList = temp;
        return;
    }
    while(temp != NULL){
        if(strcmp(temp->next->name, name) == 0){
            free(temp->next);
            temp->next = temp->next->next;
            return;
        }
        temp = temp->next;
    }
}
void print_ShoppingList(){
    struct Item *temp = ShoppingList;
    char name[255];
    int x;
    float y;
    while (temp != NULL){
        strcpy(name, temp->name);
        x = temp->quantity;
        y = temp->price;
        printf("Item: %s\n", name);
        printf("Quantity: %d\n", x);
        printf("Price: $%f\n\n", y);
        temp = temp->next;
    }
    return;
}
