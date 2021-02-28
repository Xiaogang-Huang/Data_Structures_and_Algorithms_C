#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#define FatalError( Str )   fprintf( stderr, "%s\n", Str ), exit( 1 )

int* RaidxSort(int A[], int N)
{
	int Max, Min, i, M;
	int* Count;
	int* Tmp;

	Max = INT_MIN;
	Min = INT_MAX;
	for (i = 0; i < N; i++)
	{
		if (A[i] > Max)
			Max = A[i];
		if (A[i] < Min)
			Min = A[i];
	}
	M = Max - Min + 1;
	Count = calloc(M, sizeof(int));
	if (Count == NULL)
		FatalError("Out of space!!!");
	for (i = 0; i < M; i++)
		Count[i] = (int)0;

	Tmp = malloc(N * sizeof(int));
	if (Tmp == NULL)
		FatalError("Out of space!!!");
	for (i = 0; i < N; i++)
		Tmp[i] = A[i];

	for (i = 0; i < N; i++)
		Count[A[i] - Min] += 1;
	for (i = 1; i < M; i++)
		Count[i] += Count[i - 1];
	for (i = N - 1; i > -1; i--)
		A[--Count[Tmp[i] - Min]] = Tmp[i];

	free(Count);
	free(Tmp);
}

int main()
{
	int A[] = { 19, 1, -10, 2, 4, 3, 0, 5, 6 };
	int N = sizeof(A) / sizeof(A[0]);

	RaidxSort(A, N);
	for (int i = 0; i < N; i++)
	{
		printf("%d, ", A[i]);
	}

	return 0;
}