#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtable.h"

int hash( char * c ){
    int i = 0;
    int sum = 0;
    int len = strlen(c);
    for (i=0 ; i<len; i++){
        sum += (int)c[i];
    }
    return sum%TABLE_SIZE;
}

struct entry * lookUp( char *c ){
    int index = hash(c);
    struct entry * nav;
    nav = &hashTable[index];
    
    if ( nav->key != NULL ){
        if ( strcmp(c,nav->name) == 0 ){			
            return nav;
        }else{
            nav = nav->next;
            while ( nav != NULL ){
                nav = nav->next;
                if ( strcmp(c,nav->name) == 0 ){					
                    return nav;
                }
            }
        }
    }
    return NULL;
}

int getClassCount(char * str){
    int i;
    int count;
    struct entry * nav;
    for ( i=0; i < TABLE_SIZE; i++){
        if ( hashTable[i].key != NULL ){
            if ( strcmp(str, hashTable[i].sclass) == 0 ){
                count ++;
            }
        }
        if ( hashTable[i].next != NULL){
            nav = hashTable[i].next;
            while( nav != NULL ){
                if ( strcmp(str, hashTable[i].sclass) == 0 ){
                    count ++;
                }
                nav = nav->next;
            }
        }
    }
    return count;
}

void printTable(){
    int i;
    struct entry * nav;
    for ( i=0; i < TABLE_SIZE; i++){
        if ( hashTable[i].key != NULL ){
            printf( "Key: %s | Name: %s | Class: %s | Value: %s \n", hashTable[i].key, hashTable[i].name, hashTable[i].sclass, hashTable[i].value);
        }
        if ( hashTable[i].next != NULL){
            nav = hashTable[i].next;
            while( nav != NULL ){
                printf( "Key: %s | Name: %s | Class: %s | Value: %s \n", nav->key, nav->name, nav->sclass, nav->value);
                nav = nav->next;
            }
        }
    }
}

void insertEntry( char *c, char *type ){
    struct entry * entryAddress;
    struct entry * navAddress;
    int index = hash(c);                                    //Generating the hash
    entryAddress = &hashTable[index];
    
    if ( entryAddress->key != NULL ){                       //Here it searches for a available spot in the table
        if ( strcmp(entryAddress->name,c) == 0 ){
            return; //repeated entry
        }
        navAddress = entryAddress;
        entryAddress = entryAddress->next;
        while ( entryAddress != NULL ){
            if ( strcmp(entryAddress->name,c) == 0 ){
                return; //repeated entry
            }
            navAddress = entryAddress;
            entryAddress = entryAddress->next;
        }
        entryAddress = malloc( sizeof (struct entry) );     //entryAdress represents where the token will be inserted
        navAddress->next = entryAddress;                    //Adding one more chain to the linked list
    }
    
    if ( strcmp(type,"symbol") == 0 ){
        entryAddress->key = malloc( 16*sizeof(char) );
        entryAddress->sclass = malloc( 16*sizeof(char) );
        entryAddress->name = malloc( 16*sizeof(char) );
        entryAddress->value = malloc( 16*sizeof(char) );
        sprintf(entryAddress->key, "%d", index);
        sprintf(entryAddress->sclass, "%s", "Symbol");
        sprintf(entryAddress->name, "%s", c);
        sprintf(entryAddress->value, "%s", "");
        sprintf(token, "%s", c);
        return;
    }
    if ( strcmp(type,"number") == 0 ){
        entryAddress->key = malloc( 16*sizeof(char) );
        entryAddress->sclass = malloc( 16*sizeof(char) );
        entryAddress->name = malloc( 19*sizeof(char) );
        entryAddress->value = malloc( 19*sizeof(char) );
        sprintf(entryAddress->key, "%d", index);
        sprintf(entryAddress->sclass, "%s", "Number");
        sprintf(entryAddress->name, "%s", c);
        sprintf(entryAddress->value, "%s", c);
        sprintf(token, "%s", c);
        return;
    }
    if ( strcmp(type,"keyword") == 0 ){
        entryAddress->key = malloc( 16*sizeof(char) );
        entryAddress->sclass = malloc( 16*sizeof(char) );
        entryAddress->name = malloc( 32*sizeof(char) );
        entryAddress->value = malloc( 16*sizeof(char) );
        sprintf(entryAddress->key, "%d", index);
        sprintf(entryAddress->sclass, "%s", "Keyword");
        sprintf(entryAddress->name, "%s", c);
        sprintf(entryAddress->value, "%s", "");
        sprintf(token, "%s", c);
        return;
    }
    if ( strcmp(type,"type") == 0 ){
        entryAddress->key = malloc( 16*sizeof(char) );
        entryAddress->sclass = malloc( 16*sizeof(char) );
        entryAddress->name = malloc( 16*sizeof(char) );
        entryAddress->value = malloc( 16*sizeof(char) );
        sprintf(entryAddress->key, "%d", index);
        sprintf(entryAddress->sclass, "%s", "Type");
        sprintf(entryAddress->name, "%s", c);
        sprintf(entryAddress->value, "%s", "");
        sprintf(token, "%s", c);
        return;
    }
    if ( strcmp(type,"variable") == 0 ){
        entryAddress->key = malloc( 16*sizeof(char) );
        entryAddress->sclass = malloc( 16*sizeof(char) );
        entryAddress->name = malloc( 32*sizeof(char) );
        entryAddress->value = malloc( 16*sizeof(char) );
        sprintf(entryAddress->key, "%d", index);
        sprintf(entryAddress->sclass, "%s", "Variable");
        sprintf(entryAddress->name, "%s", c);
        sprintf(entryAddress->value, "%s", "");
        sprintf(token, "%s", c);
        return;
    }
    if ( strcmp(type,"grammar") == 0 ){
        entryAddress->key = malloc( 16*sizeof(char) );
        entryAddress->sclass = malloc( 16*sizeof(char) );
        entryAddress->name = malloc( 32*sizeof(char) );
        entryAddress->value = malloc( 16*sizeof(char) );
        sprintf(entryAddress->key, "%d", index);
        sprintf(entryAddress->sclass, "%s", "Grammar");
        sprintf(entryAddress->name, "%s", c);
        sprintf(entryAddress->value, "%s", "");
        sprintf(token, "%s", c);
        return;
    }
}

/*
void printTable(int format){
    int temp = 0;
    struct entry * entryNav;                //Hashtable navigation pointer
    
    switch (format){
        case 1:
            for ( temp=0; temp < TABLE_SIZE; temp++){
                if ( hashTable[temp].key != NULL ){
                    fprintf( fpTable ,"Key: %s | Name: %s | Class: %s | Value: %s \n", hashTable[temp].key, hashTable[temp].name, hashTable[temp].sclass, hashTable[temp].value);
                }
                if ( hashTable[temp].next != NULL){
                    entryNav = hashTable[temp].next;
                    while( entryNav != NULL ){
                        fprintf( fpTable ,"Key: %s | Name: %s | Class: %s | Value: %s \n", entryNav->key, entryNav->name, entryNav->sclass, entryNav->value);
                        entryNav = entryNav->next;
                    }
                }
            }
        break;
        case 2:
            fprintf( fpTable ,"Key,Name,Class,Value\n");
            for ( temp=0; temp < TABLE_SIZE; temp++){
                if ( hashTable[temp].key != NULL ){
                    fprintf( fpTable ,"%s,%s,%s,%s\n", hashTable[temp].key, hashTable[temp].name, hashTable[temp].sclass, hashTable[temp].value);
                }
                if ( hashTable[temp].next != NULL){
                    entryNav = hashTable[temp].next;
                    while( entryNav != NULL ){
                        fprintf( fpTable ,"%s,%s,%s,%s\n", entryNav->key, (char *)entryNav->name, entryNav->sclass, entryNav->value);
                        entryNav = entryNav->next;
                    }
                }
            }
        break;
    }
}
*/
void freeTable(){
    struct entry * entryNav;            //Hashtable navigation pointer
    struct entry * entryNavPrev;        //Hashtable delayed navigation pointer
    int temp = 0;
    for ( temp=0; temp < TABLE_SIZE; temp++){
        if ( hashTable[temp].key != NULL ){
            free(hashTable[temp].key);
            free(hashTable[temp].name);
            free(hashTable[temp].sclass);
            free(hashTable[temp].value);
        }
        if ( hashTable[temp].next != NULL){
            entryNav = hashTable[temp].next;
                while( entryNav != NULL ){
                    char * freeString = (char *) entryNav->key;
                    free(entryNav->key);
                    free(entryNav->name);
                    free(entryNav->sclass);
                    free(entryNav->value);
                    entryNavPrev = entryNav;
                    entryNav = entryNav->next;
                    free(entryNavPrev);
                }
        }
    }
}