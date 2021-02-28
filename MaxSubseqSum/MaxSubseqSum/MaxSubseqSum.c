#include<stdio.h>
#define  max(a,b) ((a)>(b))?(a):(b)

int MaxSubsequenceSum1(const int A[], int N)
{
	int Thissum, Maxsum, i, j, k;

	Maxsum = 0;
	for (i = 0; i < N; i++)
		for (j = i; j < N; j++)
		{
			Thissum = 0;
			for (k = i; k <= j; k++)
			{
				Thissum += A[k];
			}
			if (Thissum > Maxsum)
			{
				Maxsum = Thissum;
			}
		}
	return Maxsum;
}

int MaxSubsequenceSum2(const int A[], int N)
{
	int Thissum, Maxsum, i, k;

	Maxsum = 0;
	for (i = 0; i < N; i++)
	{
		Thissum = 0;
		for (k = i; k <= N; k++)
		{
			Thissum += A[k];
			if (Thissum > Maxsum)
			{
				Maxsum = Thissum;
			}
		}
	}
	return Maxsum;
}

static int MaxSubseqSum(const int A[], int Left, int Right)
{
	int MaxLeftSum, MaxRightSum;
	int MaxLeftBorderSum, MaxRightBorderSum;
	int LeftBorderSum, RightBorderSum;
	int Center, i;
	
	if (Left == Right)
	{
		return max(A[Left], 0);
	}
	Center = (Left + Right) / 2;
	MaxLeftSum = MaxSubseqSum(A, Left, Center);
	MaxRightSum = MaxSubseqSum(A, Center + 1, Right);

	MaxLeftBorderSum = 0; LeftBorderSum = 0;
	for (i = Center; i >= Left; i--)
	{
		LeftBorderSum += A[i];
		if (LeftBorderSum > MaxLeftBorderSum)
			MaxLeftBorderSum = LeftBorderSum;
	}
	MaxRightBorderSum = 0; RightBorderSum = 0;
	for (i = Center+1; i <= Right; i++)
	{
		RightBorderSum += A[i];
		if (RightBorderSum > MaxRightBorderSum)
			MaxRightBorderSum = RightBorderSum;
	}
	return max(max(MaxLeftSum, MaxRightSum), MaxRightBorderSum+ MaxLeftBorderSum);
}

int MaxSubsequenceSum3(const int A[], int N)
{
	return MaxSubseqSum(A, 0, N - 1);
}

int MaxSubsequenceSum4(const int A[], int N)
{
	int Thissum, Maxsum, i;
	
	Maxsum = Thissum = 0;
	for (i = 0; i < N; i++)
	{
		Thissum += A[i];
		if (Thissum > Maxsum)
		{
			Maxsum = Thissum;
		}
		else if (Thissum < 0)
		{
			Thissum = 0;
		}
	}
	return Maxsum;
}

int main()
{
	int A[] = { 4, -3, 5, -2, -1, 2, 6, -2 };
	int N = sizeof(A) / sizeof(A[0]);
	printf("最大子序列和：%d\n", MaxSubsequenceSum1(A, N));
	printf("最大子序列和：%d\n", MaxSubsequenceSum2(A, N));
	printf("最大子序列和：%d\n", MaxSubsequenceSum3(A, N));
	printf("最大子序列和：%d\n", MaxSubsequenceSum4(A, N));
	return 0;
}