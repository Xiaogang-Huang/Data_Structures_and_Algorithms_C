#pragma once

#define NumSets 10

typedef int DisjointSet[NumSets + 1];
typedef int SetType;
typedef int ElementType;

void Initialize(DisjointSet S);
void SetUnion(DisjointSet S, SetType Root1, SetType Root2);
SetType Find(ElementType X, DisjointSet S);
