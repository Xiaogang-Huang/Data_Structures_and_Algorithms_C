#include <stdio.h>
#include <stdlib.h>

#define Error(str) FatalError(str)
#define FatalError(str) fprintf(stderr, "%s\n", str), exit(1)

typedef int ElementType;

struct QueueRecord {
	int Capacity; //队列的最大容量
	int Front;  //队首的位置
	int Rear;  //队尾的位置
	int Size; 
	ElementType* Array;
};

typedef struct QueueRecord* Queue;

int IsEmpty(Queue Q)
{
	return Q->Size == 0;
}

int IsFull(Queue Q)
{
	return Q->Size == Q->Capacity;
}

void MakeEmpty(Queue Q)
{
	Q->Size = 0;
	Q->Front = 1;
	Q->Rear = 0;
}

Queue CreateQueue(int MaxElements)
{
	Queue Q;
	
	Q = malloc(sizeof(struct QueueRecord));
	if (Q == NULL)
		FatalError("Out of space!!!");

	Q->Array = malloc(MaxElements * sizeof(ElementType));
	if (Q->Array == NULL)
		FatalError("Out of space!!!");

	Q->Capacity = MaxElements;
	MakeEmpty(Q);

	return Q;
}

void DisposeQueue(Queue Q)
{
	if (Q != NULL)
	{
		free(Q->Array);
		free(Q);
	}
}

static int Succ(int Value, Queue Q)
{
	if (++Value == Q->Capacity)
	{
		Value = 0;
	}
	return Value;
}

void Enqueue(ElementType X, Queue Q)
{
	if (IsFull(Q))
		Error("Full queue");
	else
	{
		Q->Size++;
		Q->Rear = Succ(Q->Rear, Q);
		Q->Array[Q->Rear] = X;
	}
}

ElementType Front(Queue Q)
{
	return Q->Array[Q->Front];
}

void Dequeue(Queue Q)
{
	if (IsEmpty(Q))
		Error("Empty queue");
	else
	{
		Q->Size--;
		Q->Front = Succ(Q->Front, Q);
	}
}

void PrintQueue(Queue Q)
{
	int i;

	if (IsEmpty(Q))
		Error("Empty queue");
	else
	{
		printf("Q->Size = %d\n", Q->Size);
		printf("Q->Front = %d\n", Q->Front);
		printf("Q->Rear = %d\n", Q->Rear);
		for (i = Q->Front; i != Q->Rear; i = Succ(i, Q))
			printf("%d\n", Q->Array[i]);
	}
		
}

int main() {
	int A[] = { 1,2,3,4,5,6,7,8,9,10 };
	int i = 0, N = sizeof(A) / sizeof(A[0]);
	Queue Q;
	
	Q = CreateQueue(N);

	for (i = 0; i < 8; i++)
		Enqueue(A[i], Q);
	PrintQueue(Q);
	Dequeue(Q);
	PrintQueue(Q);
	for (i = 8; i < N; i++)
		Enqueue(A[i], Q);
	PrintQueue(Q);
	Enqueue(10, Q);
	PrintQueue(Q);
	DisposeQueue(Q);
	return 0;
}