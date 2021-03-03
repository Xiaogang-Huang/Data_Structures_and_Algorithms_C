#pragma once

typedef int ElementType;
typedef double DistType;
typedef struct QueueRecord* Queue;
typedef struct Node* Vertex;
typedef struct Adjacent* AdjacentList;
typedef struct TableEntry* Table;
typedef struct Graph* PtrToGraph;
typedef struct HeapStruct* PriorityQueue;

struct QueueRecord;
struct Adjacent;
struct Node;
struct TableEntry;
struct Graph;
struct HeapNode;
struct HeapStruct;

int IsEmpty(Queue Q);
int IsFull(Queue Q);
void MakeEmpty(Queue Q);
Queue CreateQueue(int MaxElements);
void DisposeQueue(Queue Q);
static int Succ(int Value, Queue Q);
void Enqueue(ElementType X, Queue Q);
ElementType Dequeue(Queue Q);
PtrToGraph CreateGraph(int Size);
void Insert(PtrToGraph G, ElementType Start, ElementType End, DistType Weight);
void PrintGraph(PtrToGraph G);
void DeleteGraph(PtrToGraph G);
Table InitTable(ElementType X, PtrToGraph G);
void Unweighted(ElementType X, Table T, PtrToGraph G);
void PrintPath(Table T, ElementType X);
void PrintTable(Table T, int Size);
int IsFullPriorityQueue(PriorityQueue H);
int IsEmptyPriorityQueue(PriorityQueue H);
PriorityQueue Initialize(int MaxElements);
void PriorityQueueInsert(DistType X, ElementType Y, PriorityQueue H);
void PriorityQueueDecreaseKey(ElementType Y, DistType NewX, PriorityQueue H);
ElementType DeleteMin(PriorityQueue H);
void Destroy(PriorityQueue H);
void DijkstraPriorityQueue(ElementType X, Table T, PtrToGraph G);
void PrintHeap(PriorityQueue H, int m, int i);