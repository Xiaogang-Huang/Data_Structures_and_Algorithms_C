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

//��ն�����
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

//���������Ƿ���ڹؼ�����X�Ľڵ�
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

//����������С�ؼ������ڵĽڵ�
Position FindMin(SearchTree T)
{
	if (T == NULL)
		return NULL;
	else if (T->Left ==NULL)
		return T;
	else
		return FindMin(T->Left);
}

//�����������ؼ������ڵĽڵ�
Position FindMax(SearchTree T)
{
	if (T != NULL)
		while (T->Right != NULL)
			T = T->Right;
	return T;

}

/*���ؼ���X��������,Insert�����������ĸ�
T�ǿ���ʱ,�����������ǹؼ���X��һ��NULL,��������һ�����ڵ����,T��NULL��Ϊһ����ַ,������Ҫ�з���ֵ*/
SearchTree Insert(ElementType X, SearchTree T)
{
	/*T��һ�ÿ���*/
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

//ɾ���ؼ�����X�Ľڵ�
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

//���ؽڵ�P�Ĺؼ���
ElementType Retrieve(Position P)
{
	return P->Element;
}

//���������ӡ��
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
		printf("��СԪ��%d\n", x);
		T = Delete(x, T);
		printf("ɾ����СԪ�����������\n");
		PrintTree(T);
		printf("\n");
	}
	return 0;
}