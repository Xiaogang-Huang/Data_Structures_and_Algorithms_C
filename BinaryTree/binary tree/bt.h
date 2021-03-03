typedef int ElementType;

#ifndef _bt_h
#define _bt_h

struct TreeNode;
typedef struct TreeNode* Position;
typedef Position SearchTree;

SearchTree MakeEmpty(SearchTree T);
Position Find(ElementType X, SearchTree T);
Position FindMin(SearchTree T);
Position FindMax(SearchTree T);
SearchTree Insert(ElementType X, SearchTree T);
SearchTree Delete(ElementType X, SearchTree T);
ElementType Retrieve(Position P);
void PrintTree(SearchTree T);

#endif // !_bt_h
