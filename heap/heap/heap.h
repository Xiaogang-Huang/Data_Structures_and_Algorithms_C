typedef int ElementType;

#ifndef _HEAP_H
#define _HEAP_H

struct HeapStruct;
typedef struct HeapStruct* PriorityQueue;

PriorityQueue Initialize(int MaxElements);
void Destroy(PriorityQueue H);
void MakeEmpty(PriorityQueue H);
void Insert(ElementType X, PriorityQueue H);
ElementType DeleteMin(PriorityQueue H);
ElementType FindMin(PriorityQueue H);
int IsEmpty(PriorityQueue H);
int IsFull(PriorityQueue H);
void PrintHeap(PriorityQueue H, int m, int i);
PriorityQueue BuildMinHeap(PriorityQueue H, ElementType A[], int N);
void Heapify(ElementType A[], int i, int N);

#endif // !_HEAP_H

