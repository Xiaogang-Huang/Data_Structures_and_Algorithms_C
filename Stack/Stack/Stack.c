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

//判断栈是否为空，空栈：表头指向NULL
int IsEmpty(Stack S)
{
	return S->Next == NULL;
}

//返回栈顶元素的值
ElementType Top(Stack S)
{
	if (IsEmpty(S))
		Error("Empty stack");
	else
		return S->Next->Element;
}

//Pop函数
void Pop(Stack S)
{
	Position P, TmpCell;

	TmpCell = S->Next;
	S->Next = TmpCell->Next;
	free(TmpCell);
}

//Push函数
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

//删除栈中元素
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

//生成空栈
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


//打印栈
void PrintStack(Stack S)
{
	Position P;

	P = S->Next;
	if (P != NULL)
	{
		printf("栈顶元素的值：%d\n", P->Element);
		P = P->Next;
	}
	while (P != NULL)
	{
		printf("值：%d, 下一个元素的位置：%p\n", P->Element, P->Next);
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
	printf("是否是空栈（1：是，0：不是）：%d\n", IsEmpty(S));
	if (S != NULL)
		free(S);
	return 0;
}