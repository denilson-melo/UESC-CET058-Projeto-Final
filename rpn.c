#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "rpn.h"

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
    k *= treeLevelSize;
    for ( i; i<12; i++){
        generateRpn((k+i));
    }
    k /= treeLevelSize;
    nprElements[aux] = aTree[k];
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
}