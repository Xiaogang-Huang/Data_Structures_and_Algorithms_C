#include <stdio.h>

typedef int ElementType;

void swap(ElementType* a, ElementType* b)
{
	ElementType Tmp;

	Tmp = *a;
	*a = *b;
	*b = Tmp;
}

void Heapify(ElementType A[], int i, int N)
{
	int l, r, largest;
	ElementType Temp;

	l = 2 * i + 1;
	r = 2 * (i + 1);
	if (l <= N - 1 && A[l] > A[i])
		largest = l;
	else
		largest = i;
	if (r <= N - 1 && A[r] > A[largest])
		largest = r;
	if (largest != i)
	{
		swap(&A[i], &A[largest]);
		Heapify(A, largest, N);
	}
}

//层次打印树
void PrintHeap(ElementType A[], int m, int i, int N)
{
	int j;

	if (2 * (i + 1) < N) PrintHeap(A, m + 1, 2 * (i + 1), N);
	for (j = 1; j <= m; j++)
		printf("     ");//打印 i 个空格以表示出层次
	printf("%6d\n", A[i]);//打印 H 元素,换行 
	if (2 * i + 1 < N) PrintHeap(A, m + 1, 2 * i + 1, N);
}

void HeapSort(ElementType A[], int N)
{
	int i;

	//建立最大堆
	for (i = N / 2; i >= 0; i--)
		Heapify(A, i, N);

	//排序
	for (i = N; i > 0; i--)
	{
		swap(&A[0], &A[i - 1]);
		Heapify(A, 0, i - 1);
	}
}

int main()
{
	ElementType A[] = { 19, 1, -1, 2, 4, 3, 0, 5, 2 };
	int N = sizeof(A) / sizeof(A[0]);

	HeapSort(A, N);
	for (int i = 0; i < N; i++)
	{
		printf("%d, ", A[i]);
	}
	return 0;
}