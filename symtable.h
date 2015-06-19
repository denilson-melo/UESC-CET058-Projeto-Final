#ifndef SYMTABLE_H
#define SYMTABLE_H

#define TABLE_SIZE 1024
#define TOKEN_SIZE 32

struct entry{                               
    char * key;
    char * sclass;
    char * name;
    char * value;
    struct entry * next;
};

char token[TOKEN_SIZE]; 
struct entry hashTable[TABLE_SIZE];

int hash (char *c);                         
void insertEntry (char *c,  char *type);   
struct entry * lookUp(char *c);
//void printTable(int format);
int getClassCount(char *);
void printTable();
void freeTable();

#endif