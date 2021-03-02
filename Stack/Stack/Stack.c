#include <stdio.h>
#include <stdlib.h>

#define Error( Str )        FatalError( Str )
#define FatalError( Str )   fprintf( stderr, "%s\n", Str ), exit( 1 )

typedef struct Node* PtrToNode;
typedef PtrToNode Stack;
typedef PtrToNode Position;
typedef int ElementType;

struct Node
{
	Position Next;
	ElementType Element;
};

//�ж�ջ�Ƿ�Ϊ�գ���ջ����ͷָ��NULL
int IsEmpty(Stack S)
{
	return S->Next == NULL;
}

//����ջ��Ԫ�ص�ֵ
ElementType Top(Stack S)
{
	if (IsEmpty(S))
		Error("Empty stack");
	else
		return S->Next->Element;
}

//Pop����
void Pop(Stack S)
{
	Position P, TmpCell;

	TmpCell = S->Next;
	S->Next = TmpCell->Next;
	free(TmpCell);
}

//Push����
void Push(ElementType X, Stack S)
{
	Position TmpCell;

	TmpCell = malloc(sizeof(struct Node));
	if (TmpCell == NULL)
		FatalError("Out of space!!");
	TmpCell->Next = S->Next;
	TmpCell->Element = X;
	S->Next = TmpCell;
}

//ɾ��ջ��Ԫ��
void DeleteStack(Stack S)
{
	Position P, TmpCell;

	P = S->Next;
	S->Next = NULL;
	while (P != NULL)
	{
		TmpCell = P->Next;
		free(P);
		P = TmpCell;
	}
}

//���ɿ�ջ
Stack MakeEmpty(Stack S)
{
	if (S != NULL)
	{
		DeleteStack(S);
	}
	S = malloc(sizeof(struct Node));
	if (S == NULL)
		FatalError("Out of space!!");
	S->Next = NULL;
	return S;
}


//��ӡջ
void PrintStack(Stack S)
{
	Position P;

	P = S->Next;
	if (P != NULL)
	{
		printf("ջ��Ԫ�ص�ֵ��%d\n", P->Element);
		P = P->Next;
	}
	while (P != NULL)
	{
		printf("ֵ��%d, ��һ��Ԫ�ص�λ�ã�%p\n", P->Element, P->Next);
		P = P->Next;
	}
}

int main()
{ 
	ElementType A[] = { 6, 2, 1, 5, 8 };
	Stack S = NULL;
	int length = sizeof(A) / sizeof(ElementType);
	S = MakeEmpty(S);
	for (int i = 0; i < length; i++)
	{
		Push(A[i], S);
	}
	PrintStack(S);
	Pop(S);
	PrintStack(S);
	DeleteStack(S);
	printf("�Ƿ��ǿ�ջ��1���ǣ�0�����ǣ���%d\n", IsEmpty(S));
	if (S != NULL)
		free(S);
	return 0;
}