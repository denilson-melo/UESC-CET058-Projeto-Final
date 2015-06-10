#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "rpn.h"
#include "tree.h"
#define TABLE_SIZE 1024
#define TOKEN_SIZE 32
#define isSymbol c == '{' || c == '}' || c == '(' || c == ')' || c == '[' || c == ']' || c == '|' || c == ',' || c == '=' || c == '&' || c == ';' || c == '+' || c == '*' || c == '-' || c == '/'
#define isSpacing c == ' ' || c == '\t' || c == '\n'
#define isLetter c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z'
#define isDigit c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' 

//As varíaveis e os escopos das funções abaixo estão separados por funcionalidade

//Scanner
void next();
int symEquals(char * text);
char c;
char symC;

//Tratamento de erros
int row = 1;
void peekFound();
char expecting[32];
char found[32];
void error();

//Símbolos da gramática
void S();
void DECL();
void DECLC();
void DECLI();
void VAR();
void VAR2(int); //Variante do VAR para ser utilizado por declarações, não adciona à arvore de análise abstrata
void STMT();
void SATRIB();
void SIF();
void SWHILE();
void SFOR();
void E();
void B();
void NUM();
void DIGIT();
int firstDECL();
int firstDECLC();
int firstDECLI();
int firstVAR();
int firstSTMT();
int firstSATRIB();
int firstSIF();
int firstSWHILE();
int firstSFOR();
int firstE();
int firstB();
int firstNUM();
int firstDIGIT();

//Tabela de símbolos
char token[TOKEN_SIZE]; 
struct entry hashTable[TABLE_SIZE];
int hash (char *c);                         
void insertEntry (char *c,  char *type);   

void printTable(int format);
void freeTable();

//Variáveis e métodos para ambas as árvores
void printTree(int, struct entry ** t);

//Arvore de análise sintatica
long long unsigned ti = 0;
struct  entry* tree[TREE_SIZE];

//Arvore de análise sintatica abstrata
long long unsigned at = 0;
struct  entry* aTree[TREE_SIZE];

//Arvore teste
struct treeNode * ygg;
struct treeNode * nav;

//Pilha auxiliar para arvore de análise sintática
int si = 0;
int stack[1024];
void push(int);
int pop();

//Pilha auxiliar para arvore de sintaxe abstrata
int siA = 0;
int stackA[1024];
void pushA(int);
int popA();
char externalTreeString[1024];
void externalTreePreorder(int);
//Abertura do arquivo
FILE *fp;
FILE *fpTable;
char *fileName;
char *tableFileName;
char *treeType;
void formatError();

void main( int argc , char ** argv ){
    int format;
    if ( argc != 3 && argc != 4 ){
        formatError();
        return;
    }
    if ( argc == 3 ){
        printf("Formato nao especificado, 0 utilizando impressao compacta como padrao\n");
        format = 0;
    }else{
        format = atoi(argv[3]);
    }
    
    fileName = argv[1];
    treeType = argv[2];
    
    if ( strcmp(treeType,"PT") != 0 && strcmp(treeType,"AST") ){
        printf("Tipo de árvore incorreto\n");
        formatError();
        return;
    }
    
    fp = fopen(fileName,"r");
    if ( fp == NULL ){
        printf("Erro ao abrir o arquivo de amostra\n");
        return;
    }
     
    
    insertEntry("S","grammar");
    insertEntry("DECL","grammar");
    insertEntry("DECLC","grammar");
    insertEntry("DECLI","grammar");
    insertEntry("STMT","grammar");
    insertEntry("SATRIB","grammar");
    insertEntry("SFOR","grammar");
    insertEntry("SWHILE","grammar");
    insertEntry("SIF","grammar");
    insertEntry("VAR","grammar");
    insertEntry("E","grammar");
    insertEntry("B","grammar");
    insertEntry("NUM","grammar");
    insertEntry("DIGIT","grammar");
    next();
    S();    
    printf("Analise sintatica concluida\n\n");
    fclose(fp);
    if ( strcmp(treeType,"PT")==0 ){
        printf("Parsing Tree (arvore de analise):\n");
        printTree(format, tree);
    }else{
        printf("Abstract Syntax Tree(arvore de sintaxe abstrata):\n");
        strcpy(externalTreeString,"");
        externalTreePreorder(0);
        printTree(1, aTree);
        printf("Formato para desenho: %s\n",externalTreeString);
        printf("Colar impressão acima no site http://ironcreek.net/phpsyntaxtree\n");
    }
    //generateRpn(0);
    //printRpn();
    //treeTest();
    printf("URUBU DA YGGDRASIL\n");
    preOrder(ygg);
    freeTable();
    //free(tree);
}

void S(){
    if(symEquals("int")){
        insertEntry("int","type");
        tree[ti++] = lookUp("int");
        next();
        if(symEquals("main")){
            insertEntry("main","keyword");
            aTree[at++] = tree[ti++] = lookUp("main"); ygg = startNode(ygg, lookUp("main"));
            nav = ygg;
            next();
            if(symEquals("(")){				
                insertEntry("(","symbol");				
                tree[ti++] = lookUp("(");
                next();
                if(symEquals(")")){
                    insertEntry(")","symbol");
                    tree[ti++] = lookUp(")");
                    next();
                    if (symEquals("{")){
                        insertEntry("{","symbol");
                        tree[ti++] = lookUp("{");
                            next();
                        tree[ti++] = lookUp("DECL");
                        push(ti); pushA(at);
                            DECL();
                        ti = pop(); at = popA();						
                        tree[ti++] = lookUp("STMT");
                        push(ti);
                            STMT();
                        ti = pop();
                        if ( symEquals("return") ){
                            insertEntry("return","keyword");
                            at++;
                            aTree[at] = tree[ti++] = lookUp("return"); nav = addChild(nav, lookUp("return"));
                                next();
                            if ( symEquals("(") ){
                                insertEntry("(","symbol");
                                tree[ti++] = lookUp("(");
                                    next();
                                tree[ti++] = lookUp("E");
                                push(ti);		
                                at*=treeLevelSize; nav = addChild(nav,NULL);
                                    E();
                                ti = pop(); nav = nav->father;
                                if ( symEquals(")") ){
                                    insertEntry(")","symbol");									
                                    tree[ti++] = lookUp(")");
                                        next();
                                    if ( symEquals("}") ){
                                        insertEntry("}","symbol");
                                        tree[ti++] = lookUp("}");
                                        return;
                                    }
                                }
                            } 
                            
                        }   
                   }
                } 
            }
        }
    }
    error();
}

//DECL
void DECL(){
    ti--;
    ti *= treeLevelSize;	
    if ( firstDECL() ){
        if ( firstDECLC() ){
            tree[ti++] = lookUp("DECLC");
            push(ti);
                DECLC();
            ti = pop();
            return;
        }else if ( firstDECLI() ){
            tree[ti++] = lookUp("DECLI");
            push(ti);
                DECLI();
            ti = pop();
            return;
        }
    }
    error();
}
int firstDECL(){
    if ( firstDECLC() || firstDECLI() ){
        return 1;
    }
    strcpy(expecting,"char or int");
    return 0;
}

//DECLC
void DECLC(){
    ti--;
    ti *= treeLevelSize;
    if ( symEquals("char") ){
        insertEntry("char","type");
        tree[ti++] = lookUp("char");
            next();
        tree[ti++] = lookUp("VAR");
        push(ti);
            VAR2(0);
        ti = pop();
        while( symEquals(",") ){
            insertEntry(",","symbol");
            tree[ti++] = lookUp(",");
                next();
            tree[ti++] = lookUp("VAR");
            push(ti);
                VAR2(0);
            ti = pop();
        }
        if ( symEquals(";") ){
            insertEntry(";","symbol");
            tree[ti++] = lookUp(";");
                next();
            if ( firstDECLI() ){
                tree[ti++] = lookUp("DECLI");
                push(ti);
                    DECLI();
                ti = pop();
            }
        }
        return;
    }
    error();
    return;
}
int firstDECLC(){
    if ( symC == 'c' ){
        return 1;
    }
    return 0;
}

//DECLI
void DECLI(){
    ti--;
    ti*=treeLevelSize;
    
    if ( symEquals("int") ){
        insertEntry("int","type");
        tree[ti++] = lookUp("int");
            next();
        tree[ti++] = lookUp("VAR");
        push(ti);
            VAR2(0);
        ti = pop();
        int loop = 1;
        while ( loop ){
            if( symEquals(",") ){
                insertEntry(",","symbol");
                tree[ti++] = lookUp(",");
                    next();
                tree[ti++] = lookUp("VAR");
                push(ti);
                    VAR2(0);
                ti = pop();
                continue;
            }
            loop = 0;
        }
        if( symEquals(";") ){
            insertEntry(";","symbol");
            tree[ti++] = lookUp(";");
                next();
            if ( firstDECLC() ){
                tree[ti++] = lookUp("DECLC");
                push(ti);
                    DECLC();
                ti = pop();
            }
        }
        return;
    }
    error();
    return;
}
int firstDECLI(){
    if ( symC == 'i' ){
        return 1;
    }
    return 0;
}

//VAR
void VAR(){
    ti--;
    ti*=treeLevelSize;
    char tempToken[32];
    int i = 0;
    if ( firstVAR() ){
        if ( symEquals("x") || symEquals("y") || symEquals("z") ){
            tempToken[i++] = symC;
            next();
            while( symEquals("x") || symEquals("y") || symEquals("z") ){
                tempToken[i++] = symC;
                next();
            }
            tempToken[i] = '\0';			
            insertEntry(tempToken, "variable");
            aTree[at] = tree[ti++] = lookUp(tempToken); setNode(nav, lookUp(tempToken));
            ungetc(c,fp);
            next();
            return;
        }
    }
    error();
}
void VAR2(int alt){
    ti--;
    ti*=treeLevelSize;
    char tempToken[32];
    int i = 0;
    if ( firstVAR() ){
        if ( symEquals("x") || symEquals("y") || symEquals("z") ){
            tempToken[i++] = symC;
            next();
            while( symEquals("x") || symEquals("y") || symEquals("z") ){
                tempToken[i++] = symC;
                next();
            }
            tempToken[i] = '\0';			
            insertEntry(tempToken, "variable");
            tree[ti++] = lookUp(tempToken);
            ungetc(c,fp);
            next();
            return;
        }
    }
    error();
}
int firstVAR(){
    if( symC == 'x' || symC == 'y' || symC == 'z' ){
        return 1;
    }
    return 0;
}

//STMT
void STMT(){
    ti--;
    ti *= treeLevelSize;    
    if ( firstSTMT() ){
        if( firstSATRIB() ){
            tree[ti++] = lookUp("SATRIB");
            push(ti);
                SATRIB();
            ti = pop();
        }else if( firstSIF() ){
            tree[ti++] = lookUp("SIF");
            push(ti);
                SIF();
            ti = pop();
        }else if( firstSWHILE() ){
            tree[ti++] = lookUp("SWHILE");
            push(ti);
                SWHILE();
            ti = pop();
        }else if( firstSFOR() ){
            tree[ti++] = lookUp("SFOR");
            push(ti);
                SFOR();
            ti = pop();
        }
    }
    if ( firstSTMT() ){
        error();
        return;
    }
    while ( symEquals(";") ){
        insertEntry(";","symbol");
        tree[ti++] = lookUp(";");
            next();
        at++; pushA(at);
        if ( firstSTMT() ){
            if( firstSATRIB() ){
                tree[ti++] = lookUp("SATRIB");
                push(ti);
                    SATRIB();
                ti = pop();
            }else if( firstSIF() ){
                tree[ti++] = lookUp("SIF");
                push(ti);
                    SIF();
                ti = pop();
            }else if( firstSWHILE() ){
                tree[ti++] = lookUp("SWHILE");
                push(ti);
                    SWHILE();
                ti = pop();
            }else if( firstSFOR() ){
                tree[ti++] = lookUp("SFOR");
                push(ti);
                    SFOR();
                ti = pop();
            }
        }
        at = popA(at);
    }
}
int firstSTMT(){
    if ( firstSATRIB() || firstSIF() || firstSWHILE() || firstSFOR() ){
        return 1;
    }
    strcpy(expecting,"'if' or 'for' or 'while'");
    return 0;
}

//SATRIB
void SATRIB(){
    ti--;
    ti*=treeLevelSize;
    tree[ti++] = lookUp("VAR");
    nav = addChild(nav,NULL);
    push(ti); pushA(at);
    at *= treeLevelSize; nav = addChild(nav, NULL);
        VAR(); //nav->startNode(nav,)  lookUp("VAR");
    ti = pop(); at = popA(); nav = nav->father;
    if ( symEquals("=") ){
        insertEntry("=","symbol");
        tree[ti++] = lookUp("=");
        aTree[at] = lookUp("="); setNode(nav, lookUp("="));
            next();
        tree[ti++] = lookUp("E");
        push(ti); pushA(at);
        at*=treeLevelSize; at++; nav = addChild(nav, NULL);
            E();
        ti = pop(); at = popA();nav = nav->father;
        if ( symEquals(";") ){
            insertEntry(";","symbol");
            tree[ti++] = lookUp(";");
            next();
            nav = nav->father;
            return;
        }
    }
    error();
}
int firstSATRIB(){
    if ( firstVAR() ){
        return 1;
    }
    return 0;
}

//SIF
void SIF(){
    ti--;
    ti*=treeLevelSize;
    if ( symEquals("if") ){
        insertEntry("if","keyword");
        aTree[at] = tree[ti++] = lookUp("if");
        next();
        if ( symEquals("(") ){
            insertEntry("(","symbol");
            tree[ti++] = lookUp("(");
                next();
            tree[ti++] = lookUp("E");
            push(ti); pushA(at);
            at *= treeLevelSize;
                E();
            ti = pop(); at = popA();
            if ( symEquals(")") ){
                insertEntry(")","symbol");
                tree[ti++] = lookUp(")");
                    next();
                if ( symEquals("{") ){
                    insertEntry("{","symbol");
                    tree[ti++] = lookUp("{");
                        next();
                    if ( firstSTMT() ){
                        tree[ti++] = lookUp("STMT");
                        push(ti); pushA(at);
                        at *= treeLevelSize; at++;
                            STMT();
                        ti = pop(); at = popA();
                    }
                    if ( symEquals("}") ){
                        insertEntry("}","symbol");
                        tree[ti++] = lookUp("}");
                            next();
                        return;
                    }
                }
            }
        }
    }
    error();
    return;
}
int firstSIF(){
    if ( symC == 'i' ){
        return 1;
    }
    return 0;
}

//SWHILE
void SWHILE(){
    ti--;
    ti *= treeLevelSize;
    if ( symEquals("while") ){
        insertEntry("while","keyword");
        aTree[at] = tree[ti++] = lookUp("while");
            next();
        if ( symEquals("(") ){
            insertEntry("(","symbol");
            tree[ti++] = lookUp("(");
                next();
            tree[ti++] = lookUp("E");
            push(ti); pushA(at);
            at*=treeLevelSize;
                E();
            ti = pop(); at = popA();
            if ( symEquals(")") ){
                insertEntry(")","symbol");
                tree[ti++] = lookUp(")");
                    next();
                if ( symEquals("{") ){
                    insertEntry("{","symbol");
                    tree[ti++] = lookUp("{");
                        next();
                    tree[ti++] = lookUp("STMT");
                    push(ti); pushA(at);
                    at *= treeLevelSize; at++;
                        STMT();
                    ti = pop(); at = popA();
                    if ( symEquals("}") ){
                        insertEntry("}","symbol");
                        tree[ti++] = lookUp("}");
                        next();
                        return;
                    }
                }
            }
        }
    }
}
int firstSWHILE(){
    if ( symC == 'w' ){
        return 1;
    }
    return 0;
}

//SFOR
void SFOR(){
    ti--;
    ti*=treeLevelSize;
    if ( symEquals("for") ){
        insertEntry("for","keyword");
        aTree[at] = tree[ti++] = lookUp("for");
            next();
        if ( symEquals("(") ){
            insertEntry("(","symbol");
            tree[ti++] = lookUp("(");
                next();
            tree[ti++] = lookUp("E");
            push(ti); pushA(at);
            at *= treeLevelSize;
                E();
            ti = pop(); at = popA();
            if ( symEquals(";") ){
                insertEntry(";","symbol");
                tree[ti++] = lookUp(";");
                    next();
                tree[ti++] = lookUp("E"); 
                push(ti); pushA(at);
                at *= treeLevelSize; at++;
                    E();
                ti = pop(); at = popA();
                if ( symEquals(";") ){
                    insertEntry(";","symbol");
                    tree[ti++] = lookUp(";");
                        next();
                    tree[ti++] = lookUp("E");
                    push(ti); pushA(at);
                    at *= treeLevelSize; at += 2;
                        E();
                    ti = pop(); at = popA();
                    if ( symEquals(")") ){
                        insertEntry(")","symbol");
                        tree[ti++] = lookUp(")");
                            next();
                        if ( symEquals("{") ){
                            insertEntry("{","symbol");
                            tree[ti++] = lookUp("{");
                                next();
                            tree[ti++] = lookUp("STMT");
                            push(ti); pushA(at);
                            at *= treeLevelSize; at += 3;
                                STMT();
                            ti = pop(); at = popA();
                            if ( symEquals("}") ){
                                insertEntry("}","symbol");
                                tree[ti++] = lookUp("}");
                                    next();
                                return;
                            }
                        }
                    }
                }
            }
        }
    }
}
int firstSFOR(){
    if ( symC == 'f' ){
        return 1;
    }
    return 0;
}

//E
void E(){
    if (firstE()){
        ti--;
        ti *= treeLevelSize;
        if ( firstNUM() ){
            tree[ti++] = lookUp("NUM");
            push(ti);
                NUM();
            ti = pop();
            return;
        }else if ( firstVAR() ){
            tree[ti++] = lookUp("VAR");
            push(ti);
                VAR();
            ti = pop();
            return;
        }else if ( symEquals("(") ){
            insertEntry("(","symbol");
            tree[ti++] = lookUp("(");
                next();
            tree[ti++] = lookUp("E");
            push(ti); pushA(at); nav = addChild(nav, NULL);
            at*=treeLevelSize;
                E();
            ti = pop(); at = popA(); nav = nav->father;
            tree[ti++] = lookUp("B");
            push(ti);
                B();
            ti = pop();
            tree[ti++] = lookUp("E");
            push(ti); pushA(at); nav = addChild(nav, NULL);
            at*=treeLevelSize; at++;
                E();
            ti = pop(); popA(); nav = nav->father;
            if ( symEquals(")") ){
                insertEntry(")","symbol");
                tree[ti++] = lookUp(")");
                    next();
                return;
            }
        }
    }
    error();
    return;
}
int firstE(){
    if ( firstNUM() || firstVAR() || symC == '(' ){
        return 1;
    }
    strcpy(expecting,"expression");
    return 0;
}

//B
void B(){
    ti--;
    ti*=treeLevelSize;
    if ( symEquals("+") ){
        insertEntry("+","symbol");
        aTree[at] = tree[ti++] = lookUp("+"); setNode(nav, lookUp("+"));
        next();
        return;
    }else if ( symEquals("-") ){
        insertEntry("-","symbol");
        aTree[at] = tree[ti++] = lookUp("-"); setNode(nav, lookUp("-"));
        next();
        return;
    }else if ( symEquals("*") ){
        insertEntry("*","symbol");
        aTree[at] = tree[ti++] = lookUp("*"); setNode(nav, lookUp("*"));
        next();
        return;
    }else if ( symEquals("/") ){
        insertEntry("/","symbol");
        aTree[at] = tree[ti++] = lookUp("/"); setNode(nav, lookUp("/"));
        next();
        return;
    } 
    error();
    return;
}
int firstB(){
    if ( symC == '+' || symC == '-' || symC == '*' || symC == '/'  ){
        return 1;
    }
    return 0;
}

//NUM
void NUM(){
    ti--;
    ti*= treeLevelSize;
    char tempToken[32];
    int i = 0;
    if ( firstDIGIT() ){
        tempToken[i++] = symC;
        DIGIT();
            while( firstDIGIT() ){
                tempToken[i++] = symC;
                DIGIT();
            }
        tempToken[i] = '\0';
        insertEntry(tempToken,"number");
        aTree[at] = tree[ti++] = lookUp(tempToken); setNode(nav, lookUp(tempToken));
        return;
    }
    error();
    return;
}
int firstNUM(){
    c = symC;
    if( isDigit ){
        return 1;
    }
    return 0;
}

//DIGIT
void DIGIT(){
    if ( symEquals("0") || symEquals("1") || symEquals("2") || symEquals("3") || symEquals("4") || symEquals("5") || symEquals("6") || symEquals("7") || symEquals("8") || symEquals("9") ){
        next();
        return;
    }
    error();
    return;
}
int firstDIGIT(){
    if ( symEquals("0") || symEquals("1") || symEquals("2") || symEquals("3") || symEquals("4") || symEquals("5") || symEquals("6") || symEquals("7") || symEquals("8") || symEquals("9") ){
        return 1;
    }
    return 0;
}

//Scanner
void next(){
    int i = 0;
    c = fgetc(fp);
    while ( isSpacing ){
        if ( c =='\n' ){
            row++;
        }
        c = fgetc(fp);
    }
    if ( isSymbol ){
        symC = c;
        return;
    }
    if ( isLetter || isDigit || c == '\"' ){
        symC = c;
    }
    return;
}
int symEquals(char * text){
    strcpy (expecting, text);
    int i = 0;
    while ( text[i] != '\0' ){
        if ( symC == text[i] ){
            i++;
            symC = c = fgetc(fp);
        }else{
            if ( strcmp(expecting,"return") == 0 ){
                strcpy(expecting,"correct statement or 'return'");
            }
            return 0;
        }
    }
    ungetc(c,fp);
    symC = text[i-1];
    return 1;
}

//Tratamento de erro
void peekFound(){
    char tempToken[32];
    int i = 0;
    while ( isSpacing ){
        c = fgetc(fp);
    }
    while ( isLetter || isDigit || isSymbol ){
        tempToken[i++] = c;
        c = fgetc(fp);
    }
    tempToken[i] = '\0';
    strcpy(found, tempToken);
}
void error(){
    peekFound();
    printf("Erro de sintaxe\n");
    printf("Linha: %d\n", row);
    printf("Esperando: %s\n", expecting);
    printf("Encontrado: %s\n", found);
    
    exit(0);
}
void formatError(){
    printf("Formato incorreto, Utilizar: ./lp0 (amostra) (tipo de arvore a ser impresso) (formato)*\n\n");
    printf("Tipos de arvore: AST (Abstract Syntax Tree) \n");
    printf("               : PT (Parsing tree) \n\n");
    printf("O formato eh opcional: 0 impressao compacta\n");
    printf("                     : 1 impressao de cada indice da arvore\n\n");
    printf("Exemplo de execucao: ./lp0 t1 PT 0 \n");
}

//Tabela de símbolos
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

//Arvore de análise
void push(int i){
    if ( si == 1024 ){
        printf("Stack Overflow\n");
        exit(0);
    }
    stack[si] = i;
    si++;
}
int pop(){
    if ( si == 0 ){
        printf("Stack Underflow\n");
        exit(0);
    }
    si--;
    return stack[si];
}

//Arvore de sintaxe abstrata
void pushA(int i){
    if ( siA == 1024 ){
        printf("Stack Overflow\n");
        exit(0);
    }
    stackA[siA] = i;
    siA++;
}
int popA(){
    if ( siA == 0 ){
        printf("Stack Underflow\n");
        exit(0);
    }
    siA--;
    return stackA[siA];
}

void printTree( int format, struct entry ** t ){
    int i;
    int k = treeLevelSize;
    if ( format == 0 ){
    for ( i=0 ; i<TREE_SIZE; i++){
        if ( t[i] !=NULL ){
            if ( i%treeLevelSize == 0 ){
                printf("\n%d: ", i);
            }
            printf("%s ", t[i]->name);
        } 
        if ( i == k ){
                printf("\n -");
                k *= treeLevelSize;
        }
    }
    }else{
        printf("\n");
        for ( i=0 ; i<TREE_SIZE; i++){
            if ( t[i] !=NULL ){
                printf("%d: %s\n", i, t[i]->name);
            } 
        }
    }
    printf("\n");
}
void externalTreePreorder(int k){
    int i;
    if ( k==0 ){
        i=1;
    }else{
        i=0;
    }
    if ( aTree[k] == NULL ){
        return;
    }
    char tempStr[32]; sprintf(tempStr,"[%s", aTree[k]->name);
    strcat(externalTreeString,tempStr);
    k *= treeLevelSize;
    for ( i; i<12; i++){
        externalTreePreorder((k+i));
    }
    strcat(externalTreeString,"]");
}

