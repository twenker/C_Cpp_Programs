//
//  parse.c
//  CSE340Project5
//
//  Created by Todd Wenker on 11/15/15.
//  Copyright © 2015 Todd Wenker. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "compiler.h"

// allocating memory (from project 4)
#define ALLOC(t) (t*) calloc(1, sizeof(t))

//function declaration
struct PrintStatement * parsePrint();
struct StatementNode * parseStmt();
struct AssignmentStatement * parseAssign();
struct IfStatement * parseIf();
struct GotoStatement * parseGoto();
struct ValueNode * parseRValue();
void parseVarList();
void addToVarList(struct ValueNode *);
int searchValueList(char *);
void parseVar();
struct ValueNode * parseRValue();
struct IfStatement * parseCase(struct ValueNode *);
struct StatementNode * parseSwitch();
struct IfStatement * parseWhile();

//////////////////////////Error//////////////////////////
void syntaxError(char * errorType, int errorNumber){
    printf("Error in %s statement\nError at %d\nCurrent token type: %d\nCurrent token: %s\n", errorType, errorNumber, t_type, token);
    exit(1);
}

//////////////////////////Parsing Variables (L and R values)//////////////////////////
//list of all variables that will be declared in the var_list
struct ValueNode * varList[10];
int varCount = 0;

//adds a var ValueNode to the varList. used during the parsing of the var_List section
void addToVarList(struct ValueNode * newVar){
    if(varCount <= 9){
        varList[varCount] = newVar;
        varCount++;
    }
    else{
        printf("varList is too small...");
    }
}
//searches the valueList and returns the index of the entered id. -1 if not found.
int searchValueList(char * searchId){
    int i = 0;
    while(varList[i] != NULL){
        if(strcmp(varList[i]->name, searchId) == 0){
            return i;
        }
        i++;
    }
    
    return -1;
}

//valueNode: char* id, int value. Parse the var list at the start of the program
void parseVar(){
    t_type = getToken();
    if(t_type == ID){
        struct ValueNode * newVar = ALLOC(struct ValueNode);
        newVar->name = strdup(token);
        newVar->value = 0;
        addToVarList(newVar);
    }
    else{
        syntaxError("VARLIST:VAR", 1);
    }
}

//var_section → id_list SEMICOLON
void parseVarList(){
    t_type = getToken();
    while(t_type != LBRACE){
        if(t_type == ID){
            ungetToken();
            parseVar();
            t_type = getToken();// to account for commas and semicolons
        }
        else{
            syntaxError("VARLIST", 1);
        }
        t_type = getToken();
    }
    ungetToken();
}

//Used to create a ValueNode for an R-Value or a value that lacks a place in memory (10, 7, 43...).
//sets the id of ValueNode to null?
struct ValueNode * parseRValue(){
    t_type = getToken();
    if(t_type == NUM){
        struct ValueNode * newRValue = ALLOC(struct ValueNode);
        newRValue->value = atoi(token);
        newRValue->name = NULL;
        return newRValue;
    }
    else{
        syntaxError("PARSE R VALUE", 1);
    }
    return NULL;
}

//////////////////////////Statement Nodes//////////////////////////

// iterate through a body and returns the last statement.
struct StatementNode * lastBodyStmt(struct StatementNode * body){
    while(body->next != NULL){
        body = body->next;
    }
    return body;
}

// parses body statements
struct StatementNode * parseBody(){
    t_type = getToken();
    if(t_type == LBRACE){
        
        struct StatementNode * bodyStmt = parseStmt();
        struct StatementNode * iterator = bodyStmt;
        t_type = getToken();
        
        // iterates through the body of a statement until it reaches the RBRACE
        while(t_type != RBRACE){
            ungetToken();
            if(iterator->type == IF_STMT){
                iterator->next = iterator->if_stmt->false_branch;
                iterator = iterator->next;
            }
            else{
                iterator->next = parseStmt();
                iterator = iterator->next;
                t_type = getToken();
            }
        }
        if(iterator->type == IF_STMT){
            
            iterator->next = iterator->if_stmt->false_branch;
            iterator = iterator->next;
        }
        return bodyStmt;
        
    }
    else{
        syntaxError("BODY", 1);
    }
    
    
    return NULL;
}

// Par
struct StatementNode * parseCaseBody(){
    t_type = getToken();
    if(t_type == LBRACE){
        
        struct StatementNode * bodyStmt = parseStmt();
        struct StatementNode * iterator = bodyStmt;
        t_type = getToken();
        
        // iterates through the body of a statement until it reaches the RBRACE
        while(t_type != RBRACE){
            ungetToken();
            iterator->next = parseStmt();
            iterator = iterator->next;
            t_type = getToken();
        }
        if(iterator->type == IF_STMT){
            
            iterator->next = iterator->if_stmt->false_branch;
            iterator = iterator->next;
        }
        return bodyStmt;
        
    }
    else{
        syntaxError("BODY", 1);
    }
    
    
    return NULL;
}

//print_stmt → PRINT ID SEMICOLON
struct PrintStatement * parsePrint(){
    t_type = getToken();
    if(t_type == PRINT){
        struct PrintStatement * ps = ALLOC(struct PrintStatement);
        
        t_type = getToken();
        if(t_type == ID){
            ps->id = varList[searchValueList(token)];
            t_type = getToken();
            if(t_type == SEMICOLON){
                return ps;
            }
            else{
                syntaxError("PRINT", 3);
            }
        }
        else{
            syntaxError("PRINT", 2);
        }
    }
    else{
        syntaxError("PRINT", 1);
    }
    return NULL;
}

// stmt → assign_stmt | print_stmt | while_stmt | if_stmt | switch_stmt
// statement types: Assign, Print, If, GoTo
struct StatementNode * parseStmt(){
    // to descern the diverence between standard IF, WHILE, and SWITCH statements. (0,1,2)
    int ifStmtType = -1;
    t_type = getToken();
    struct StatementNode * stmt = NULL;
    
    switch(t_type){
        case ID:
            stmt = ALLOC(struct StatementNode);
            stmt->type = ASSIGN_STMT;
            ungetToken();
            stmt->assign_stmt = parseAssign();
            break;
        case PRINT:
            stmt = ALLOC(struct StatementNode);
            stmt->type = PRINT_STMT;
            ungetToken();
            stmt->print_stmt = parsePrint();
            break;
        case IF:
            stmt = ALLOC(struct StatementNode);
            stmt->type = IF_STMT;
            ungetToken();
            stmt->if_stmt = parseIf();
            //ifStmtType = 0;
            break;
        case GOTO:
            stmt = ALLOC(struct StatementNode);
            stmt->type = GOTO_STMT;
            ungetToken();
            //stmt->goto_stmt = parseGoto();/////////////////////////////
            break;
        case SWITCH:
            stmt = ALLOC(struct StatementNode);
            stmt->type = IF_STMT;
            ungetToken();
            stmt = parseSwitch();
            ifStmtType = 2;
            break;
        case WHILE:
            stmt = ALLOC(struct StatementNode);
            stmt->type = IF_STMT;
            ungetToken();
            stmt->if_stmt = parseWhile();
            ifStmtType = 1;
            break;
        default:
            syntaxError("Stmt", 1);
            break;
    }
    
    // sets the final goto statement of the while loop
    if(ifStmtType == 1){
        struct StatementNode * iterator = stmt->if_stmt->true_branch;
        while(iterator->next != NULL){
            iterator = iterator->next;
        }
        iterator->next = ALLOC(struct StatementNode);
        iterator->next->type = GOTO_STMT;
        iterator->next->goto_stmt = ALLOC(struct GotoStatement);
        iterator->next->goto_stmt->target = stmt;
        
    }
    
    
    return stmt;
}

// assign_stmt → ID EQUAL primary SEMICOLON
// assign_stmt → ID EQUAL expr SEMICOLON
struct AssignmentStatement * parseAssign(){
    t_type = getToken();
    if(t_type == ID){
        struct AssignmentStatement * assignStmt = ALLOC(struct AssignmentStatement);
        assignStmt->left_hand_side = varList[searchValueList(token)];
        t_type = getToken();
        if(t_type == EQUAL){
            //Operand #1
            t_type = getToken();
            switch(t_type){
                case ID:
                    assignStmt->operand1 = varList[searchValueList(token)];
                    break;
                case NUM:
                    ungetToken();
                    assignStmt->operand1 = parseRValue();
                    break;
                default:
                    syntaxError("ASSIGN", 3);
                    break;
            }
            t_type = getToken();
            /*
             * If op is OP_NOOP then only operand1 is meaningful.
             * Otherwise op has to be one of the following values:
             * OP_PLUS, OP_MINUS, OP_MULT, OP_DIV
             * and both operands are meaningful
             */
            switch(t_type){
                case PLUS:
                    assignStmt->op = OP_PLUS;
                    break;
                case MINUS:
                    assignStmt->op = OP_MINUS;
                    break;
                case MULT:
                    assignStmt->op = OP_MULT;
                    break;
                case DIV:
                    assignStmt->op = OP_DIV;
                    break;
                default:
                    assignStmt->op = OP_NOOP;
                    return assignStmt;
                    break;
            }
            // Operand #2
            t_type = getToken();
            switch(t_type){
                case ID:
                    assignStmt->operand2 = varList[searchValueList(token)];
                    break;
                case NUM:
                    ungetToken();
                    assignStmt->operand2 = parseRValue();
                    break;
                default:
                    syntaxError("ASSIGN", 4);
                    break;
            }
            t_type = getToken(); // to account for semicolon
            return assignStmt;
            
        }
        else{
            syntaxError("ASSIGN", 2);
        }
    }
    else{
        syntaxError("ASSIGN", 1);
    }
    return NULL;
}


/* IFStatement structure
 struct ValueNode* condition_operand1;
 struct ValueNode* condition_operand2;
 
 
 * condition_op should be one of the following values:
 * CONDITION_GREATER, CONDITION_LESS, or CONDITION_NOTEQUAL
ConditionOpType condition_op;

struct StatementNode* true_branch;
struct StatementNode* false_branch;
*/

//if_stmt → IF condition body
//ConditionOp: CONDITION_GREATER, CONDITION_LESS, CONDITION_NOTEQUAL
/* EX
 IF b > a {
	print a;
	print b;
	print c;
	print d;
 }
*/

struct IfStatement * parseIf(){
    t_type = getToken();
    if(t_type == IF){
        t_type = getToken();
        struct IfStatement * ifStmt = ALLOC(struct IfStatement);
        // Operand 1
        switch(t_type){
            case ID:
                ifStmt->condition_operand1 = varList[searchValueList(token)];
                break;
            case NUM:
                ungetToken();
                ifStmt->condition_operand1 = parseRValue();
                break;
            default:
                syntaxError("IF", 2);
                break;
        }
        
        t_type = getToken();
        // condition_op should be one of the following values:
        // CONDITION_GREATER, CONDITION_LESS, or CONDITION_NOTEQUAL
        switch(t_type) {
            case GREATER:
                ifStmt->condition_op = CONDITION_GREATER;
                break;
            case LESS:
                ifStmt->condition_op = CONDITION_LESS;
                break;
            case NOTEQUAL:
                ifStmt->condition_op = CONDITION_NOTEQUAL;
                break;
            default:
                syntaxError("IF", 3);
                break;
        }
        t_type = getToken();
        // Operand 2
        switch(t_type){
            case ID:
                ifStmt->condition_operand2 = varList[searchValueList(token)];
                break;
            case NUM:
                ungetToken();
                ifStmt->condition_operand2 = parseRValue();
                break;
            default:
                syntaxError("IF", 4);
                break;
        }
        ///// BODY //////
        ifStmt->true_branch = parseBody();
        
        struct StatementNode * iterator = ifStmt->true_branch;
        while(iterator->next != NULL){
            iterator = iterator->next;
        }
        iterator->next = ALLOC(struct StatementNode);
        iterator->next->type = NOOP_STMT;
        ifStmt->false_branch = iterator->next;
        return ifStmt;
        
    }
    else{
        syntaxError("IF", 1);
    }
    
    return NULL;
}

/*
 while_stmt → WHILE condition body
 
 WHILE a > 0 {
 ...
 
 }
 
*/

struct IfStatement * parseWhile(){
    t_type = getToken();
    if(t_type == WHILE){
        t_type = getToken();
        struct IfStatement * whileStmt = ALLOC(struct IfStatement);
        // Operand 1
        switch(t_type){
            case ID:
                whileStmt->condition_operand1 = varList[searchValueList(token)];
                break;
            case NUM:
                ungetToken();
                whileStmt->condition_operand1 = parseRValue();
                break;
            default:
                syntaxError("WHILE", 2);
                break;
        }
        
        t_type = getToken();
        // condition_op should be one of the following values:
        // CONDITION_GREATER, CONDITION_LESS, or CONDITION_NOTEQUAL
        switch(t_type) {
            case GREATER:
                whileStmt->condition_op = CONDITION_GREATER;
                break;
            case LESS:
                whileStmt->condition_op = CONDITION_LESS;
                break;
            case NOTEQUAL:
                whileStmt->condition_op = CONDITION_NOTEQUAL;
                break;
            default:
                syntaxError("WHILE", 3);
                break;
        }
        t_type = getToken();
        // Operand 2
        switch(t_type){
            case ID:
                whileStmt->condition_operand2 = varList[searchValueList(token)];
                break;
            case NUM:
                ungetToken();
                whileStmt->condition_operand2 = parseRValue();
                break;
            default:
                syntaxError("WHILE", 4);
                break;
        }
        ///// BODY //////
        whileStmt->true_branch = parseBody();
        
        //goto statement set in the parseStmt
       
        // setting the NOOP statement
        whileStmt->false_branch = ALLOC(struct StatementNode);
        whileStmt->false_branch->type = NOOP_STMT;
        return whileStmt;
        
    }
    else{
        syntaxError("WHILE", 1);
    }
    
    return NULL;
}

/*
 switch_stmt → SWITCH ID LBRACE case_list RBRACE
 switch_stmt → SWITCH ID LBRACE case_list default_case RBRACE
 case_list → case case_list | case
 case → CASE NUM COLON body
 default_case → DEFAULT COLON body
*/

// Condition op 1 = ID after 'SWITCH'. Condition op 2 = NUM after 'CASE'. False branch = body after COLON,
// True branch = case/stmt afterwards. THERE IS NO EQUAL TO OPERATOR SO IT NEEDS TO BE REVERSED!!!!!!
struct IfStatement * parseCase(struct ValueNode * condOpId){
    t_type = getToken();
    if(t_type == CASE){
        t_type = getToken();
        if(t_type == NUM){
            ungetToken();
            struct IfStatement * caseStmt = ALLOC(struct IfStatement);
            caseStmt->condition_operand1 = condOpId;
            caseStmt->condition_operand2 = parseRValue();
            caseStmt->condition_op = CONDITION_NOTEQUAL;
            
            t_type = getToken();// account for COLON
            caseStmt->false_branch = parseCaseBody(); // NO EQUAL OP
            return caseStmt;
        }
        else{
            syntaxError("CASE", 2);
        }
    }
    else{
        syntaxError("CASE", 1);
    }
    return NULL;
}

//parses the switch statement but the body is left to case
struct StatementNode * parseSwitch(){
    t_type = getToken();
    if(t_type == SWITCH){
        t_type = getToken();
        struct ValueNode * condOpId = varList[searchValueList(token)];
        
        struct StatementNode * switchStmt = ALLOC(struct StatementNode);
        struct StatementNode * iterator = ALLOC(struct StatementNode);
        switchStmt->type = IF_STMT;
        
        t_type = getToken(); // to account for LBRACE
        switchStmt->if_stmt = parseCase(condOpId);
        iterator = switchStmt;
        t_type = getToken();
        while(t_type == CASE){
            ungetToken();
            iterator->next = ALLOC(struct StatementNode);
            iterator->next->type = IF_STMT;
            iterator->next->if_stmt = parseCase(condOpId);
            iterator->if_stmt->true_branch = iterator->next;
            iterator = iterator->next;
            
            t_type = getToken();
        }
        if(t_type == DEFAULT){
            t_type = getToken();// account for COLON
            iterator->next = parseBody();
            return  switchStmt;
            
        }
        else if(t_type ==LBRACE){
            iterator->next = ALLOC(struct StatementNode);
            iterator->next->type = NOOP_STMT;
            return switchStmt;
        }
    }
    else{
        syntaxError("SWITCH", 1);
    }
    
    return NULL;
}

//Complete parse tree generator
struct StatementNode* parse_generate_intermediate_representation(){
    
    // Var List. Creates ValueNodes that are stored into a
    parseVarList();
    
    // the intermedRep is the head of the statement parse tree
    struct StatementNode * intermedRep = parseBody();
    
    
    //returns the head of the statement parse tree
    return intermedRep;
    
}






