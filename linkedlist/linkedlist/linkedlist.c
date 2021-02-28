#include <stdio.h>
#include "linkedlist.h"
#include <stdlib.h>

#define Error( Str )        FatalError( Str )
#define FatalError( Str )   fprintf( stderr, "%s\n", Str ), exit( 1 )

struct Node
{
	Position Next;
	ElementType Element;
};

//�ж������Ƿ�Ϊ�գ���������ͷָ��NULL
int IsEmpty(List L)
{
	return L->Next == NULL;
}

//���Ե�ǰλ���Ƿ��������ĩβ�ĺ���
int IsLast(Position P, List L)
{
	return P->Element == NULL;
}

//���عؼ����״γ��ֵ�λ��
Position Find(ElementType X, List L) 
{
	Position P;
	P = L->Next;
	while (P != NULL && P->Element != X)
	{
		P = P->Next;
	}
	return P;
}

//�ҵ��ؼ��ֳ���λ�õ�ǰ��Ԫ
Position FindPrevious(ElementType X, List L)
{
	Position P;
	P = L;
	while (P->Next != NULL && P->Next->Element != X)
	{
		P = P->Next;
	}
	return P;
}

//ɾ������ĳ��Ԫ��
void Delete(ElementType X, List L)
{
	Position P, TmpCell;

	P = FindPrevious(X, L);
	if (P!=IsLast(P, L))
	{
		TmpCell = P->Next;
		P->Next = TmpCell->Next;
		free(TmpCell);
	}
}

//����Ԫ��
void Insert(ElementType X, List L, Position P)
{
	Position TmpCell;

	TmpCell = malloc(sizeof(struct Node));
	if (TmpCell == NULL)
		FatalError("Out of space!!");
	TmpCell->Next = P->Next;
	TmpCell->Element = X;
	P->Next = TmpCell;
}

void DeleteList(List L) 
{
	Position P, TmpCell;

	P = L->Next;
	L->Next = NULL;
	while (P != NULL)
	{
		TmpCell = P->Next;
		free(P);
		P = TmpCell;
	}
}

Position Header(List L)
{
	return L;
}

Position First(List L)
{
	return L->Next;
}

ElementType Retrieve(Position P)
{
	return P->Element;
}

List MakeEmpty(List L)
{
	if (L != NULL)
	{
		DeleteList(L);
	}
	L = malloc(sizeof(struct Node));
	if (L == NULL)
		FatalError("Out of space!!");
	L->Next = NULL;
	return L;
}

Position Advance(Position P)
{
	return P->Next;
}

void PrintList(List L)
{
	Position P;

	P = L->Next;
	while (P != NULL)
	{
		printf("ֵ��%d, ��һ��Ԫ�ص�λ�ã�%p\n", P->Element, P->Next);
		P = P->Next;
	}
}

int main()
{
	ElementType A[] = { 6, 2, 1, 5, 8 };
	List L = NULL;
	int length = sizeof(A) / sizeof(ElementType);
	L = MakeEmpty(L);

	for (int i = 0; i < length; i++)
	{
		Insert(A[i], L, L);
	}
	PrintList(L);
	DeleteList(L);
	printf("%d\n", IsEmpty(L));
	if (L != NULL)
		free(L);
	return 0;
}