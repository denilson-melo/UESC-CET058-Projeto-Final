#ifndef MAIN_H
#define MAIN_H

#define TREE_SIZE 100000000
#define treeLevelSize 12
//Estrutura da tabela de símbolos
struct entry{                               
    char * key;
    char * sclass;
    char * name;
    char * value;
    struct entry * next;                    //Lista ligada para tratamento de colisões
};
extern struct entry* aTree[];
struct entry * lookUp(char *c);

#endif