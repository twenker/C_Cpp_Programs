//  Todd Wenker
//  CSE240 Chen
//  hw5.c
//  10/5/14

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#pragma warning(disable: 4996)

typedef enum{
    Final = 0,Midterm,Quiz,Project,Homework
}GradeType;

typedef struct Entry{
    char name[255];
    float possible_points;
    float points_earned;
    GradeType grade_type;
}Entry;

struct Entry GradeBook[10];
int count = 0;

void insert_Entry();
void delete_Entry();
void print_GradeBook();
int main() {
	char ch;
	do {
		printf("\ti: insert a new entry\n");
        printf("\td: delete an entry\n");
        printf("\tp: print all entries\n");
        printf("\tq: quit \n");
        printf("Enter your selection\n");
		scanf("%c",&ch);

        switch(ch){
            case 'i':
                insert_Entry();
                break;
            case 'd':
                delete_Entry();
                break;
            case 'p':
                print_GradeBook();
                break;
            case 'q':
                break;
            default:
                printf("Incorrect command\n");
        }
	} while (ch != 'q');

	return 0;
}

void insert_Entry(){
    char name[255];
    float pp;
    float pe;
    int gradeType;

    printf("Enter a name: ");
    scanf("%s", name);
    printf("Enter the possible points: ");
    scanf("%f", &pp);
    printf("Enter the points earned: ");
    scanf("%f", &pe);
    printf("Enter the grade type(Final=0,Midterm=1,Quiz=2,Project=3,Homework=4): ");
    scanf("%d", &gradeType);

    strcpy(GradeBook[count].name, name);
    GradeBook[count].possible_points=pp;
    GradeBook[count].points_earned=pe;
    if(gradeType == 0){
        GradeBook[count].grade_type = Final;
    }
    if(gradeType == 1){
        GradeBook[count].grade_type = Midterm;
    }
    if(gradeType == 2){
        GradeBook[count].grade_type = Project;
    }
    if(gradeType == 3){
        GradeBook[count].grade_type = Quiz;
    }
    if(gradeType == 4){
        GradeBook[count].grade_type = Homework;
    }
    ++count;
    printf("There are %d entries", count);
    return;
}

void delete_Entry(){
    int i;
    char name[255];
    printf("Enter a name to delete:\n");
    scanf("%s", name);

    for(i=0;i<9;++i){
        if(strcmp(name, GradeBook[i].name) ==1){
            strcpy(GradeBook[i].name, GradeBook[i+1].name);
            GradeBook[i].possible_points = GradeBook[i+1].possible_points;
            GradeBook[i].points_earned = GradeBook[i+1].points_earned;
            GradeBook[i].grade_type = GradeBook[i+1].grade_type;
            printf("The grade entry has been deleted. ");
            count--;
            return;
        }
    }
    printf("Grade not found");
    return;
}

void print_GradeBook(){
    float totalGrade = 0;
    int i;
    for(i=0;i<count;++i){
        printf("Name:\t%s\n", GradeBook[i].name);
        printf("Possible Points:\t%f\n", GradeBook[i].possible_points);
        printf("Points Earned:\t%f\n", GradeBook[i].points_earned);
        printf("Grade Type:\t%d\n", GradeBook[i].grade_type);
        if(GradeBook[i].grade_type == Final){
            totalGrade += 0.22 * (GradeBook[i].points_earned/GradeBook[i].possible_points);
        }
        if(GradeBook[i].grade_type == 1){
            totalGrade += 0.18 * (GradeBook[i].points_earned/GradeBook[i].possible_points);
        }
        if(GradeBook[i].grade_type == 2){
            totalGrade += 0.1 * (GradeBook[i].points_earned/GradeBook[i].possible_points);
        }
        if(GradeBook[i].grade_type == 3){
            totalGrade += 0.2 * (GradeBook[i].points_earned/GradeBook[i].possible_points);
        }
        if(GradeBook[i].grade_type == 4){
            totalGrade += 0.3 * (GradeBook[i].points_earned/GradeBook[i].possible_points);
        }
    }
    printf("Final Grade:\t%f", totalGrade);
    return;
}
