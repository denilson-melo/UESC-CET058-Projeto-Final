#ifndef GENERATOR_H
#define GENERATOR_H

#define INSTRUCTION_COUNT 9
#define LIST_SIZE 1024
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
struct entry * list[LIST_SIZE];

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

void generateInstructions();
void multiplyOperation();
void equalsOperation();
void whileOperation();
void returnOperation();

void populateStack(struct entry **);
void printStack();
void removeFromList(int);
void shiftListLeft(int);
void shiftListRight(int);
void listAdd(int);
void listSet(int, struct entry *);
char * getInstructionName(int);
#endif