#include <stdio.h>
#include <stdlib.h>
#include "generator.h"
#include "symtable.h"

//FILE * codefp;
//char codeFilename[] = {"code"};

char * instructionString[] = { "LIT", "OPR", "LOD", "STO", "CAL", "INT", "JMP", "JPC", "WRT" };
int counter = 0;
int gi = 0;
int variableCount = 0;
int lastJmp = -1;
int lastStart = -1;

void generateCode(){
    setInstruction(counter, INT, 0, 3+getClassCount("Variable"));
    setupVariables();
    generateInstructions("main", 0);
}

void generateInstructions(char * str, int values){
    while ( strcmp(list[gi]->name, str) != 0 ) {
        printf("TRE %s %d\n", list[gi]->name, gi);
        if ( strcmp(list[gi]->name,"=") == 0 ){
            equalsOperation();
        }else if ( strcmp(list[gi]->name,"*") == 0 ){
            multiplyOperation();
        }else if ( strcmp(list[gi]->name,"return") == 0 ){
            returnOperation();
        }else if ( strcmp(list[gi]->name,"while") == 0 ){
            whileOperation();
        }else if ( !values && strcmp(list[gi]->name,"}") == 0 ){
            if ( lastJmp != -1 ){
                setInstruction(counter, JMP, 0, lastStart);
                setInstruction(lastJmp, JPC, 0, counter+1);
                lastJmp = -1;
                lastStart = -1;
            }
            gi++;
        } else
        gi++;
        if ( list[gi] == NULL ){
            gi++;
        } 
    }
}

void multiplyOperation(){
    gi--;
    if ( strcmp(list[gi]->sclass,"Number") == 0 ){
        setInstruction(counter, LIT, 0, atoi(list[gi]->value) );
    }
    if ( strcmp(list[gi]->sclass,"Variable") == 0 ){
        setInstruction(counter, LOD, 0, getVariablePos(list[gi]->name) );
    }
    removeFromList(gi);
    gi--;
    if ( strcmp(list[gi]->sclass,"Number") == 0 ){
        setInstruction(counter, LIT, 0, atoi(list[gi]->value) );
    }
    if ( strcmp(list[gi]->sclass,"Variable") == 0 ){
        setInstruction(counter, LOD, 0, getVariablePos(list[gi]->name) );
    }
    removeFromList(gi);
    
    setInstruction(counter, OPR, 0, MUL );
    listSet(gi, lookUp("E"));
    printf("VEREDITCO %s\n", list[gi]->name);
}

void equalsOperation(){
    gi--;
    if ( strcmp(list[gi]->sclass,"Number") == 0 ){
        setInstruction(counter, LIT, 0, atoi(list[gi]->value) );
    }
    removeFromList(gi);
    
    gi--;
    setInstruction(counter, STO, 0, getVariablePos(list[gi]->name) );
    removeFromList(gi);
    gi++;
    //printf("VEREDITCO %s\n", list[gi]->name);
}

void whileOperation(){
    gi++;
    lastStart = counter+1;
    generateInstructions("{", 1);
    lastJmp = counter;
    setInstruction(counter, JPC, 0, -1 );
    gi++;
}

void returnOperation(){
    gi--;
    setInstruction(counter, STO, 0, 0);
    removeFromList(gi);
    setInstruction(counter, OPR, 0, RTN);
    removeFromList(gi);
    //printf("VEREDITCO %s\n", list[gi]->name);
}

void setupVariables(){
    int i;
    for( i=0; i<LIST_SIZE; i++ ){
        if ( list[i] != NULL ){
            if ( strcmp(list[i]->sclass,"Variable") == 0 ){
                if ( getVariablePos(list[i]->name) == -1 ){
                    variables[variableCount].var = list[i];
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
        printf("%s %d %d\n", getInstructionName( instructions[i].operation ), instructions[i].level, instructions[i].argument);
    }
}

void populateStack(struct entry ** e){
    int i;
    for( i=0; i<LIST_SIZE; i++ ){
        list[i] = e[i];
    }
}

void removeFromList(int i){
    list[i] = NULL;
    shiftListLeft(i);
}

void listAdd(int i){
    shiftListRight(i);
    list[i] = NULL;
}

void listSet(int i, struct entry * e){
    list[i] = e;
}

void shiftListLeft(int pos){
    int i;
    for ( i = pos; i<LIST_SIZE-1; i++ ){
        list[i] = list[i+1];
    }
}

void shiftListRight(int pos){
    int i;
    for ( i = LIST_SIZE-1; i>pos; i-- ){
        list[i] = list[i-1];
    }
}

void printStack(){
    printf("PRINT STACK\n");
    int i;
    for( i=0; i<LIST_SIZE; i++ ){
        if ( list[i] != NULL ){
            printf("%s\n", list[i]->name);
        }
    }
}

char * getInstructionName(int code){
    if ( code >= 0 && code <INSTRUCTION_COUNT ){
        return instructionString[code];
    }
    return "UNKNOWN INSTRUCTION";
}