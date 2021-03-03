#include <stdio.h>
#include <stdlib.h>
#include "bt.h"

#define Error( Str )        FatalError( Str )
#define FatalError( Str )   fprintf( stderr, "%s\n", Str ), exit( 1 )

struct TreeNode {
	ElementType Element;
	SearchTree Left;
	SearchTree Right;
};

//清空二叉树
SearchTree MakeEmpty(SearchTree T)
{
	if (T != NULL)
	{
		MakeEmpty(T->Left);
		MakeEmpty(T->Right);
		free(T);
	}
	return NULL;
}

//查找树中是否存在关键字是X的节点
Position Find(ElementType X, SearchTree T)
{
	if (T == NULL)
		return NULL;
	if (X < T->Element)
		return Find(X, T->Left);
	else if (X > T->Element)
		return Find(X, T->Right);
	else
		return T;
}

//查找树中最小关键字所在的节点
Position FindMin(SearchTree T)
{
	if (T == NULL)
		return NULL;
	else if (T->Left ==NULL)
		return T;
	else
		return FindMin(T->Left);
}

//查找树中最大关键字所在的节点
Position FindMax(SearchTree T)
{
	if (T != NULL)
		while (T->Right != NULL)
			T = T->Right;
	return T;

}

/*将关键字X插入树中,Insert函数返回树的根
T是空树时,函数的输入是关键字X和一个NULL,函数生成一个单节点的树,T从NULL变为一个地址,所以需要有返回值*/
SearchTree Insert(ElementType X, SearchTree T)
{
	/*T是一棵空树*/
	if (T == NULL)
	{
		T = malloc(sizeof(struct TreeNode));
		if (T == NULL)
			FatalError("Out of space!!!");
		else
		{
			T->Element = X;
			T->Left = T->Right = NULL;
		}
	}
	else if (X < T->Element)
	{
		T->Left = Insert(X, T->Left);
	}
	else if(X > T->Element)
	{
		T->Right = Insert(X, T->Right);
	}
	return T;
}

//删除关键字是X的节点
SearchTree Delete(ElementType X, SearchTree T)
{
	Position TmpCell;

	if (T == NULL)
		Error("Element not found");
	else
		if (X < T->Element)  /* Go left */
			T->Left = Delete(X, T->Left);
		else
			if (X > T->Element)  /* Go right */
				T->Right = Delete(X, T->Right);
			else  /* Found element to be deleted */
				if (T->Left && T->Right)  /* Two children */
				{
					/* Replace with smallest in right subtree */
					TmpCell = FindMin(T->Right);
					T->Element = TmpCell->Element;
					T->Right = Delete(T->Element, T->Right);
				}
				else  /* One or zero children */
				{
					TmpCell = T;
					if (T->Left == NULL) /* Also handles 0 children */
						T = T->Right;
					else if (T->Right == NULL)
						T = T->Left;
					free(TmpCell);
				}

	return T;
}

//返回节点P的关键字
ElementType Retrieve(Position P)
{
	return P->Element;
}

//中序遍历打印树
void PrintTree(SearchTree T)
{
	if (T != NULL)
	{
		PrintTree(T->Left);
		printf("%d, ", T->Element);
		PrintTree(T->Right);
	}
}

int main()
{
	ElementType A[] = { 6, 2, 1, 5, 8, 19, -5, 3, 0, 2, 10 };
	ElementType x;
	SearchTree T = NULL;
	int length = sizeof(A) / sizeof(ElementType);
	
	for (int i = 0; i < length; i++)
	{
		T = Insert(A[i], T);
	}
	PrintTree(T);
	printf("\n");
	while (T != NULL)
	{
		x = Retrieve(FindMin(T));
		printf("最小元：%d\n", x);
		T = Delete(x, T);
		printf("删除最小元后中序遍历：\n");
		PrintTree(T);
		printf("\n");
	}
	return 0;
}