#include <stdio.h>

typedef int ElementType;

void InsertionSort(ElementType A[], int N)
{
	int i, p;
	ElementType Tmp;

	for (i = 0; i < N; i++)
	{
		Tmp = A[i];
		for (p = i; p > 0 && A[p-1] > Tmp; p--)
		{
			A[p] = A[p - 1];
		}
		A[p] = Tmp;
	}
}

int main()
{
	ElementType A[] = { 19, 1, -1, 2, 4, 3, 0, 5, 2 };
	int N = sizeof(A) / sizeof(A[0]);

	InsertionSort(A, N);
	for (int i = 0; i < N; i++)
	{
		printf("%d, ", A[i]);
	}
	return 0;
}