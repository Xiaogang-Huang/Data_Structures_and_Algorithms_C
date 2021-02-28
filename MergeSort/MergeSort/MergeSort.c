#include <stdio.h>
#include <stdlib.h>

#define FatalError( Str )   fprintf( stderr, "%s\n", Str ), exit( 1 )

typedef int ElementType;

void Merge(ElementType A[], ElementType TmpArray[], int Lpos, int Rpos, int RightEnd)
{
	int i, LeftEnd, NumElements, TmpPos;

	LeftEnd = Rpos - 1;
	TmpPos = Lpos;
	NumElements = RightEnd - Lpos + 1;

	while (Lpos <= LeftEnd && Rpos <= RightEnd)
	{
		if (A[Lpos] <= A[Rpos])
			TmpArray[TmpPos++] = A[Lpos++];
		else
			TmpArray[TmpPos++] = A[Rpos++];
	}

	while (Lpos <= LeftEnd)
		TmpArray[TmpPos++] = A[Lpos++];
	while (Rpos <= RightEnd)
		TmpArray[TmpPos++] = A[Rpos++];

	for (i = 0; i < NumElements; i++, RightEnd--)
		A[RightEnd] = TmpArray[RightEnd];
}

void Msort(ElementType A[], ElementType TmpArray[], int Left, int Right)
{
	int Center;

	if (Left < Right)
	{
		Center = (Right + Left) / 2;
		Msort(A, TmpArray, Left, Center);
		Msort(A, TmpArray, Center + 1, Right);
		Merge(A, TmpArray, Left, Center + 1, Right);
	}
}

void MergeSort(ElementType A[], int N)
{
	ElementType* TmpArray;

	TmpArray = malloc(sizeof(ElementType) * N);
	if (TmpArray != NULL)
	{
		Msort(A, TmpArray, 0, N - 1);
		free(TmpArray);
	}
	else
		FatalError("Out of space!!!");
}

int main()
{
	ElementType A[] = { 19, 1, -1, 2, 4, 3, 0, 5, 2 };
	int N = sizeof(A) / sizeof(A[0]);

	MergeSort(A, N);
	for (int i = 0; i < N; i++)
	{
		printf("%d, ", A[i]);
	}
	return 0;
}