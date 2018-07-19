//
//  symbols.h
//  cse340_Project3
//
//  Created by Todd Wenker on 10/2/15.

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <ctype.h>

#ifndef Header_h
#define Header_h

#define KEYWORDS  5
#define RESERVED  26

using namespace std;

//------------------------------------------------------------//
//lexer.h
//------------------------------------------------------------//

typedef enum  { END_OF_FILE = -1,
    IF = 1,
    WHILE,
    DO,
    THEN,
    PRINT,
    PLUS,
    MINUS,
    DIV,
    MULT,
    EQUAL,
    COLON,
    COMMA,
    SEMICOLON,
    LBRAC,
    RBRAC,
    LPAREN,
    RPAREN,
    NOTEQUAL,
    GREATER,
    LESS,
    LTEQ,
    GTEQ,
    DOT,
    NUM,
    ID,
    HASH,
    DOUBLEHASH,
    ARROW,
    EPSILON,
    ERROR } token_type;

extern string current_token;                  // token string
extern token_type t_type;                     // token type
extern int active_token;                      // active token

//------------------------------------------------------------//
//lexer.c
//------------------------------------------------------------//

static string reserved[] =
{   "",
    "IF",
    "WHILE",
    "DO",
    "THEN",
    "PRINT",
    "+",
    "-",
    "/",
    "*",
    "=",
    ":",
    ",",
    ";",
    "[",
    "]",
    "(",
    ")",
    "<>",
    ">",
    "<",
    "<=",
    ">=",
    ".",
    "NUM",
    "ID",
    "HASH",
    "DOUBLEHASH",
    "ARROW",
    "ERROR"
};

static void skip_space()
{
    char c;
    
    c = getchar();
    while (!feof(stdin) && isspace(c))
    {
        c = getchar();
    }
    // return character to input buffer if eof is not reached
    if (!feof(stdin))
    {
        ungetc(c,stdin);
    }
}


static int is_keyword(string s)
{
    int i;
    
    for (i = 1; i <= KEYWORDS; i++)
    {
        if (reserved[i].compare(s) == 0)
        {
            return i;
        }
    }
    return 0;
}

static token_type scan_number()
{
    char c;
    
    c = getchar();
    if (isdigit(c))
    {
        if (c == '0')
        {
            current_token  += c;
        }
        else
        {
            while (isdigit(c))
            {
                current_token += c;
                c = getchar();
            }
            ungetc(c, stdin);
        }
        current_token += '\0';
        return NUM;
    }
    else
    {
        return ERROR;
    }
}

static token_type scan_id_keyword()
{
    char c;
    int k;
    token_type the_type;
    
    c = getchar();
    if (isalpha(c))
    {
        while (isalnum(c))
        {
            current_token += c;
            c = getchar();
        }
        current_token += '\0';
        ungetc(c, stdin);
        k = is_keyword(current_token);
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
    
    if (active_token)
    {
        active_token = 0;
        return t_type;
    }
    skip_space();
    current_token = '\0';
    c = getchar();
    switch (c)
    {
        case '.': current_token = c; t_type = DOT;        return t_type;
        case '+': current_token = c; t_type = PLUS;       return t_type;
        case '/': current_token = c; t_type = DIV;        return t_type;
        case '*': current_token = c; t_type = MULT;       return t_type;
        case '=': current_token = c; t_type = EQUAL;      return t_type;
        case ':': current_token = c; t_type = COLON;      return t_type;
        case ',': current_token = c; t_type = COMMA;      return t_type;
        case ';': current_token = c; t_type = SEMICOLON;  return t_type;
        case '[': current_token = c; t_type = LBRAC;      return t_type;
        case ']': current_token = c; t_type = RBRAC;      return t_type;
        case '(': current_token = c; t_type = LPAREN;     return t_type;
        case ')': current_token = c; t_type = RPAREN;     return t_type;
        // HASH and DOUBLEHASH
        case '#':
            c = getchar();
            if(c == '#'){
                t_type = DOUBLEHASH;
            }
            else{
                ungetc(c,stdin);
                t_type = HASH;
            }
            return t_type;
        // MINUS and ARROW
        case '-':
            current_token = c;
            c = getchar();
            if(c == '>'){
                current_token += c;
                t_type = ARROW;
            }
            else{
                ungetc(c, stdin);
                t_type = MINUS;
            }
            return t_type;
            
        case '<':
            current_token = c;
            c = getchar();
            if (c == '=')
            {
                t_type = LTEQ;
            }
            else if (c == '>')
            {
                t_type = NOTEQUAL;
            }
            else
            {
                ungetc(c, stdin);
                t_type = LESS;
            }
            return t_type;
        case '>':
            c = getchar();
            if (c == '=')
            {
                t_type = GTEQ;
            }
            else
            {
                ungetc(c, stdin);
                t_type = GREATER;
            }
            return t_type;
        default:
            if (isdigit(c))
            {
                ungetc(c, stdin);
                t_type = scan_number();
            }
            else if (isalpha(c)) // token is either keyword or ID
            {
                ungetc(c, stdin);
                t_type = scan_id_keyword();
            }
            else if (c == EOF)
            {
                t_type = END_OF_FILE;
            }
            else
            {
                t_type = ERROR;
            }
            return t_type;
    }
}

void ungetToken()
{
    active_token = 1;
}


//------------------------------------------------------------//
// Symbol
//------------------------------------------------------------//

struct symbol{
    string name;
    token_type t_type;
    bool isNonTerminal;
};

//extern variables;

extern vector <symbol *> nonTerminalList;
extern vector < vector <symbol *> > cfgMatrix;
extern vector < vector <string> > firstSets;
extern vector < vector <string> > followSets;


// checks non terminal vector if a symbol is a non terminal
int isNonTerminal(string s){
    for(int i = 0; i < nonTerminalList.size(); i++){
        if(nonTerminalList[i]->name.compare(s) == 0){
            return i;
        }
    }
    return -1;
}


// will detect spaces and remove them
int detectSpace(){
    //whether or not a space has been reached
    int x = 0;
    
    char c;
    c = getchar();
    while (!feof(stdin) && isspace(c))
    {
        c = getchar();
        x = 1;
    }
    // return character to input buffer if eof is not reached
    if (!feof(stdin))
    {
        ungetc(c,stdin);
    }
    return x;
}

//creates a symbol
symbol * createNonTerminal(){
    symbol * x = new symbol;
    x->name = current_token;
    x->t_type = t_type;
    x->isNonTerminal = true;
    return x;
}

//creates a symbol
symbol * createTerminal(){
    symbol * x = new symbol;
    x->name = current_token;
    x->t_type = t_type;
    x->isNonTerminal = false;
    return x;
}

void printNonTerminals(){
    for(int i = 0; i < nonTerminalList.size(); i++){
        cout << i << ": " << nonTerminalList[i]->name << endl;
    }
}


void printCFGMatrix(){
    cout << "CFG Matrix: " << endl;
    for(int i = 0; i < cfgMatrix.size(); i++){
        for(int j = 0; j < cfgMatrix[i].size(); j++){
            cout << "row: "<< i << ", column: " << j << " " << cfgMatrix[i][j]->name << endl;
        }
    }
}

void unionSet(){
    
}

void addFirstSet(int i,string s){
    int index = isNonTerminal(cfgMatrix[i][0]->name);
    firstSets[index].push_back(s);
    sort(firstSets[index].begin(), firstSets[index].end());
}

void unionFirstSets(int i, int j){
    set_union(firstSets[i].begin(), firstSets[i].end(), firstSets[j].begin(), firstSets[j].end(), firstSets[i]);
}

void addFollowSet(int i,string s){
    
}

void unionFollowSets(int i, int j){
    set_union(followSets[i].begin(), followSets[i].end(), followSets[j].begin(), followSets[j].end(), followSets[i]);
}

//calculating FIRST sets
//1. If ε belongs in the set, represent it as #
//2. If ε belongs in the set, it should be listed before any other elements
//3.All other elements of the set should be sorted in dictionary order (specifically, the C function
//      strcmp from string.h defines the sorting order)
//---------------------------------------------------//
// 1. FIRST(x) = {x} if is term
// 2. FIRST(eps) = {eps}
// 3. If A -> B is a production rule, then union FIRST(B) - {eps} with FIRST(A)
// 4. if A->B0 B1 B2... Bi Bi+1 ... Bk and epsilon is in FIRST(B0...i) then add FIRST(Bi+1) - {eps}
//          to FIRST(A)
// 5. if A-> B0 ... Bk and epsilon is in FIRST(B0...k) than add {eps} to FIRST(A)

void firstSet(){
    
    //calculating the first sets
    firstSets.resize(nonTerminalList.size());
    bool change = true;
    while(change){
        change = false;
        for(int i = 0; i < cfgMatrix.size(); i++){
            
            int currentNonTerminalIndex = isNonTerminal(cfgMatrix[i][0]->name);
            
            if(!(cfgMatrix[i][1]->isNonTerminal)){
                
                if(find(firstSets[currentNonTerminalIndex].begin(),
                        firstSets[currentNonTerminalIndex].end(), cfgMatrix[i][1]) !=
                        firstSets[currentNonTerminalIndex].end())
                {
                    
                    addFirstSet(i, cfgMatrix[i][1]->name);
                    change = true;
                }
            }
            else{
                if(equal(firstSets[currentNonTerminalIndex].begin(),
                         firstSets[currentNonTerminalIndex].end(),
                         firstSets[isNonTerminal(cfgMatrix[i][1]->name)]))
                {
                    
                    unionFirstSets(isNonTerminal(cfgMatrix[i][0]->name),
                                   isNonTerminal(cfgMatrix[i][1]->name));
                    change = true;
                }
            }
        }
    }
    // printing the first set matrix
    for(int i = 0; i < firstSets.size(); i++){
        cout << "FIRST(" << nonTerminalList[i] << ") = { ";
        for(int j = 0; j < firstSets[i].size()-1; j++){
            cout << firstSets[i][j] << ", ";
        }
        cout << firstSets[i][firstSets[i].size()-1] << " }" << endl;
    }
}

//calculating FOLLOW sets
//

void followSet(){
    
}


#endif

