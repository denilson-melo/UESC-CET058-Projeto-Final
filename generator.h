#ifndef GENERATOR_H
#define GENERATOR_H

#define GEN_STACK_SIZE 1024
#define VARIABLE_MAX 12
#define CODE_SIZE 1024

enum { LIT, OPR, LOD, STO, CAL, INT, JMP, JPC, WRT } instructionCode;
enum { RTN, NEG, ADD, SUB, MUL, DIV, ODD, MOD, EQL, NEQ, LSS, LEQ, GTR, GEQ } operationCode;

struct instruction{
    int operation;
    int level;
    int argument;
};
struct instruction instructions[CODE_SIZE];
struct entry * genStack[GEN_STACK_SIZE];

struct variable{
    struct entry * var;
    int pos;
};
struct variable variables[VARIABLE_MAX];

void setupVariables();
int getVariablePos(char *);


void generateCode();
void setInstruction(int,int,int,int);
void printInstructions();

void multiplyOperation();
void equalsOperation();
void returnOperation();

void populateStack(struct entry **);
void printStack();

#endif