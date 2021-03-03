#include <stdio.h>
#include <stdlib.h>
#include "MST.h"
#include <limits.h>

#define Error(str) FatalError(str)
#define FatalError(str) fprintf(stderr, "%s\n", str), exit(1)
#define NotAVertex (-1)
#define Infinity INT_MAX
#define MinPQSize 3

struct HeapStruct {
	int Capacity;
	int Size;
	int* Position; //可以根据元素的值找到堆中的位置
	ElementType* HeapPos; //元素在堆中的位置
};

struct TableEntry {
	int Known;
	DistType Dist;
	ElementType Path;
};

struct KruskalTableEntry {
	int Known;
	DistType Dist;
	ElementType Point1;
	ElementType Point2;
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

struct Graph {
	Vertex AdjGraph;
	int VertexNum;
	int EdgeNum;
};

//优先队列
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

	H->Position = calloc(MaxElements + 1, sizeof(int));
	if (H->Position == NULL)
		FatalError("Out of space!!!");

	H->HeapPos = calloc(MaxElements + 1, sizeof(ElementType));
	if (H->HeapPos == NULL)
		FatalError("Out of space!!!");

	H->Capacity = MaxElements;
	H->Size = 0;
	H->Size = 0;

	return H;
}

void PriorityQueueInsert(Table T, ElementType Y, PriorityQueue H)
{
	int i;

	if (IsFullPriorityQueue(H))
	{
		Error("Priority queue size is full!!!");
		return;
	}
	//从堆尾向前查找元素插入的位置
	for (i = ++H->Size; T[H->HeapPos[i / 2]].Dist > T[Y].Dist; i /= 2)
	{
		H->HeapPos[i] = H->HeapPos[i / 2];
		H->Position[H->HeapPos[i]] = i;
	}
	
	H->HeapPos[i] = Y;
	H->Position[Y] = i;
}

void PriorityQueueInsertK(KruskalTable T, ElementType Y, PriorityQueue H)
{
	int i;

	if (IsFullPriorityQueue(H))
	{
		Error("Priority queue size is full!!!");
		return;
	}
	//从堆尾向前查找元素插入的位置
	for (i = ++H->Size; i / 2 != 0 && T[H->HeapPos[i / 2]].Dist > T[Y].Dist; i /= 2)
	{
		H->HeapPos[i] = H->HeapPos[i / 2];
		H->Position[H->HeapPos[i]] = i;
	}

	H->HeapPos[i] = Y;
	H->Position[Y] = i;
}

void PriorityQueueDecreaseKey(Table T, ElementType Y, PriorityQueue H)
{
	int i = 1, j;

	i = H->Position[Y];
	for (j = i; T[H->HeapPos[j / 2]].Dist > T[Y].Dist; j /= 2)
	{
		H->HeapPos[j] = H->HeapPos[j / 2];
		H->Position[H->HeapPos[j]] = j;
	}
	H->HeapPos[j] = Y;
	H->Position[Y] = j;
}

ElementType DeleteMin(Table T, PriorityQueue H)
{
	int i, Child, LastPosition;
	DistType LastDist;
	ElementType MinElement, LastElement;

	if (IsEmptyPriorityQueue(H))
	{
		Error("Priority queue is empty!!!");
		return H->HeapPos[1];
	}
	MinElement = H->HeapPos[1];
	LastElement = H->HeapPos[H->Size];
	LastDist = T[LastElement].Dist;
	LastPosition = H->Position[LastElement];
	H->Size--;

	for (i = 1; i * 2 <= H->Size; i = Child)
	{
		Child = i * 2;
		if (Child != H->Size && T[H->HeapPos[Child + 1]].Dist < T[H->HeapPos[Child]].Dist)
			Child++;
		if (LastDist > T[H->HeapPos[Child]].Dist)
		{
			H->HeapPos[i] = H->HeapPos[Child];
			H->Position[H->HeapPos[i]] = i;
		}
		else
			break;
	}
	
	H->HeapPos[i] = LastElement;
	H->Position[LastElement] = i;
	return MinElement;
}

ElementType DeleteMinK(KruskalTable T, PriorityQueue H)
{
	int i, Child, LastPosition;
	DistType LastDist;
	ElementType MinElement, LastElement;

	if (IsEmptyPriorityQueue(H))
	{
		Error("Priority queue is empty!!!");
		return H->HeapPos[1];
	}
	MinElement = H->HeapPos[1];
	LastElement = H->HeapPos[H->Size];
	LastDist = T[LastElement].Dist;
	LastPosition = H->Position[LastElement];
	H->Size--;

	for (i = 1; i * 2 <= H->Size; i = Child)
	{
		Child = i * 2;
		if (Child != H->Size && T[H->HeapPos[Child + 1]].Dist < T[H->HeapPos[Child]].Dist)
			Child++;
		if (LastDist > T[H->HeapPos[Child]].Dist)
		{
			H->HeapPos[i] = H->HeapPos[Child];
			H->Position[H->HeapPos[i]] = i;
		}
		else
			break;
	}

	H->HeapPos[i] = LastElement;
	H->Position[LastElement] = i;
	return MinElement;
}

void Destroy(PriorityQueue H)
{
	free(H->HeapPos);
	free(H->Position);
	free(H);
}

//图
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
			printf("%d(权重=%5.2f), ", Tmp->Element, Tmp->Weight);
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

//表格
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

void PrintTable(Table T, int Size)
{
	printf("Known Dist  Path\n");
	for (int i = 0; i < Size; i++)
	{
		printf("%6d%5.2f%4d\n", T[i].Known, T[i].Dist, T[i].Path);
	}
}

KruskalTable InitKruskalTable(PtrToGraph G)
{
	KruskalTable T;
	AdjacentList Tmp;
	int X = 0, C = -1;

	T = calloc((G->EdgeNum) / 2, sizeof(struct KruskalTableEntry));
	if (T == NULL)
	{
		Error("out of space!!!");
		return NULL;
	}
	for (int i = 0; i < G->VertexNum; i++)
	{
		Tmp = G->AdjGraph[i].Adjacent;
		while (Tmp != NULL)
		{
			if (Tmp->Element > i)
			{
				C++;
				T[C].Dist = Tmp->Weight;
				T[C].Point1 = i;
				T[C].Point2 = Tmp->Element;
			}
			Tmp = Tmp->Next;
		}
	}
	return T;
}

void PrintKruskalTable(KruskalTable T, int Size)
{
	printf("Known Point1 Point2   Path\n");
	for (int i = 0; i < Size; i++)
	{
		printf("%5d %6d %6d %5.2f\n", T[i].Known, T[i].Point1, T[i].Point2, T[i].Dist);
	}
}

//Prim算法生成最小生成树
void Prim(Table T, PtrToGraph G)
{
	ElementType V;
	AdjacentList Tmp;
	PriorityQueue H;
	int* Inqueue;

	V = 0;
	H = Initialize(G->VertexNum);
	Inqueue = calloc(G->VertexNum, sizeof(int));
	if (Inqueue == NULL)
		Error("Out of space!");
	for (int i = 0; i < G->VertexNum; i++)
		Inqueue[i] = 0;

	PriorityQueueInsert(T, V, H);
	Inqueue[V] = 1;
	for (;;)
	{
		if (IsEmptyPriorityQueue(H))
			break;
		V = DeleteMin(T, H);
		T[V].Known = 1;
		Tmp = G->AdjGraph[V].Adjacent;
		while (Tmp != NULL)
		{
			if (T[Tmp->Element].Known == 0)
			{
				if (Tmp->Weight < T[Tmp->Element].Dist)
				{
					T[Tmp->Element].Dist = Tmp->Weight;
					if (Inqueue[Tmp->Element] == 0)
					{
						PriorityQueueInsert(T, Tmp->Element, H);
						Inqueue[Tmp->Element] = 1;
					}
					else
						PriorityQueueDecreaseKey(T, Tmp->Element, H);
					T[Tmp->Element].Path = V + 1;
				}
			}
			Tmp = Tmp->Next;
		}
	}
	free(Inqueue);
	Destroy(H);
}

//不相交集合
//初始化
DisjointSet DisjointSetInitialize(int Size)
{
	int i;
	int* S;

	S = calloc(Size + 1, sizeof(SetType));
	if (S == NULL)
		Error("Out of space!");
	for (i = 0; i < Size + 1; i++)
		S[i] = 0;

	return S;
}

//集合的并
void SetUnion(DisjointSet S, SetType Root1, SetType Root2)
{
	if (S[Root2] < S[Root1])
		S[Root1] = Root2;
	else
	{
		if (S[Root2] == S[Root1])
			S[Root1]--;
		S[Root2] = Root1;
	}
}

//返回元素X所在集合的根
SetType Find(ElementType X, DisjointSet S)
{
	if (S[X] <= 0)
		return X;
	else
		return S[X] = Find(S[X], S);
}

//Kruskal算法（无向图）
void Kruskal(KruskalTable T, PtrToGraph G)
{
	int EdgeAccepted = 0, i;
	DisjointSet S;
	PriorityQueue H;
	ElementType U;
	SetType s1, s2;

	S = DisjointSetInitialize(G->VertexNum);
	H = Initialize((G->EdgeNum) / 2);
	for (i = 0; i < (G->EdgeNum) / 2; i++)
		PriorityQueueInsertK(T, (ElementType)i, H);

	while (EdgeAccepted < G->VertexNum - 1)
	{
		U = DeleteMinK(T, H);
		s1 = T[U].Point1 + 1;
		s2 = T[U].Point2 + 1;
		if (Find(s1, S) != Find(s2, S))
		{
			EdgeAccepted++;
			SetUnion(S, Find(s1, S), Find(s2, S));
			T[U].Known = 1;
		}
	}
	Destroy(H);
	free(S);
}

//层次打印树
void PrintHeap(Table T, PriorityQueue H, int m, int i)
{
	int j;
	if (2 * i + 1 <= H->Size) PrintHeap(T, H, m + 1, 2 * i + 1);
	for (j = 1; j <= m; j++)
		printf("     ");//打印 i 个空格以表示出层次
	printf("节点=%2d, 距离=%5.2f\n", H->HeapPos[i], T[H->HeapPos[i]].Dist);//打印 H 元素,换行 
	if (2 * i <= H->Size) PrintHeap(T, H, m + 1, 2 * i);
}

int main()

{
	PtrToGraph G;
	int size = 7;
	Table T;
	KruskalTable T1;
	int i, N;
	int A[][3] = {
		{1, 2, 2},
		{1, 4, 1},
		{2, 4, 3},
		{2, 5, 10},
		{3, 1, 4},
		{3, 6, 5},
		{4, 3, 2},
		{4, 5, 7},
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
		Insert(G, A[i][1] - 1, A[i][0] - 1, A[i][2]);
	}
	PrintGraph(G);
	 
	//Prim算法
	T = InitTable(1, G);
	Prim(T, G);
	PrintTable(T, size);
	free(T);

	//Kruskal算法
	T1 = InitKruskalTable(G);
	Kruskal(T1, G);
	PrintKruskalTable(T1, N);
	free(T1);

	DeleteGraph(G);
	return 0;
}