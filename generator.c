#include <stdio.h>
#include <stdlib.h>
#include "generator.h"
#include "symtable.h"

//FILE * codefp;
//char codeFilename[] = {"code"};

int counter = 0;
int gi = 0;
int variableCount = 0;

void generateCode(){
    setInstruction(counter, INT, 0, 3+getClassCount("Variable"));
    setupVariables();
    while ( genStack[gi] != NULL ) {
        if ( strcmp(genStack[gi]->name,"=") == 0 ){
            equalsOperation();
        }
        if ( strcmp(genStack[gi]->name,"*") == 0 ){
            multiplyOperation();
        }
        if ( strcmp(genStack[gi]->name,"return") == 0 ){
            returnOperation();
        }
        gi++;
    }
    
}

void multiplyOperation(){
    int i = gi;
    
    i--;
    if ( strcmp(genStack[i]->sclass,"Number") == 0 ){
        setInstruction(counter, LIT, 0, atoi(genStack[i]->value) );
    }
    if ( strcmp(genStack[i]->sclass,"Variable") == 0 ){
        setInstruction(counter, LOD, 0, getVariablePos(genStack[i]->name) );
    }
    i--;
    if ( strcmp(genStack[i]->sclass,"Number") == 0 ){
        setInstruction(counter, LIT, 0, atoi(genStack[i]->value) );
    }
    if ( strcmp(genStack[i]->sclass,"Variable") == 0 ){
        setInstruction(counter, LOD, 0, getVariablePos(genStack[i]->name) );
    }
    setInstruction(counter, OPR, 0, MUL );
}

void equalsOperation(){
    int i = gi;
    i--;
    if ( strcmp(genStack[i]->sclass,"Number") == 0 ){
        setInstruction(counter, LIT, 0, atoi(genStack[i]->value) );
    }
    i--;
    setInstruction(counter, STO, 0, getVariablePos(genStack[i]->name) );
}

void returnOperation(){
    int i = gi;
    i--;
    setInstruction(counter, STO, 0, 0);
    setInstruction(counter, OPR, 0, RTN);
}

void setupVariables(){
    int i;
    for( i=0; i<GEN_STACK_SIZE; i++ ){
        if ( genStack[i] != NULL ){
            if ( strcmp(genStack[i]->sclass,"Variable") == 0 ){
                if ( getVariablePos(genStack[i]->name) == -1 ){
                    variables[variableCount].var = genStack[i];
                    variables[variableCount].pos = 3+variableCount;
                    variableCount++;
                }
            }
        }
    }
}

int getVariablePos(char * str){
    int i;
    for ( i=0; i<VARIABLE_MAX; i++){
        if ( variables[i].var != NULL ){
            if ( strcmp( variables[i].var->name, str ) == 0 ){
                return variables[i].pos;
            }
        }
    }
    return -1;
}


void setInstruction(int pos, int opr, int lvl, int arg){
    instructions[pos].operation = opr;
    instructions[pos].level = lvl;
    instructions[pos].argument = arg;
    counter++;
}

void printInstructions(){
    int i;
    printf("PRINT INSTRUCTIONS\n");
    for( i=0; i<counter; i++ ){
        printf("%d %d %d\n", instructions[i].operation, instructions[i].level, instructions[i].argument);
    }
}

void populateStack(struct entry ** e){
    int i;
    for( i=0; i<GEN_STACK_SIZE; i++ ){
        genStack[i] = e[i];
    }
}

void printStack(){
    printf("PRINT STACK\n");
    int i;
    for( i=0; i<GEN_STACK_SIZE; i++ ){
        if ( genStack[i] != NULL ){
            printf("%s\n", genStack[i]->name);
        }
    }
}