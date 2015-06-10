#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "main.h"

struct treeNode * yggdrasil;

void treeTest (){
    yggdrasil = startNode(yggdrasil,lookUp("main"));
    //yggdrasil->info = lookUp("main");
    //yggdrasil->children = malloc( sizeof(struct treeNode) );
    //yggdrasil->children->info = lookUp("return");
    //printf("%s %s\n", yggdrasil->info->name, yggdrasil->children->info->name);
    yggdrasil->info = lookUp("main");
    addChild(yggdrasil, lookUp("("));
    addChild(yggdrasil, lookUp("("));
    addChild(yggdrasil, lookUp("("));
    addChild(yggdrasil->children, lookUp(")"));
    addChild(yggdrasil->children, lookUp(")"));
    addChild(yggdrasil->children, lookUp(")"));
    addChild(yggdrasil->children->children, lookUp("}"));
    printf("URUBU DA YGGDRASIL\n");
    //printf("%s\n", yggdrasil->info->name);
    //printf("%s\n", yggdrasil->children->info->name);
    //printf("%s\n", yggdrasil->children->next->info->name);
    //printf("%s\n", yggdrasil->children->next->next->info->name);
    preOrder(yggdrasil);
}

struct treeNode * addChild (struct treeNode * node, struct entry * data){
    //Navigation variables
    struct treeNode * nav;
    struct treeNode * navPrev;
    
    //If adding the first child, just create it
    if ( node->children == NULL ){
        node->children = startNode(node->children,data);
    }
    //If there is already one child, it goes into the linked list, to get the
    //last position (the number of children is variable)
    else{
        nav = node->children;
        while ( nav->next != NULL ){
            nav = nav->next;
        }
        nav->next = startNode(nav->next,data);
        nav = nav->next;
    }
    //Set the father
    nav->father = node;
    return nav;
}

void preOrder(struct treeNode * node){
    struct treeNode * nav;
    if ( node == NULL ){
        return;
    }
    printf("%s\n", node->info->name);
    nav = node->children;
    while ( nav != NULL ){
        preOrder(nav);
        nav = nav->next;
    }
}

struct treeNode * startNode(struct treeNode * node, struct entry * data){
    node = malloc(sizeof(struct treeNode));
    node->info = data;
    node->next = NULL;
    node->children = NULL;
    node->father = NULL;
    return node;
}