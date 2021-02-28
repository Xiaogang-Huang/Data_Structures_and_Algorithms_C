#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

#define Error( Str )        FatalError( Str )
#define FatalError( Str )   fprintf( stderr, "%s\n", Str ), exit( 1 )
#define MinPQSize 3
#define MinData (-32767)

struct HeapStruct {
	int Capacity;
	int Size;
	ElementType* Elements;
};

PriorityQueue Initialize(int MaxElements)
{
	PriorityQueue H;

	if (MaxElements < MinPQSize)
		Error("Priority queue size is too small!!!");
	
	H = malloc(sizeof(struct HeapStruct));
	if (H == NULL)
		FatalError("Out of space!!!");

	H->Elements = malloc(sizeof(ElementType) * (MaxElements + 1));
	if (H->Elements == NULL)
		FatalError("Out of space!!!");

	H->Capacity = MaxElements;
	H->Size = 0;
	H->Elements[0] = MinData;

	return H;
}

void Heapify(ElementType A[], int i, int N)
{
	int l, r, largest;
	ElementType Temp;

	l = 2 * i + 1;
	r = 2 * (i + 1);
	if (l <= N - 1 && A[l] < A[i])
		largest = l;
	else
		largest = i;
	if (r <= N - 1 && A[r] < A[largest])
		largest = r;
	if (largest != i)
	{
		Temp = A[i];
		A[i] = A[largest];
		A[largest] = Temp;
		Heapify(A, largest, N);
	}
}

PriorityQueue BuildMinHeap(PriorityQueue H, ElementType A[], int N)
{
	int i, x;
	x = (N - 2) / 2;
	for (i = x; i > -1; i--)
		Heapify(A, i, N);

	for (i = 0; i < N; i++)
	{
		H->Elements[i + 1] = A[i];
	}
	H->Size = N;
	return H;
}

void MakeEmpty(PriorityQueue H)
{
	H->Size = 0;
}

void Destroy(PriorityQueue H)
{
	free(H->Elements);
	free(H);
}

void Insert(ElementType X, PriorityQueue H)
{
	int i;

	if (IsFull(H))
	{
		Error("Priority queue size is full!!!");
		return;
	}
	//从堆尾向前查找元素插入的位置
	for (i = ++H->Size; H->Elements[i/2] > X; i /= 2)
	{
		H->Elements[i] = H->Elements[i / 2];
	}
	H->Elements[i] = X;
}

int IsFull(PriorityQueue H)
{
	return H->Size == H->Capacity;
}

int IsEmpty(PriorityQueue H)
{
	return H->Size == 0;
}

ElementType FindMin(PriorityQueue H)
{
	if (!IsEmpty(H))
	{
		return H->Elements[1];
	}
	Error("Priority queue is empty!!!");
	return H->Elements[0];
}

ElementType DeleteMin(PriorityQueue H)
{
	int i, Child;
	ElementType MinElement, LastElement;

	if (IsEmpty(H))
	{
		Error("Priority queue is empty!!!");
		return H->Elements[0];
	}
	MinElement = H->Elements[1];
	LastElement = H->Elements[H->Size--];

	for (i = 1; i * 2 <= H->Size; i = Child)
	{
		Child = i * 2;
		if (Child != H->Size && H->Elements[Child + 1] < H->Elements[Child])
			Child++;
		if (LastElement > H->Elements[Child])
			H->Elements[i] = H->Elements[Child];
		else
			break;
	}

	H->Elements[i] = LastElement;
	return MinElement;
}

//层次打印树
void PrintHeap(PriorityQueue H, int m, int i)
{
	int j;
	if (2 * i + 1 <= H->Size) PrintHeap(H, m + 1, 2 * i + 1);
	for (j = 1; j <= m; j++)
		printf("     ");//打印 i 个空格以表示出层次
	printf("%6d\n", H->Elements[i]);//打印 H 元素,换行 
	if (2 * i <= H->Size) PrintHeap(H, m + 1, 2 * i);
}

int main()
{
	ElementType A[] = { 6, 2, 1, 5, 8, 19, -5, 3, 0, 2, 10 };
	ElementType X;
	PriorityQueue H, H1;
	int length = sizeof(A) / sizeof(ElementType);

	H = Initialize(length);
	for (int i = 0; i < length; i++)
	{
		Insert(A[i], H);
	}
	PrintHeap(H, 0, 1);
	printf("\n");
	X = DeleteMin(H);
	PrintHeap(H, 0, 1);
	printf("\n");
	Destroy(H);

	H1 = Initialize(length);
	H1 = BuildMinHeap(H1, A, length);
	PrintHeap(H1, 0, 1);
	printf("\n");
	X = DeleteMin(H1);
	PrintHeap(H1, 0, 1);
	printf("\n");
	Destroy(H1);
	return 0;
}