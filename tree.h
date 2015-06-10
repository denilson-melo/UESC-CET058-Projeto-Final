#ifndef TREE_H
#define TREE_H

struct treeNode{
    struct entry * info;
    struct treeNode * next;
    struct treeNode * children;
    struct treeNode * father;
};

void treeTest();
void startTree(struct treeNode *);
struct treeNode * addChild(struct treeNode *, struct entry *);
struct treeNode * startNode(struct treeNode *, struct entry *);
void setNode(struct treeNode *, struct entry *);
void preOrder(struct treeNode *);

#endif