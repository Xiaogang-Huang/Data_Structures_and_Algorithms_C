typedef int ElementType;

#ifndef _AVLTree_H
#define _AVLTree_H

struct AVLNode;
typedef struct AVLNode* Position;
typedef struct AVLNode* AVLTree;

AVLTree MakeEmpty(AVLTree T);
Position Find(ElementType X, AVLTree T);
Position FindMin(AVLTree T);
Position FindMax(AVLTree T);
AVLTree Insert(ElementType X, AVLTree T);
AVLTree Delete(ElementType X, AVLTree T);
ElementType Retrieve(Position P);
void PrintTree(AVLTree T);
void PrintAVLTree(AVLTree T, int m);

#endif // !_AVLTree_H



