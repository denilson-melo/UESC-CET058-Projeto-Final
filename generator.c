#include <stdio.h>
#include <stdlib.h>
#include "generator.h"
#include "symtable.h"

//FILE * codefp;
//char codeFilename[] = {"code"};

char * instructionString[] = { "LIT", "OPR", "LOD", "STO", "CAL", "INT", "JMP", "JPC", "WRT" };
int counter = 0;
int li = 0;
int variableCount = 0;
int body = -1;
int condition = -1;
int conditionJpc = -1;
int increment = -1;
int incrementJmp = -1;
int control;

void generateCode(){
    setInstruction(counter, INT, 0, 3+getClassCount("Variable"));
    setupVariables();
    generateInstructions("main");
}

void generateInstructions(char * str){
    while ( strcmp(list[li]->name, str) != 0 ) {
        //printf("TRE %s %d\n", list[li]->name, li);
        if ( listCompare(li,"=") ){
            equalsOperation();
        }else if ( listCompare(li,"*") ){
            multiplyOperation();
        }else if ( listCompare(li,"+") ){
            sum();
        }else if ( listCompare(li,"return") ){
            returnOperation();
        }else if ( listCompare(li,"while") ){
            whileOperation();
        }else if ( listCompare(li,"if") ){
            ifOperation();
        }else if ( listCompare(li,"for") ){
            forOperation();
        }else if ( control != NONE && listCompare(li,"}") ){
            if ( control == WHILE ){
                setInstruction(counter, JMP, 0, condition+1);
                setInstruction(conditionJpc, JPC, 0, counter+1);
                counter -= 1;
            }
            if ( control == IF ){
                setInstruction(conditionJpc, JPC, 0, counter+1);
                counter -= 1;
            }
            if ( control == FOR ){
                printStack(); printf("TRETA %d\n", counter);
                setInstruction(counter, JMP, 0, increment+1);
                setInstruction(incrementJmp, JMP, 0, condition+1);
                setInstruction(conditionJpc, JPC, 0, counter);
                setInstruction(conditionJpc+1, JMP, 0, body+1);
                counter -= 3;
            }
            control = NONE;
            li++;
        } else
        li++;
        if ( list[li] == NULL ){
            li++;
        } 
    }
}

void getOperand(){
    if ( strcmp(list[li]->sclass,"Number") == 0 ){
        setInstruction(counter, LIT, 0, atoi(list[li]->value) );
    }
    if ( strcmp(list[li]->sclass,"Variable") == 0 ){
        setInstruction(counter, LOD, 0, getVariablePos(list[li]->name) );
    }
}

void binaryOperation(){
    li--;
    getOperand();
    removeFromList(li);
    li--;
    getOperand();
    removeFromList(li);
}

void multiplyOperation(){
    binaryOperation();
    setInstruction(counter, OPR, 0, MUL );
    listSet(li, lookUp("E"));
}

void sum(){
    binaryOperation();
    setInstruction(counter, OPR, 0, ADD );
    listSet(li, lookUp("E"));
}

void equalsOperation(){
    li--;
    getOperand();
    removeFromList(li);
    li--;
    setInstruction(counter, STO, 0, getVariablePos(list[li]->name) );
    removeFromList(li);
    li++;
    //printf("VEREDITCO %s\n", list[li]->name);
}

void whileOperation(){
    li++;
    condition = counter;
    control = WHILE;
    generateInstructions("{");
    conditionJpc = counter;
    setInstruction(counter, JPC, 0, -1 );
    li++;
}

void ifOperation(){
    li++;
    control = IF;
    generateInstructions("{");
    conditionJpc = counter;
    setInstruction(counter, JPC, 0, -1 );
    li++;
}

void forOperation(){
    li++;
    generateInstructions(";");
    control = FOR;
    li++;
    condition = counter;
    generateInstructions(";");
    conditionJpc = counter;
    setInstruction(counter, JPC, 0, -1 );
    setInstruction(counter, JMP, 0, -1 );
    
    increment = counter;
    generateInstructions("{");
    incrementJmp = counter;
    setInstruction(counter, JMP, 0, -1 );
    body = counter;
    li++;
}

void returnOperation(){
    li--;
    getOperand();
    setInstruction(counter, STO, 0, 0);
    removeFromList(li);
    setInstruction(counter, OPR, 0, RTN);
    removeFromList(li);
    //printf("VEREDITCO %s\n", list[li]->name);
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
        printf("%d: %s %d %d\n", i+1,getInstructionName( instructions[i].operation ), instructions[i].level, instructions[i].argument);
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

int listCompare(int i, char * str){
    if ( strcmp(list[i]->name,str)==0 ){
        return 1;
    }
    return 0;
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