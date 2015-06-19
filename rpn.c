#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "rpn.h"
#include "symtable.h"
#include "generator.h"

struct entry * nprElements[ NPR_STRING_SIZE ];
int aux = 0;

void generateRpn(int k){
    int i;
    if ( k==0 ){
        i=1;
    }else{
        i=0;
    }
    if ( aTree[k] == NULL ){
        return;
    }
    
    if ( strcmp(aTree[k]->name, "while") == 0 ){
        nprElements[aux] = aTree[k];
        aux++;
    }
    k *= treeLevelSize;
    for ( i; i<12; i++){
        generateRpn((k+i));
    }
    k /= treeLevelSize;
    if ( strcmp(aTree[k]->name, "while") != 0 ){
        nprElements[aux] = aTree[k];
    }
    //nprElements[aux] = aTree[k];
    aux++;
}

void printRpn(){
    int i;
    printf("\n");
    printf("SUPER TESTE VOANDO AQUI\n");
    for ( i=0; i<NPR_STRING_SIZE; i++ ){
        if ( nprElements[i]!=NULL ){
            printf("%d : %s\n", i, nprElements[i]->name);
        }
    }
    populateStack(nprElements);
}