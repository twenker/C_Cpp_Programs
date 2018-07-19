/*----------------------------------------------------------------------------
 Note: the code in this file is not to be shared with anyone or posted online.
 (c) Rida Bazzi, 2015, Adam Doupe, 2015
 
 CSE 340 
 Todd Wenker
 10/30/15
 ----------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "syntax.h"

/* ------------------------------------------------------- */
/* -------------------- LEXER SECTION -------------------- */
/* ------------------------------------------------------- */

#define KEYWORDS  11

typedef enum
{
    END_OF_FILE = -1, VAR = 1, WHILE, INT, REAL, STRING, BOOLEAN,
    TYPE, LONG, DO, CASE, SWITCH,
    PLUS, MINUS, DIV, MULT, EQUAL,
    COLON, COMMA, SEMICOLON,
    LBRAC, RBRAC, LPAREN, RPAREN, LBRACE, RBRACE,
    NOTEQUAL, GREATER, LESS, LTEQ, GTEQ, DOT,
    ID, NUM, REALNUM,
    ERROR
} token_type;

const char *reserved[] = {"",
    "VAR", "WHILE", "INT", "REAL", "STRING", "BOOLEAN",
    "TYPE", "LONG", "DO", "CASE", "SWITCH",
    "+", "-", "/", "*", "=",
    ":", ",", ";",
    "[", "]", "(", ")", "{", "}",
    "<>", ">", "<", "<=", ">=", ".",
    "ID", "NUM", "REALNUM",
    "ERROR"
};

// Global Variables associated with the next input token
#define MAX_TOKEN_LENGTH 100
char token[MAX_TOKEN_LENGTH]; // token string
token_type t_type; // token type
bool activeToken = false;
int tokenLength;
int line_no = 1;

void skipSpace()
{
    char c;
    
    c = getchar();
    line_no += (c == '\n');
    while (!feof(stdin) && isspace(c))
    {
        c = getchar();
        line_no += (c == '\n');
    }
    ungetc(c, stdin);
}

int isKeyword(char *s)
{
    int i;
    
    for (i = 1; i <= KEYWORDS; i++)
    {
        if (strcmp(reserved[i], s) == 0)
        {
            return i;
        }
    }
    return false;
}

/*
 * ungetToken() simply sets a flag so that when getToken() is called
 * the old t_type is returned and the old token is not overwritten.
 * NOTE: BETWEEN ANY TWO SEPARATE CALLS TO ungetToken() THERE MUST BE
 * AT LEAST ONE CALL TO getToken(). CALLING TWO ungetToken() WILL NOT
 * UNGET TWO TOKENS
 */
void ungetToken()
{
    activeToken = true;
}

token_type scan_number()
{
    char c;
    
    c = getchar();
    if (isdigit(c))
    {
        // First collect leading digits before dot
        // 0 is a NUM by itself
        if (c == '0')
        {
            token[tokenLength] = c;
            tokenLength++;
            token[tokenLength] = '\0';
        }
        else
        {
            while (isdigit(c))
            {
                token[tokenLength] = c;
                tokenLength++;
                c = getchar();
            }
            ungetc(c, stdin);
            token[tokenLength] = '\0';
        }
        // Check if leading digits are integer part of a REALNUM
        c = getchar();
        if (c == '.')
        {
            c = getchar();
            if (isdigit(c))
            {
                token[tokenLength] = '.';
                tokenLength++;
                while (isdigit(c))
                {
                    token[tokenLength] = c;
                    tokenLength++;
                    c = getchar();
                }
                token[tokenLength] = '\0';
                if (!feof(stdin))
                {
                    ungetc(c, stdin);
                }
                return REALNUM;
            }
            else
            {
                ungetc(c, stdin);
                c = '.';
                ungetc(c, stdin);
                return NUM;
            }
        }
        else
        {
            ungetc(c, stdin);
            return NUM;
        }
    }
    else
    {
        return ERROR;
    }
}

token_type scan_id_or_keyword()
{
    token_type the_type;
    int k;
    char c;
    
    c = getchar();
    if (isalpha(c))
    {
        while (isalnum(c))
        {
            token[tokenLength] = c;
            tokenLength++;
            c = getchar();
        }
        if (!feof(stdin))
        {
            ungetc(c, stdin);
        }
        token[tokenLength] = '\0';
        k = isKeyword(token);
        if (k == 0)
        {
            the_type = ID;
        }
        else
        {
            the_type = (token_type) k;
        }
        return the_type;
    }
    else
    {
        return ERROR;
    }
}

token_type getToken()
{
    char c;
    
    if (activeToken)
    {
        activeToken = false;
        return t_type;
    }
    skipSpace();
    tokenLength = 0;
    c = getchar();
    switch (c)
    {
        case '.': return DOT;
        case '+': return PLUS;
        case '-': return MINUS;
        case '/': return DIV;
        case '*': return MULT;
        case '=': return EQUAL;
        case ':': return COLON;
        case ',': return COMMA;
        case ';': return SEMICOLON;
        case '[': return LBRAC;
        case ']': return RBRAC;
        case '(': return LPAREN;
        case ')': return RPAREN;
        case '{': return LBRACE;
        case '}': return RBRACE;
        case '<':
            c = getchar();
            if (c == '=')
            {
                return LTEQ;
            }
            else if (c == '>')
            {
                return NOTEQUAL;
            }
            else
            {
                ungetc(c, stdin);
                return LESS;
            }
        case '>':
            c = getchar();
            if (c == '=')
            {
                return GTEQ;
            }
            else
            {
                ungetc(c, stdin);
                return GREATER;
            }
        default:
            if (isdigit(c))
            {
                ungetc(c, stdin);
                return scan_number();
            }
            else if (isalpha(c))
            {
                ungetc(c, stdin);
                return scan_id_or_keyword();
            }
            else if (c == EOF)
            {
                return END_OF_FILE;
            }
            else
            {
                return ERROR;
            }
    }
}

/* ----------------------------------------------------------------- */
/* -------------------- SYNTAX ANALYSIS SECTION -------------------- */
/* ----------------------------------------------------------------- */

void syntax_error(const char* msg)
{
    printf("Syntax error while parsing %s line %d\n", msg, line_no);
    exit(1);
}

/* -------------------- PRINTING PARSE TREE -------------------- */
void print_parse_tree(struct programNode* program)
{
    print_decl(program->decl);
    print_body(program->body);
}

void print_decl(struct declNode* dec)
{
    if (dec->type_decl_section != NULL)
    {
        print_type_decl_section(dec->type_decl_section);
    }
    if (dec->var_decl_section != NULL)
    {
        print_var_decl_section(dec->var_decl_section);
    }
}

void print_body(struct bodyNode* body)
{
    printf("{\n");
    print_stmt_list(body->stmt_list);
    printf("}\n");
}

void print_var_decl_section(struct var_decl_sectionNode* varDeclSection)
{
    printf("VAR\n");
    if (varDeclSection->var_decl_list != NULL)
    {
        print_var_decl_list(varDeclSection->var_decl_list);
    }
}

void print_var_decl_list(struct var_decl_listNode* varDeclList)
{
    print_var_decl(varDeclList->var_decl);
    if (varDeclList->var_decl_list != NULL)
    {
        print_var_decl_list(varDeclList->var_decl_list);
    }
}

void print_var_decl(struct var_declNode* varDecl)
{
    print_id_list(varDecl->id_list);
    printf(": ");
    print_type_name(varDecl->type_name);
    printf(";\n");
}

void print_type_decl_section(struct type_decl_sectionNode* typeDeclSection)
{
    printf("TYPE\n");
    if (typeDeclSection->type_decl_list != NULL)
    {
        print_type_decl_list(typeDeclSection->type_decl_list);
    }
}

void print_type_decl_list(struct type_decl_listNode* typeDeclList)
{
    print_type_decl(typeDeclList->type_decl);
    if (typeDeclList->type_decl_list != NULL)
    {
        print_type_decl_list(typeDeclList->type_decl_list);
    }
}

void print_type_decl(struct type_declNode* typeDecl)
{
    print_id_list(typeDecl->id_list);
    printf(": ");
    print_type_name(typeDecl->type_name);
    printf(";\n");
}

void print_type_name(struct type_nameNode* typeName)
{
    if (typeName->type != ID)
    {
        printf("%s ", reserved[typeName->type]);
    }
    else
    {
        printf("%s ", typeName->id);
    }
}

void print_id_list(struct id_listNode* idList)
{
    printf("%s ", idList->id);
    if (idList->id_list != NULL)
    {
        printf(", ");
        print_id_list(idList->id_list);
    }
}

void print_stmt_list(struct stmt_listNode* stmt_list)
{
    print_stmt(stmt_list->stmt);
    if (stmt_list->stmt_list != NULL)
    {
        print_stmt_list(stmt_list->stmt_list);
    }
    
}

void print_assign_stmt(struct assign_stmtNode* assign_stmt)
{
    printf("%s ", assign_stmt->id);
    printf("= ");
    print_expression_prefix(assign_stmt->expr);
    printf("; \n");
}

void print_stmt(struct stmtNode* stmt)
{
    switch (stmt->stmtType)
    {
        case ASSIGN:
            print_assign_stmt(stmt->assign_stmt);
            break;
        case WHILE:
            print_while_stmt(stmt->while_stmt);
            break;
        case DO:
            print_do_stmt(stmt->while_stmt);
            break;
        case SWITCH:
            print_switch_stmt(stmt->switch_stmt);
            break;
    }
}

void print_expression_prefix(struct exprNode* expr)
{
    if (expr->tag == EXPR)
    {
        printf("%s ", reserved[expr->op]);
        print_expression_prefix(expr->leftOperand);
        print_expression_prefix(expr->rightOperand);
    }
    else if (expr->tag == PRIMARY)
    {
        if (expr->primary->tag == ID)
        {
            printf("%s ", expr->primary->id);
        }
        else if (expr->primary->tag == NUM)
        {
            printf("%d ", expr->primary->ival);
        }
        else if (expr->primary->tag == REALNUM)
        {
            printf("%.4f ", expr->primary->fval);
        }
    }
}

void print_while_stmt(struct while_stmtNode* while_stmt)
{
    // TODO: implement this for your own debugging purposes
    printf("WHILE");
    print_condition(while_stmt->condition);
    printf("\n");
    print_body(while_stmt->body);
}

void print_do_stmt(struct while_stmtNode* do_stmt)
{
    // TODO: implement this for your own debugging purposes
    printf("DO");
    print_body(do_stmt->body);
    printf("WHILE");
    print_condition(do_stmt->condition);
    printf("SEMICOLON");
}

void print_primary(struct primaryNode * primary){
    // 0 INT, 1 REALNUM, 2 ID
    if(primary->tag == 0){
        printf("%d", primary->ival);
    }
    else if(primary->tag == 1){
        printf("%f", primary->fval);
    }
    else if(primary->tag == 2){
        printf("%s", primary->id);
    }
    else{
        printf("error");
    }
}

void print_condition(struct conditionNode* condition)
{
    // TODO: implement this for your own debugging purposes
    if(condition->relop == -1){
        print_primary(condition->left_operand);
        
    }
    else{
        print_primary(condition->left_operand);
        
        // -1 is no relop, 0 >, 1 >=, 2 <, 3 <>, 4 <=
        if(condition->relop == 0){
            printf(" GREATER ");
        }
        else if (condition->relop == 1){
            printf(" GTEQ ");
        }
        else if (condition->relop == 2){
            printf(" LESS ");
        }
        else if (condition->relop == 3){
            printf(" NOTEQUEAL ");
        }
        else if(condition->relop == 4){
            printf(" LTEQ ");
        }
        
        print_primary(condition->right_operand);
    }
}

void print_case(struct caseNode* cas)
{
    // TODO: implement this for your own debugging purposes
    printf("CASE ");
    printf("NUM ");
    printf("COLON ");
    print_body(cas->body);
}

void print_case_list(struct case_listNode* case_list)
{
    // TODO: implement this for your own debugging purposes
    print_case(case_list->cas);
    if(case_list->case_list != NULL){
        printf("\n");
        print_case_list(case_list->case_list);
    }
    
}

void print_switch_stmt(struct switch_stmtNode* switc)
{
    // TODO: implement this for your own debugging purposes
    printf("SWITCH");
    printf("%s", switc->id);
    printf("LBRACE");
    print_case_list(switc->case_list);
    printf("RBRACE");
    
}

/* -------------------------------- ERRORS --------------------------------- */

void type0Error(char* id){
    printf("ERROR CODE 0 %s", id);
    exit(1);
}

void type1Error(char* id){
    printf("ERROR CODE 1 %s", id);
    exit(1);
}

void type2Error(char* id){
    printf("ERROR CODE 2 %s", id);
    exit(1);
}

void type3Error(){
    printf("ERROR CODE 3 %d", line_no);
    exit(1);
}

void type4Error(char* id){
    printf("ERROR CODE 4 %s", id);
    exit(1);
}

void noError(){
    printf("All systems go!");
}

/* --------------------------------- Other -------------------------------- */

int relopType(){
    // -1 is no relop, 0 >, 1 >=, 2 <, 3 <>, 4 <=
    
    if(t_type == GREATER){
        return 0;
    }
    else if(t_type == GTEQ){
        return 1;
    }
    else if(t_type == LESS){
        return 2;
    }
    else if(t_type == NOTEQUAL){
        return 3;
    }
    else if(t_type == LTEQ){
        return 4;
    }
    else{
        return -1;
    }
}

bool isExplicitType(token_type t){
    if(t == INT || t == REAL || t == BOOLEAN || t == STRING || t == LONG){
        return true;
    }
    else{
        return false;
    }
    
}

typedef enum{
    T1 = 1,
    T2,
    T3,
    T4,
    T5,
    T6,
    T7,
    T8,
    T9
}implicitType;

int impicitTypeCount = 1;

implicitType newImplicitType(){
    implicitType implType = impicitTypeCount;
    impicitTypeCount++;
    return implType;
}

struct type{
    char* id;
    token_type explType;
    implicitType implType;
};

struct var{
    char* id;
    token_type explType;
    struct type * varType;
};

int varCount = 0;
int typeCount = 0;
struct var * varList[25];
struct type * typeList[25];

//update all the entries in the type list and var listthat have an implicit type into an explicit type
void updateImplicit(implicitType impType, token_type expType){
    int i;
    for(i = 0; typeList[i] != NULL; i++){
        if(typeList[i]->implType == impType){
            int j;
            for(j = 0; varList[j] != NULL; j++){
                if(varList[j]->varType == typeList[i]){
                    varList[j]->explType = expType;
                }
            }
            typeList[i]->explType = expType;
            typeList[i]->implType = 0;
        }
    }
    
}


int varIdExists(char* id){
    int i;
    for(i = 0; i < varCount && varList[i] != NULL; i++){
        if(strcmp(id, varList[i]->id) == 0){
            return i;
        }
    }
    return -1;
}

int typeIdExists(char* id){
    int i;
    for(i = 0; i < typeCount && typeList[i] != NULL; i++){
        if(strcmp(id, typeList[i]->id) == 0){
            return i;
        }
    }
    return -1;
}

void createTypes(struct id_listNode * id_l){
    
    if(typeIdExists(id_l->id) == -1){
        struct type * newType = ALLOC(struct type);
        newType->id = strdup(id_l->id);
        if(typeIdExists(token) == -1 && t_type == ID){
            struct type * secondType = ALLOC(struct type);
            secondType->id = strdup(token);
            secondType->implType = newImplicitType();
            newType->implType = secondType->implType;
            typeList[typeCount] = newType;
            typeCount++;
            typeList[typeCount] = secondType;
            typeCount++;
        }
        else if(typeIdExists(token) != -1 && isExplicitType(typeList[typeIdExists(token)]->explType)){
            newType->explType = typeList[typeIdExists(token)]->explType;
            typeList[typeCount] = newType;
            typeCount++;
        }
        else if(typeIdExists(token) != -1 && !isExplicitType(typeList[typeIdExists(token)]->explType)){
            newType->implType = typeList[typeIdExists(token)]->implType;
            typeList[typeCount] = newType;
            typeCount++;
        }
        else{
            newType->explType = t_type;
            typeList[typeCount] = newType;
            typeCount++;
        }
        
    }
    // type 0 error: type name more than once
    else{
        type0Error(id_l->id);
    }
    
    if(id_l->id_list != NULL){
        createTypes(id_l->id_list);
    }
    
    return;
    
}


void createVars(struct id_listNode * id_l){
    
    if(varIdExists(id_l->id) == -1 && typeIdExists(id_l->id) == -1){
        struct var * newVar = ALLOC(struct var);
        newVar->id = strdup(id_l->id);
        
        if(typeIdExists(token) == -1){
            if(t_type == ID && varIdExists(token) == -1){
                struct type * newType = ALLOC(struct type);
                newType->id = strdup(token);
                newType->implType = newImplicitType();
                newVar->varType = newType;
                typeList[typeCount] = newType;
                typeCount++;
                varList[varCount] = newVar;
                varCount++;
            }
            else if(isExplicitType(t_type)){
                newVar->explType = t_type;
                varList[varCount] = newVar;
                
            }
            // type 4 error: var declared as type name
            else{
                type4Error(token);
            }
        }
        else{
            newVar->varType = typeList[typeIdExists(token)];
            varList[varCount] = newVar;
            varCount++;
        }
    }
    // type 1 error: type name as variable
    else if(typeIdExists(id_l->id) != -1){
        type1Error(id_l->id);
    }
    // type 2 error: var declared more than once
    else if(varIdExists(id_l->id) != -1){
        type2Error(id_l->id);
    }
    if(id_l->id_list != NULL){
        createVars(id_l->id_list);
    }
    return;
}



/* -------------------- PARSING AND BUILDING PARSE TREE -------------------- */

// Note that the following function is not
// called case because case is a keyword in C/C++
int caseCount = 0;
struct caseNode* cas()
{
    // TODO: implement this for EC
    struct caseNode * cs;
    t_type =getToken();
    if(t_type == CASE){
        t_type = getToken();
        if(t_type == NUM){
            t_type = getToken();
            if(t_type == COLON){
                t_type = getToken();
                if(t_type == LBRACE){
                    ungetToken();
                    cs = ALLOC(struct caseNode);
                    cs->body = body();
                    cs->num = caseCount;
                    caseCount++;
                    return cs;
                }
                else{
                    syntax_error("case. Expect body");
                }
            }
            else{
                syntax_error("case. Expect COLON");
            }
        }
        else{
            syntax_error("case. Expect NUM");
        }
    }
    else{
        syntax_error("case. Expect CASE");
    }
    
    return NULL;
}

struct case_listNode* case_list()
{
    // TODO: implement this for EC
    struct case_listNode * case_lis;
    t_type = getToken();
    
    if(t_type == CASE){
        ungetToken();
        case_lis = ALLOC(struct case_listNode);
        case_lis->cas = cas();
        
        t_type = getToken();
        if(t_type == CASE){
            ungetToken();
            case_lis->case_list = case_list();
            return case_lis;
        }
        else{
            return case_lis;
        }
    }
    else{
        syntax_error("case_list. Expect case");
    }
    
    return NULL;
}

struct switch_stmtNode* switch_stmt()
{
    // TODO: implement this for EC
    struct switch_stmtNode * switch_st;
    t_type = getToken();
    
    if(t_type == SWITCH){
        t_type = getToken();
        if(t_type == ID){
            switch_st = ALLOC(struct switch_stmtNode);
            switch_st->id = strdup(token);
            t_type = getToken();
            if(t_type == LBRACE){
                t_type = getToken();
                if(t_type == CASE){
                    ungetToken();
                    switch_st->case_list = case_list();
                    t_type = getToken();
                    if(t_type == RBRACE){
                        return switch_st;
                    }
                    else{
                        syntax_error("switch_stmt. Expect RBRACE");
                    }
                }
                else{
                    syntax_error("switch_stmt. Expect case_list");
                }
            }
            else{
                syntax_error("switch_stmt. Expect LBRACE");
            }
        }
        else{
            syntax_error("switch_stmt. Expect ID");
        }
    }
    else{
        syntax_error("switch_stmt. Expect SWITCH");
    }
    return NULL;
}

struct while_stmtNode* do_stmt()
{
    // TODO: implement this
    struct while_stmtNode * do_st;
    t_type = getToken();
    
    if(t_type == DO){
        t_type = getToken();
        
        if(t_type == LBRACE) {
            ungetToken();
            do_st = ALLOC(struct while_stmtNode);
            do_st->body = body();
            t_type = getToken();
            if(t_type == WHILE){
                t_type = getToken();
                if(t_type == NUM || t_type == REALNUM || t_type == ID){
                    ungetToken();
                    do_st->condition = condition();
                    t_type = getToken();
                    if(t_type == SEMICOLON){
                        return do_st;
                    }
                    else{
                        printf("do. Expect SEMICOLON");
                    }
                    
                }
                else{
                    syntax_error("do. Expect condition");
                }
            }
            else{
                syntax_error("do. Expect WHILE");
            }
        }
    }
    else{
        syntax_error("do. Expect DO");
    }
    
    
    return NULL;
}

struct primaryNode* primary()
{
    // TODO: implement this
    struct  primaryNode * prim;
    t_type = getToken();
    
    // 0 NUM, 1 REALNUM or 2 ID
    if(t_type == NUM) {
        prim = ALLOC(struct primaryNode);
        prim->tag = 0;
        prim->ival = atoi(token);
    }
    else if(t_type == REALNUM) {
        prim = ALLOC(struct primaryNode);
        prim->tag = 1;
        prim->fval = atof(token);
    }
    else if(t_type == ID) {
        prim = ALLOC(struct primaryNode);
        prim->tag = 2;
        prim->id = strdup(token);
    }
    else{
        syntax_error("primary. Expect NUM, REALNUM, or ID");
    }
    
    return prim;
}

struct conditionNode* condition()
{
    // TODO: implement this
    struct conditionNode * cond;
    
    t_type = getToken();
    if(t_type == ID || t_type == NUM || t_type == REALNUM){
        cond = ALLOC(struct conditionNode);
        
        ungetToken();
        cond->left_operand = primary();
        
        t_type = getToken();
        // -1 is no relop, 0 >, 1 >=, 2 <, 3 <>, 4 <=
        if(t_type == GREATER || t_type == GTEQ || t_type == LESS || t_type == NOTEQUAL || t_type == LTEQ){
            cond->relop = relopType();
            cond->right_operand = primary();
            
            if(cond->left_operand->tag != cond->right_operand->tag){
                type3Error();
            }
            if(varIdExists(cond->left_operand->id) != -1 && varIdExists(cond->right_operand->id) != -1){
                if(varList[varIdExists(cond->left_operand->id)]->explType != varList[varIdExists(cond->right_operand->id)]->explType || varList[varIdExists(cond->left_operand->id)]->varType != varList[varIdExists(cond->right_operand->id)]->varType ){
                    type3Error();
                }
            }
            
        }
        else{
            cond->relop = -1;
            if(cond->left_operand->id == NULL){
                type3Error();
            }
            ungetToken();
        }
    }
    else{
        syntax_error("condition. Expect NUM, REALNUM, or ID");
    }
    if(cond->relop == -1 && varList[varIdExists(cond->left_operand->id)]->varType->explType != BOOLEAN){
        type3Error();
    }

    return cond;
}



struct while_stmtNode* while_stmt()
{
    // TODO: implement this
    struct while_stmtNode * while_st;
    
    t_type = getToken();
    
    if(t_type == WHILE){
        while_st = ALLOC(struct while_stmtNode);
        while_st->condition = condition();
        while_st->body = body();
        
        return while_st;
    }
    else{
        syntax_error("while_stmt. WHILE expected");
    }
    
    return NULL;
}

struct exprNode* factor()
{
    struct exprNode* facto;
    
    t_type = getToken();
    if (t_type == LPAREN)
    {
        facto = expr();
        t_type = getToken();
        if (t_type == RPAREN)
        {
            return facto;
        }
        else
        {
            syntax_error("factor. RPAREN expected");
        }
    }
    else if (t_type == NUM)
    {
        facto = ALLOC(struct exprNode);
        facto->primary = ALLOC(struct primaryNode);
        facto->tag = PRIMARY;
        facto->op = NOOP;
        facto->leftOperand = NULL;
        facto->rightOperand = NULL;
        facto->primary->tag = NUM;
        facto->primary->ival = atoi(token);
        return facto;
    }
    else if (t_type == REALNUM)
    {
        facto = ALLOC(struct exprNode);
        facto->primary = ALLOC(struct primaryNode);
        facto->tag = PRIMARY;
        facto->op = NOOP;
        facto->leftOperand = NULL;
        facto->rightOperand = NULL;
        facto->primary->tag = REALNUM;
        facto->primary->fval = atof(token);
        return facto;
    }
    else if (t_type == ID)
    {
        facto = ALLOC(struct exprNode);
        facto->primary = ALLOC(struct primaryNode);
        facto->tag = PRIMARY;
        facto->op = NOOP;
        facto->leftOperand = NULL;
        facto->rightOperand = NULL;
        facto->primary->tag = ID;
        facto->primary->id = strdup(token);
        return facto;
    }
    else
    {
        syntax_error("factor. NUM, REALNUM, or ID, expected");
    }
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct exprNode* term()
{
    struct exprNode* ter;
    struct exprNode* f;
    
    t_type = getToken();
    if (t_type == ID || t_type == LPAREN || t_type == NUM || t_type == REALNUM)
    {
        ungetToken();
        f = factor();
        t_type = getToken();
        if (t_type == MULT || t_type == DIV)
        {
            ter = ALLOC(struct exprNode);
            ter->op = t_type;
            ter->leftOperand = f;
            ter->rightOperand = term();
            ter->tag = EXPR;
            ter->primary = NULL;
            return ter;
        }
        else if (t_type == SEMICOLON || t_type == PLUS ||
                 t_type == MINUS || t_type == RPAREN)
        {
            ungetToken();
            return f;
        }
        else
        {
            syntax_error("term. MULT or DIV expected");
        }
    }
    else
    {
        syntax_error("term. ID, LPAREN, NUM, or REALNUM expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct exprNode* expr()
{
    struct exprNode* exp;
    struct exprNode* t;
    
    t_type = getToken();
    if (t_type == ID || t_type == LPAREN || t_type == NUM || t_type == REALNUM)
    {
        ungetToken();
        t = term();
        t_type = getToken();
        if (t_type == PLUS || t_type == MINUS)
        {
            exp = ALLOC(struct exprNode);
            exp->op = t_type;
            exp->leftOperand = t;
            exp->rightOperand = expr();
            exp->tag = EXPR;
            exp->primary = NULL;
            return exp;
        }
        else if (t_type == SEMICOLON || t_type == MULT ||
                 t_type == DIV || t_type == RPAREN)
        {
            ungetToken();
            return t;
        }
        else
        {
            syntax_error("expr. PLUS, MINUS, or SEMICOLON expected");
        }
    }
    else
    {
        syntax_error("expr. ID, LPAREN, NUM, or REALNUM expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct assign_stmtNode* assign_stmt()
{
    struct assign_stmtNode* assignStmt;
    
    t_type = getToken();
    if (t_type == ID)
    {
        assignStmt = ALLOC(struct assign_stmtNode);
        assignStmt->id = strdup(token);
        t_type = getToken();
        if (t_type == EQUAL)
        {
            assignStmt->expr = expr();
            t_type = getToken();
            if (t_type == SEMICOLON)
            {
                if(typeIdExists(assignStmt->id) != -1){
                    type1Error(assignStmt->id);
                }
                
                return assignStmt;
            }
            else
            {
                syntax_error("asign_stmt. SEMICOLON expected");
            }
        }
        else
        {
            syntax_error("assign_stmt. EQUAL expected");
        }
    }
    else
    {
        syntax_error("assign_stmt. ID expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct stmtNode* stmt()
{
    struct stmtNode* stm;
    
    stm = ALLOC(struct stmtNode);
    t_type = getToken();
    if (t_type == ID) // assign_stmt
    {
        ungetToken();
        stm->assign_stmt = assign_stmt();
        stm->stmtType = ASSIGN;
    }
    else if (t_type == WHILE) // while_stmt
    {
        ungetToken();
        stm->while_stmt = while_stmt();
        stm->stmtType = WHILE;
    }
    else if (t_type == DO)  // do_stmt
    {
        ungetToken();
        stm->while_stmt = do_stmt();
        stm->stmtType = DO;
    }
    else if (t_type == SWITCH) // switch_stmt
    {
        ungetToken();
        stm->switch_stmt = switch_stmt();
        stm->stmtType = SWITCH;
    }
    else
    {
        syntax_error("stmt. ID, WHILE, DO or SWITCH expected");
    }
    return stm;
}

struct stmt_listNode* stmt_list()
{
    struct stmt_listNode* stmtList;
    
    t_type = getToken();
    if (t_type == ID || t_type == WHILE ||
        t_type == DO || t_type == SWITCH)
    {
        ungetToken();
        stmtList = ALLOC(struct stmt_listNode);
        stmtList->stmt = stmt();
        t_type = getToken();
        if (t_type == ID || t_type == WHILE ||
            t_type == DO || t_type == SWITCH)
        {
            ungetToken();
            stmtList->stmt_list = stmt_list();
            return stmtList;
        }
        else // If the next token is not in FOLLOW(stmt_list),
            // let the caller handle it.
        {
            ungetToken();
            stmtList->stmt_list = NULL;
            return stmtList;
        }
    }
    else
    {
        syntax_error("stmt_list. ID, WHILE, DO or SWITCH expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct bodyNode* body()
{
    struct bodyNode* bod;
    
    t_type = getToken();
    if (t_type == LBRACE)
    {
        bod = ALLOC(struct bodyNode);
        bod->stmt_list = stmt_list();
        t_type = getToken();
        if (t_type == RBRACE)
        {
            return bod;
        }
        else
        {
            syntax_error("body. RBRACE expected");
        }
    }
    else
    {
        syntax_error("body. LBRACE expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct type_nameNode* type_name()
{
    struct type_nameNode* tName;
    
    tName = ALLOC(struct type_nameNode);
    t_type = getToken();
    if (t_type == ID || t_type == INT || t_type == REAL ||
        t_type == STRING || t_type == BOOLEAN || t_type == LONG)
    {
        tName->type = t_type;
        if (t_type == ID)
        {
            tName->id = strdup(token);
        }
        else
        {
            tName->id = NULL;
        }
        return tName;
    }
    else
    {
        syntax_error("type_name. type name expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct id_listNode* id_list()
{
    struct id_listNode* idList;
    
    idList = ALLOC(struct id_listNode);
    t_type = getToken();
    if (t_type == ID)
    {
        idList->id = strdup(token);
        t_type = getToken();
        if (t_type == COMMA)
        {
            idList->id_list = id_list();
            return idList;
        }
        else if (t_type == COLON)
        {
            ungetToken();
            idList->id_list = NULL;
            return idList;
        }
        else
        {
            syntax_error("id_list. COMMA or COLON expected");
        }
    }
    else
    {
        syntax_error("id_list. ID expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct type_declNode* type_decl()
{
    struct type_declNode* typeDecl;
    
    typeDecl = ALLOC(struct type_declNode);
    t_type = getToken();
    if (t_type == ID)
    {
        ungetToken();
        typeDecl->id_list = id_list();
        t_type = getToken();
        if (t_type == COLON)
        {
            typeDecl->type_name = type_name();
            createTypes(typeDecl->id_list);
            t_type = getToken();
            if (t_type == SEMICOLON)
            {
                return typeDecl;
            }
            else
            {
                syntax_error("type_decl. SEMICOLON expected");
            }
        }
        else
        {
            syntax_error("type_decl. COLON expected");
        }
    }
    else
    {
        syntax_error("type_decl. ID expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct var_declNode* var_decl()
{
    struct var_declNode* varDecl;
    
    varDecl = ALLOC(struct var_declNode);
    t_type = getToken();
    if (t_type == ID)
    {
        ungetToken();
        varDecl->id_list = id_list();
        t_type = getToken();
        if (t_type == COLON)
        {
            varDecl->type_name = type_name();
            createVars(varDecl->id_list);
            t_type = getToken();
            if (t_type == SEMICOLON)
            {
                return varDecl;
            }
            else
            {
                syntax_error("var_decl. SEMICOLON expected");
            }
        }
        else
        {
            syntax_error("var_decl. COLON expected");
        }
    }
    else
    {
        syntax_error("var_decl. ID expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct var_decl_listNode* var_decl_list()
{
    struct var_decl_listNode* varDeclList;
    
    varDeclList = ALLOC(struct var_decl_listNode);
    t_type = getToken();
    if (t_type == ID)
    {
        ungetToken();
        varDeclList->var_decl = var_decl();
        t_type = getToken();
        if (t_type == ID)
        {
            ungetToken();
            varDeclList->var_decl_list = var_decl_list();
            return varDeclList;
        }
        else
        {
            ungetToken();
            varDeclList->var_decl_list = NULL;
            return varDeclList;
        }
    }
    else
    {
        syntax_error("var_decl_list. ID expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct type_decl_listNode* type_decl_list()
{
    struct type_decl_listNode* typeDeclList;
    
    typeDeclList = ALLOC(struct type_decl_listNode);
    t_type = getToken();
    if (t_type == ID)
    {
        ungetToken();
        typeDeclList->type_decl = type_decl();
        t_type = getToken();
        if (t_type == ID)
        {
            ungetToken();
            typeDeclList->type_decl_list = type_decl_list();
            return typeDeclList;
        }
        else
        {
            ungetToken();
            typeDeclList->type_decl_list = NULL;
            return typeDeclList;
        }
    }
    else
    {
        syntax_error("type_decl_list. ID expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct var_decl_sectionNode* var_decl_section()
{
    struct var_decl_sectionNode *varDeclSection;
    
    varDeclSection = ALLOC(struct var_decl_sectionNode);
    t_type = getToken();
    if (t_type == VAR)
    {
        // no need to ungetToken()
        varDeclSection->var_decl_list = var_decl_list();
        return varDeclSection;
    }
    else
    {
        syntax_error("var_decl_section. VAR expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct type_decl_sectionNode* type_decl_section()
{
    struct type_decl_sectionNode *typeDeclSection;
    
    typeDeclSection = ALLOC(struct type_decl_sectionNode);
    t_type = getToken();
    if (t_type == TYPE)
    {
        typeDeclSection->type_decl_list = type_decl_list();
        return typeDeclSection;
    }
    else
    {
        syntax_error("type_decl_section. TYPE expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct declNode* decl()
{
    struct declNode* dec;
    
    dec = ALLOC(struct declNode);
    dec->type_decl_section = NULL;
    dec->var_decl_section = NULL;
    t_type = getToken();
    if (t_type == TYPE)
    {
        ungetToken();
        dec->type_decl_section = type_decl_section();
        t_type = getToken();
        if (t_type == VAR)
        {
            // type_decl_list is epsilon
            // or type_decl already parsed and the
            // next token is checked
            ungetToken();
            dec->var_decl_section = var_decl_section();
        }
        else
        {
            ungetToken();
            dec->var_decl_section = NULL;
        }
        return dec;
    }
    else
    {
        dec->type_decl_section = NULL;
        if (t_type == VAR)
        {
            // type_decl_list is epsilon
            // or type_decl already parsed and the
            // next token is checked
            ungetToken();
            dec->var_decl_section = var_decl_section();
            return dec;
        }
        else
        {
            if (t_type == LBRACE)
            {
                ungetToken();
                dec->var_decl_section = NULL;
                return dec;
            }
            else
            {
                syntax_error("decl. LBRACE expected");
            }
        }
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

struct programNode* program()
{
    struct programNode* prog;
    
    prog = ALLOC(struct programNode);
    t_type = getToken();
    if (t_type == TYPE || t_type == VAR || t_type == LBRACE)
    {
        ungetToken();
        prog->decl = decl();
        prog->body = body();
        return prog;
    }
    else
    {
        syntax_error("program. TYPE or VAR or LBRACE expected");
    }
    assert(false);
    return NULL; // control never reaches here, this is just for the sake of GCC
}

int main()
{
    struct programNode* parseTree;
    parseTree = program();
    // TODO: remove the next line after you complete the parser
    print_parse_tree(parseTree); // This is just for debugging purposes
    // TODO: do type checking & print output according to project specification
    noError();
    return 0;
}
