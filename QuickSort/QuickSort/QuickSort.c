#include <stdio.h>

#define Cutoff (3)
#define Error( Str )        FatalError( Str )
#define FatalError( Str )   fprintf( stderr, "%s\n", Str ), exit( 1 )

typedef int ElementType;

void swap(ElementType* a, ElementType* b)
{
	ElementType Temp;
	Temp = *a;
	*a = *b;
	*b = Temp;
}

void InsertionSort(ElementType A[], int N)
{
	int i, p;
	ElementType Tmp;

	for (i = 0; i < N; i++)
	{
		Tmp = A[i];
		for (p = i; p > 0 && A[p - 1] > Tmp; p--)
		{
			A[p] = A[p - 1];
		}
		A[p] = Tmp;
	}
}

ElementType Median3(ElementType A[], int Left, int Right)
{
	int Center = (Left + Right) / 2;

	if (A[Left] > A[Center])
		swap(&A[Left], &A[Center]);
	if (A[Left] > A[Right])
		swap(&A[Left], &A[Right]);
	if (A[Center] > A[Right])
		swap(&A[Center], &A[Right]);

	swap(&A[Center], &A[Right - 1]);
	return A[Right - 1];
}

void Qsort(ElementType A[], int Left, int Right)
{
	int i, j;
	ElementType Pivot;

	if (Left + Cutoff <= Right)
	{
		Pivot = Median3(A, Left, Right);
		i = Left; j = Right - 1;
		for (;;)
		{
			while (A[++i] < Pivot) {}
			while (A[--j] > Pivot) {}
			if (i < j)
				swap(&A[i], &A[j]);
			else
				break;
		}
		swap(&A[i], &A[Right - 1]);

		Qsort(A, Left, i - 1);
		Qsort(A, i + 1, Right);
	}
	else
		InsertionSort(A + Left, Right - Left + 1);
}

void QuickSort(ElementType A[], int N)
{
	Qsort(A, 0, N - 1);
}


//选择数组中第k小的元素
void Qselect(ElementType A[], int k, int Left, int Right)
{
	int i, j;
	ElementType Pivot;

	if (Left + Cutoff <= Right)
	{
		Pivot = Median3(A, Left, Right);
		i = Left; j = Right - 1;
		for (;;)
		{
			while (A[++i] < Pivot) {}
			while (A[--j] > Pivot) {}
			if (i < j)
				swap(&A[i], &A[j]);
			else
				break;
		}
		swap(&A[i], &A[Right - 1]);

		if (k <= i)
			Qselect(A, k, Left, i - 1);
		if (k > i + 1)
			Qselect(A, k, i + 1, Right);
	}
	else
		InsertionSort(A + Left, Right - Left + 1);
}

int main()
{
	ElementType A[] = { 19, 1, -1, 2, 4, 3, 0, 5, 2 };
	int N = sizeof(A) / sizeof(A[0]);

	Qselect(A, 3, 0, N - 1);
	printf("数组中第3小的元素：%d\n", A[2]);

	QuickSort(A, N);
	for (int i = 0; i < N; i++)
	{
		printf("%d, ", A[i]);
	}

	return 0;
}