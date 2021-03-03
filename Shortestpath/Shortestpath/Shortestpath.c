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
		printf("第%d个节点的入度为%d, 邻接的顶点及权重：", G->AdjGraph[i].Element, G->AdjGraph[i].Indegree);

		Tmp = G->AdjGraph[i].Adjacent;
		while (Tmp != NULL)
		{
			printf("%d(权重=%3d), ", Tmp->Element, Tmp->Weight);
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

//Dijkstra算法_优先队列优化
struct HeapNode {
	ElementType Elements;
	DistType Dist;
	int Position;
};

struct HeapStruct {
	int Capacity;
	int Size;
	struct HeapNode* PtrToNode;
};

typedef struct HeapStruct* PriorityQueue;

int IsFullPriorityQueue(PriorityQueue H)
{
	return H->Size == H->Capacity;
}

int IsEmptyPriorityQueue(PriorityQueue H)
{
	return H->Size == 0;
}

PriorityQueue Initialize(int MaxElements)
{
	PriorityQueue H;

	if (MaxElements < MinPQSize)
		Error("Priority queue size is too small!!!");

	H = malloc(sizeof(struct HeapStruct));
	if (H == NULL)
		FatalError("Out of space!!!");

	H->PtrToNode = calloc(MaxElements + 1, sizeof(struct HeapNode));
	if (H->PtrToNode == NULL)
		FatalError("Out of space!!!");

	H->Capacity = MaxElements;
	H->Size = 0;
	H->PtrToNode[0].Elements = MinData;
	H->PtrToNode[0].Dist = MinData;

	return H;
}

void PriorityQueueInsert(DistType X, ElementType Y, PriorityQueue H)
{
	int i;

	if (IsFullPriorityQueue(H))
	{
		Error("Priority queue size is full!!!");
		return;
	}
	//从堆尾向前查找元素插入的位置
	for (i = ++H->Size; H->PtrToNode[i / 2].Dist > X; i /= 2)
	{
		H->PtrToNode[i].Dist = H->PtrToNode[i / 2].Dist;
		H->PtrToNode[i].Elements = H->PtrToNode[i / 2].Elements;
		H->PtrToNode[H->PtrToNode[i / 2].Elements].Position = i;
	}
	H->PtrToNode[i].Dist = X;
	H->PtrToNode[i].Elements = Y;
	H->PtrToNode[Y].Position = i;
}

//层次打印树
void PrintHeap(PriorityQueue H, int m, int i)
{
	int j;
	if (2 * i + 1 <= H->Size) PrintHeap(H, m + 1, 2 * i + 1);
	for (j = 1; j <= m; j++)
		printf("     ");//打印 i 个空格以表示出层次
	printf("节点=%2d, 距离=%3d\n", H->PtrToNode[i].Elements, H->PtrToNode[i].Dist);//打印 H 元素,换行 
	if (2 * i <= H->Size) PrintHeap(H, m + 1, 2 * i);
}

void PriorityQueueDecreaseKey(ElementType Y, DistType NewX, PriorityQueue H)
{
	int i = 1, j;

	i = H->PtrToNode[Y].Position;
	for (j = i; H->PtrToNode[j / 2].Dist > NewX; j /= 2)
	{
		H->PtrToNode[j].Dist = H->PtrToNode[j / 2].Dist;
		H->PtrToNode[j].Elements = H->PtrToNode[j / 2].Elements;
		H->PtrToNode[H->PtrToNode[j / 2].Elements].Position = j;
	}
	H->PtrToNode[j].Dist = NewX;
	H->PtrToNode[j].Elements = Y;
	H->PtrToNode[Y].Position = i;
}

ElementType DeleteMin(PriorityQueue H)
{
	int i, Child, LastPosition;
	DistType LastDist;
	ElementType MinElement, LastElement;

	if (IsEmptyPriorityQueue(H))
	{
		Error("Priority queue is empty!!!");
		return H->PtrToNode[0].Elements;
	}
	MinElement = H->PtrToNode[1].Elements;
	LastDist = H->PtrToNode[H->Size].Dist;
	LastElement = H->PtrToNode[H->Size].Elements;
	LastPosition = H->PtrToNode[LastElement].Position;
	H->Size--;

	for (i = 1; i * 2 <= H->Size; i = Child)
	{
		Child = i * 2;
		if (Child != H->Size && H->PtrToNode[Child + 1].Dist < H->PtrToNode[Child].Dist)
			Child++;
		if (LastDist > H->PtrToNode[Child].Dist)
		{
			H->PtrToNode[i].Dist = H->PtrToNode[Child].Dist;
			H->PtrToNode[i].Elements = H->PtrToNode[Child].Elements;
			H->PtrToNode[H->PtrToNode[Child].Elements].Position = i;
		}
		else
			break;
	}
	H->PtrToNode[i].Dist = LastDist;
	H->PtrToNode[i].Elements = LastElement;
	H->PtrToNode[LastElement].Position = i;
	return MinElement;
}

void Destroy(PriorityQueue H)
{
	free(H->PtrToNode);
	free(H);
}

void DijkstraPriorityQueue(ElementType X, Table T, PtrToGraph G)
{
	ElementType V;
	AdjacentList Tmp;
	PriorityQueue H;
	int* Inqueue;

	X--;
	H = Initialize(G->VertexNum);
	Inqueue = calloc(G->VertexNum, sizeof(int));
	if (Inqueue == NULL)
	{
		Error("Out of space!");
	}
	for (int i = 0; i < G->VertexNum; i++)
		Inqueue[i] = 0;
	PriorityQueueInsert(T[X].Dist, X, H);
	Inqueue[X] = 1;
	for (;;)
	{
		if (IsEmptyPriorityQueue(H))
			break;
		V = DeleteMin(H);
		T[V].Known = 1;
		Tmp = G->AdjGraph[V].Adjacent;
		while (Tmp != NULL)
		{
			if (T[Tmp->Element].Known == 0)
			{
				if (T[V].Dist + Tmp->Weight < T[Tmp->Element].Dist)
				{
					if (Inqueue[Tmp->Element] == 0)
					{
						PriorityQueueInsert(T[V].Dist + Tmp->Weight, Tmp->Element, H);
						Inqueue[Tmp->Element] = 1;
					}
					else
						PriorityQueueDecreaseKey(Tmp->Element, T[V].Dist + Tmp->Weight, H);
					T[Tmp->Element].Dist = T[V].Dist + Tmp->Weight;
					T[Tmp->Element].Path = V + 1;
				}
			}
			Tmp = Tmp->Next;
		}
	}
	free(Inqueue);
	Destroy(H);
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
	Table T, T1, T2;
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
	printf("无权重最短路径:\n");
	X = 3;
	T = InitTable(X, G);
	Unweighted(X, T, G);
	PrintPath(T, 1);
	printf("\n");
	PrintTable(T, size);
	free(T);
	printf("Dijkstra算法求最短路径:\n");
	X = 1;
	T1 = InitTable(X, G);
	Dijkstra(X, T1, G);
	PrintPath(T1, 3);
	printf("\n");
	PrintTable(T1, size);
	free(T1);
	printf("优先队列优化的Dijkstra算法求最短路径:\n");
	X = 1;
	T2 = InitTable(X, G);
	DijkstraPriorityQueue(X, T2, G);
	PrintPath(T2, 3);
	printf("\n");
	PrintTable(T2, size);
	free(T2);

	DeleteGraph(G);
	return 0;
}