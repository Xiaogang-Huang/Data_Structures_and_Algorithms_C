#include <stdio.h>

typedef int ElementType;

void ShellSort(ElementType A[], int N)
{
	int i, j, Increment;
	ElementType Tmp;

	for (Increment = N / 2; Increment > 0; Increment /= 2)
		for (i = Increment; i < N; i++)
		{
			Tmp = A[i];
			for (j = i; j >= Increment; j -= Increment)
				if (Tmp < A[j-Increment])
					A[j] = A[j - Increment];
				else
					break;
			A[j] = Tmp;
		}
}

int main()
{
	ElementType A[] = { 19, 1, -1, 2, 4, 3, 0, 5, 2 };
	int N = sizeof(A) / sizeof(A[0]);

	ShellSort(A, N);
	for (int i = 0; i < N; i++)
	{
		printf("%d, ", A[i]);
	}
	return 0;
}