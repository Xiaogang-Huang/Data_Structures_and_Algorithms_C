#pragma once

typedef int ElementType;
typedef double DistType;
typedef struct Node* Vertex;
typedef struct Adjacent* AdjacentList;
typedef struct TableEntry* Table;
typedef struct KruskalTableEntry* KruskalTable;
typedef struct Graph* PtrToGraph;
typedef struct HeapStruct* PriorityQueue;
typedef ElementType SetType;
typedef int* DisjointSet;

struct Adjacent;
struct Node;
struct TableEntry;
struct KruskalTableEntry;
struct Graph;
struct HeapStruct;

PtrToGraph CreateGraph(int Size);
void Insert(PtrToGraph G, ElementType Start, ElementType End, DistType Weight);
void PrintGraph(PtrToGraph G);
void DeleteGraph(PtrToGraph G);

Table InitTable(ElementType X, PtrToGraph G);
void PrintTable(Table T, int Size);
KruskalTable InitKruskalTable(PtrToGraph G);
void PrintKruskalTable(KruskalTable T, int Size);

int IsFullPriorityQueue(PriorityQueue H);
int IsEmptyPriorityQueue(PriorityQueue H);
PriorityQueue Initialize(int MaxElements);
void PriorityQueueInsert(Table T, ElementType Y, PriorityQueue H);
void PriorityQueueInsertK(KruskalTable T, ElementType Y, PriorityQueue H);
void PriorityQueueDecreaseKey(Table T, ElementType Y, PriorityQueue H);
ElementType DeleteMin(Table T, PriorityQueue H);
ElementType DeleteMinK(KruskalTable T, PriorityQueue H);
void Destroy(PriorityQueue H);
void PrintHeap(Table T, PriorityQueue H, int m, int i);

void Prim(Table T, PtrToGraph G);

DisjointSet DisjointSetInitialize(int Size);
void SetUnion(DisjointSet S, SetType Root1, SetType Root2);
SetType Find(ElementType X, DisjointSet S);

void Kruskal(KruskalTable T, PtrToGraph G);