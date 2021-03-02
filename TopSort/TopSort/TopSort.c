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

ElementType Dequeue(Queue Q)
{
	ElementType Result;
	if (IsEmpty(Q))
		Error("Empty queue");
	else
	{
		Q->Size--;
		Result = Q->Array[Q->Front];
		Q->Front = Succ(Q->Front, Q);
	}
	return Result;
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


typedef struct Node* Vertex;
typedef struct Adjacent* AdjacentList;

struct Adjacent {
	ElementType Element;
	AdjacentList Next;
};

struct Node {
	ElementType Element;
	int Indegree;
	AdjacentList Adjacent;
};

typedef struct {
	Vertex AdjGraph;
	int VertexNum;
	int EdgeNum;
} Graph;

Graph* CreateGraph(int Size)
{
	Graph* G;
	int i;

	G = malloc(sizeof(Graph));
	if (G == NULL)
	{
		Error("out of space!!!");
		return NULL;
	}
	G->EdgeNum = 0;
	G->VertexNum = Size;
	G->AdjGraph = calloc(Size, sizeof(struct Node));
	if (G->AdjGraph == NULL)
	{
		Error("out of space!!!");
		return NULL;
	}
	for (i = 0; i < Size; i++)
	{
		G->AdjGraph[i].Element = i;
		G->AdjGraph[i].Indegree = 0;
		G->AdjGraph[i].Adjacent = NULL;
	}
	return G;
}

void Insert(Graph* G, ElementType Start, ElementType End)
{
	AdjacentList Tmp;
	Tmp = malloc(sizeof(struct Adjacent));
	if (Tmp == NULL)
		Error("out of space!!!");
	Tmp->Element = End;
	if (G->AdjGraph[Start].Adjacent == NULL)
	{
		G->AdjGraph[Start].Adjacent = Tmp;
		Tmp->Next = NULL;
	}
	else
	{
		Tmp->Next = G->AdjGraph[Start].Adjacent->Next;
		G->AdjGraph[Start].Adjacent->Next = Tmp;
	}
	G->AdjGraph[End].Indegree++;
	G->EdgeNum++;
}

void PrintGraph(Graph* G)
{
	AdjacentList Tmp;
	printf("图中共有%d个节点，%d条边.\n", G->VertexNum, G->EdgeNum);
	for (int i = 0; i < G->VertexNum; i++)
	{
		printf("第%d个节点的入度为%d, 邻接的顶点：", G->AdjGraph[i].Element + 1, G->AdjGraph[i].Indegree);
		Tmp = G->AdjGraph[i].Adjacent;
		while (Tmp != NULL)
		{
			printf("%d, ", Tmp->Element + 1);
			Tmp = Tmp->Next;
		}
		printf("\n");
	}
}

int* Topsort(Graph* G)
{
	Queue Q;
	int Counter = 0;
	int i;
	int* TopNum;
	int* Indegree;
	AdjacentList Tmp;

	Indegree = calloc(G->VertexNum, sizeof(int));
	if (Indegree == NULL)
	{
		Error("out of space!!!");
		return NULL;
	}
	for (i = 0; i < G->VertexNum; i++)
		Indegree[i] = G->AdjGraph[i].Indegree;

	TopNum = malloc(G->VertexNum * sizeof(int));
	if (TopNum == NULL)
	{
		Error("out of space!!!");
		return NULL;
	}
	Q = CreateQueue(G->VertexNum);
	MakeEmpty(Q);
	for (i = 0; i < G->VertexNum; i++)
		if (Indegree[i] == 0)
			Enqueue(i, Q);

	while (!IsEmpty(Q))
	{
		i = Dequeue(Q);
		TopNum[i] = ++Counter;

		Tmp = G->AdjGraph[i].Adjacent;
		while (Tmp != NULL)
		{
			if (--Indegree[Tmp->Element] == 0)
				Enqueue(Tmp->Element, Q);
			Tmp = Tmp->Next;
		}
	}
	free(Indegree);
	DisposeQueue(Q);
	if (Counter != G->VertexNum)
	{
		free(TopNum);
		Error("Graph has a cycle!");
	}
	return TopNum;
}

void DeleteGraph(Graph* G)
{
	AdjacentList Tmp, Tmp1;
	for (int i = 0; i < G->VertexNum; i++)
	{
		Tmp = G->AdjGraph[i].Adjacent;
		while (Tmp != NULL)
		{
			Tmp1 = Tmp->Next;
			free(Tmp);
			Tmp = Tmp1;
		}
	}
	free(G->AdjGraph);
	free(G);
}

int main()

{

	Graph* G;
	int size = 6;
	int i, N;
	int* Topnum;
	int A[][2] = {
		{1, 2},
		{1, 3},
		{1, 4},
		{3, 2},
		{3, 5},
		{4, 5},
		{6, 4},
		{6, 5}
	};

	G = CreateGraph(size);
	N = sizeof(A) / sizeof(A[0]);
	for (i = 0; i < N; i++)
	{
		Insert(G, A[i][0] - 1, A[i][1] - 1);
	}
	PrintGraph(G);

	Topnum = Topsort(G);
	for (i = 0; i < size; i++)
	{
		printf("%d, ", Topnum[i]);
	}
	printf("\n");
	DeleteGraph(G);
	return 0;
}