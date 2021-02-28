#include <stdio.h>
#include "AVLTree.h"
#include <stdlib.h>

#define Error( Str )        FatalError( Str )
#define FatalError( Str )   fprintf( stderr, "%s\n", Str ), exit( 1 )

struct AVLNode {
	ElementType Element;
	AVLTree Left;
	AVLTree Right;
	int Height;
};

//����ڵ�ĸ߶�
static int Height(Position P)
{
	if (P == NULL)
		return -1;
	return P->Height;
}

//����ת
static Position SingleRotationWithLeft(Position K2)
{
	Position K1;

	K1 = K2->Left;
	K2->Left = K1->Right;
	K1->Right = K2;

	K2->Height = max(Height(K2->Left), Height(K2->Right)) + 1;
	K1->Height = max(Height(K1->Left), K2->Height) + 1;

	return K1;
}

static Position SingleRotationWithRight(Position K1)
{
	Position K2;

	K2 = K1->Right;
	K1->Right = K2->Left;
	K2->Left = K1;

	K1->Height = max(Height(K1->Left), Height(K1->Right)) + 1;
	K2->Height = max(K1->Height, Height(K2->Right)) + 1;

	return K2;
}

//˫��ת
static Position DoubleRotationWithLeft(Position K3)
{
	K3->Left = SingleRotationWithRight(K3->Left);
	return SingleRotationWithLeft(K3);
}

static Position DoubleRotationWithRight(Position K1)
{
	K1->Right = SingleRotationWithLeft(K1->Right);
	return SingleRotationWithRight(K1);
}

//����
AVLTree Insert(ElementType X, AVLTree T)
{
	if (T == NULL)
	{
		T = malloc(sizeof(struct AVLNode));
		if (T == NULL)
			FatalError("Out of space!!!");
		else
		{
			T->Element = X;
			T->Height = 0;
			T->Left = T->Right = NULL;
		}
	}
	else if (X < T->Element)
	{
		T->Left = Insert(X, T->Left);
		if (Height(T->Left) - Height(T->Right) == 2)
			if (X < T->Left->Element)
				T = SingleRotationWithLeft(T);
			else
				T = DoubleRotationWithLeft(T);
	}
	else if (X > T->Element)
	{
		T->Right = Insert(X, T->Right);
		if (Height(T->Right) - Height(T->Left) == 2)
			if (X > T->Right->Element)
				T = SingleRotationWithRight(T);
			else
				T = DoubleRotationWithRight(T);
	}

	T->Height = max(Height(T->Left), Height(T->Right)) + 1;
	return T;
}

//�����
AVLTree MakeEmpty(AVLTree T)
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
Position Find(ElementType X, AVLTree T)
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
Position FindMin(AVLTree T)
{
	if (T == NULL)
		return NULL;
	else if (T->Left == NULL)
		return T;
	else
		return FindMin(T->Left);
}

//�����������ؼ������ڵĽڵ�
Position FindMax(AVLTree T)
{
	if (T != NULL)
		while (T->Right != NULL)
			T = T->Right;
	return T;

}

AVLTree Delete(ElementType X, AVLTree T)
{
	Position TmpCell, r, l;

	if (T == NULL)
		Error("Element not found");
	else
		if (X < T->Element)  /* Go left */
		{
			T->Left = Delete(X, T->Left);
			T->Height = max(Height(T->Right), Height(T->Left)) + 1;
			if (Height(T->Right) - Height(T->Left) == 2)
			{
				r = T->Right;
				if (Height(r->Left) > Height(r->Right))
					T = DoubleRotationWithRight(T);
				else
					T = SingleRotationWithRight(T);
			}
		}
		else
			if (X > T->Element)  /* Go right */
			{
				T->Right = Delete(X, T->Right);
				T->Height = max(Height(T->Right), Height(T->Left)) + 1;
				if (Height(T->Left) - Height(T->Right) == 2)
				{
					l = T->Left;
					if (Height(l->Right) > Height(l->Left))
						T = DoubleRotationWithLeft(T);
					else
						T = SingleRotationWithLeft(T);
				}
			}
			else  /* Found element to be deleted */
				if (T->Left && T->Right)  /* Two children */
				{
					/* Replace with smallest in right subtree */
					TmpCell = FindMin(T->Right);
					T->Element = TmpCell->Element;
					T->Right = Delete(T->Element, T->Right);
					T->Height = max(Height(T->Right), Height(T->Left)) + 1;
				}
				else  /* One or zero children */
				{
					TmpCell = T;
					if (T->Left == NULL) /* Also handles 0 children */
					{
						T = T->Right;
						if (T != NULL)
							T->Height = max(Height(T->Right), Height(T->Left)) + 1;
					}
					else if (T->Right == NULL)
					{
						T = T->Left;
						if (T != NULL)
							T->Height = max(Height(T->Right), Height(T->Left)) + 1;
					}
					free(TmpCell);
				}

	return T;
}

ElementType Retrieve(Position P)
{
	return P->Element;
}

//���������ӡ��
void PrintTree(AVLTree T)
{
	if (T != NULL)
	{
		PrintTree(T->Left);
		printf("%d, ", T->Element);
		PrintTree(T->Right);
	}
}

//��δ�ӡ��
void PrintAVLTree(AVLTree T, int m)
{
	int i;
	if (T->Right) PrintAVLTree(T->Right, m + 1);
	for (i = 1; i <= m; i++)
		printf("     ");//��ӡ i ���ո��Ա�ʾ�����
	printf("%d\n", T->Element);//��ӡ T Ԫ��,���� 
	if (T->Left)
		PrintAVLTree(T->Left, m + 1);

}

int main()
{
	ElementType A[] = { 6, 2, 1, 5, 8, 19, -5, 3, 0, 2, 10 };

	AVLTree T = NULL;
	int length = sizeof(A) / sizeof(ElementType);

	for (int i = 0; i < length; i++)
	{
		T = Insert(A[i], T);
	}
	PrintTree(T);
	printf("\n");
	PrintAVLTree(T, 0);
	printf("\n");
	T = Delete(19, T);
	T = Delete(8, T);
	printf("�ؼ���3���ڽڵ�ĸ߶ȣ�%d\n", Height(Find(3, T)));
	PrintAVLTree(T, 0);
	printf("\n");
	T = Delete(2, T);
	printf("�ؼ���3���ڽڵ�ĸ߶ȣ�%d\n", Height(Find(3, T)));
	PrintAVLTree(T, 0);
	T = MakeEmpty(T);
	return 0;
}