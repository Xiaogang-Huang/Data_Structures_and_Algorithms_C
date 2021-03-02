#include <stdio.h>
#include <stdlib.h>
#include "Shortestpath.h"
#include <limits.h>

#define Error(str) FatalError(str)
#define FatalError(str) fprintf(stderr, "%s\n", str), exit(1)
#define NotAVertex (-1)
#define Infinity INT_MAX
#define MinPQSize 3
#define MinData (-32767)

struct QueueRecord {
	int Capacity; //队列的最大容量
	int Front;  //队首的位置
	int Rear;  //队尾的位置
	int Size;
	ElementType* Array;
};

struct Adjacent {
	ElementType Element;
	DistType Weight;
	AdjacentList Next;
};

struct Node {
	ElementType Element;
	int Indegree;
	AdjacentList Adjacent;
};

struct Graph  {
	Vertex AdjGraph;
	int VertexNum;
	int EdgeNum;
};

struct TableEntry {
	AdjacentList Header;
	int Known;
	DistType Dist;
	ElementType Path;
};

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

PtrToGraph CreateGraph(int Size)
{
	PtrToGraph G;
	int i;

	G = malloc(sizeof(struct Graph));
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

void Insert(PtrToGraph G, ElementType Start, ElementType End, DistType Weight)
{
	AdjacentList Tmp;
	Tmp = malloc(sizeof(struct Adjacent));
	if (Tmp == NULL)
		Error("out of space!!!");
	Tmp->Element = End;
	Tmp->Weight = Weight;
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

void PrintGraph(PtrToGraph G)
{
	AdjacentList Tmp;
	printf("图中共有%d个节点，%d条边.\n", G->VertexNum, G->EdgeNum);
	for (int i = 0; i < G->VertexNum; i++)
	{
		printf("第%d个节点的入度为%d, 邻接的顶点及权重：", G->AdjGraph[i].Element + 1, G->AdjGraph[i].Indegree);

		Tmp = G->AdjGraph[i].Adjacent;
		while (Tmp != NULL)
		{
			printf("%d(权重=%3d), ", Tmp->Element + 1, Tmp->Weight);
			Tmp = Tmp->Next;
		}
		printf("\n");
	}
}

void DeleteGraph(PtrToGraph G)
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

//表格初始化
Table InitTable(ElementType X, PtrToGraph G)
{
	Table T;

	X--;
	T = calloc(G->VertexNum, sizeof(struct TableEntry));
	if (T == NULL)
	{
		Error("out of space!!!");
		return NULL;
	}
	for (int i = 0; i < G->VertexNum; i++)
	{
		T[i].Dist = Infinity;
		T[i].Known = 0;
		T[i].Path = NotAVertex;
	}
	T[X].Dist = 0;
	return T;
}

//广度优先搜索
void Unweighted(ElementType X, Table T, PtrToGraph G)
{
	Queue Q;
	ElementType V;
	AdjacentList Tmp;

	X--;
	Q = CreateQueue(G->VertexNum); MakeEmpty(Q);
	Enqueue(X, Q);

	while (!IsEmpty(Q))
	{
		V = Dequeue(Q);
		T[V].Known = 1;

		Tmp = G->AdjGraph[V].Adjacent;
		while (Tmp != NULL)
		{
			if (T[Tmp->Element].Dist == Infinity)
			{
				T[Tmp->Element].Dist = T[V].Dist + 1;
				T[Tmp->Element].Path = V + 1;
				Enqueue(Tmp->Element, Q);
			}
			Tmp = Tmp->Next;
		}
	}
	DisposeQueue(Q);
}

//Dijkstra算法(可以用优先队列优化)
void Dijkstra(ElementType X, Table T, PtrToGraph G)
{
	ElementType V;
	AdjacentList Tmp;
	DistType Tmpdist;
	int i;

	X--;
	for (;;)
	{
		Tmpdist = Infinity;
		V = NotAVertex;
		for (i = 0; i < G->VertexNum; i++)
			if (T[i].Dist < Tmpdist && T[i].Known == 0)
			{
				V = i;
				Tmpdist = T[i].Dist;
			}
		if (V == NotAVertex)
			break;
		T[V].Known = 1;

		Tmp = G->AdjGraph[V].Adjacent;
		while (Tmp != NULL)
		{
			if (T[Tmp->Element].Known == 0)
			{
				if (T[V].Dist + Tmp->Weight < T[Tmp->Element].Dist)
				{
					T[Tmp->Element].Dist = T[V].Dist + Tmp->Weight;
					T[Tmp->Element].Path = V + 1;
				}
			}
			Tmp = Tmp->Next;
		}
	}
}

//打印路径
void PrintPath(Table T, ElementType X)
{
	X--;
	if (T[X].Path != NotAVertex)
	{
		PrintPath(T, T[X].Path);
		printf(" to ");
	}
	printf("%d", X + 1);
}

void PrintTable(Table T, int Size)
{
	printf("Known Dist  Path\n");
	for (int i = 0; i < Size; i++)
	{
		printf("%6d%6d%4d\n", T[i].Known, T[i].Dist, T[i].Path);
	}
}

int main()

{
	PtrToGraph G;
	int size = 7;
	Table T, T1;
	int i, N;
	ElementType X;
	int A[][3] = {
		{1, 2, 2},
		{1, 4, 1},
		{2, 4, 3},
		{2, 5, 10},
		{3, 1, 4},
		{3, 6, 5},
		{4, 3, 2},
		{4, 5, 2},
		{4, 6, 8},
		{4, 7, 4},
		{5, 7, 6},
		{7, 6, 1}
	};

	G = CreateGraph(size);

	N = sizeof(A) / sizeof(A[0]);
	for (i = 0; i < N; i++)
	{
		Insert(G, A[i][0] - 1, A[i][1] - 1, A[i][2]);
	}
	PrintGraph(G);

	X = 3;
	T = InitTable(X, G);
	Unweighted(X, T, G);
	PrintPath(T, 1);
	printf("\n");
	PrintTable(T, size);
	free(T);

	X = 1;
	T1 = InitTable(X, G);
	Dijkstra(X, T1, G);
	PrintPath(T1, 1);
	printf("\n");
	PrintTable(T1, size);
	free(T1);
	DeleteGraph(G);
	return 0;
}