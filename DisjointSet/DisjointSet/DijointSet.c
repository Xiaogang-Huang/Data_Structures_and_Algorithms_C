#include <stdio.h>
#include <stdlib.h>
#include "DisjointSet.h"

#define Error(str) FatalError(str)
#define FatalError(str) fprintf(stderr, "%s\n", str), exit(1)

//初始化
void Initialize(DisjointSet S)
{
	int i;

	for (i = NumSets + 1; i > 0; i--)
		S[i] = 0;
}

//集合的并
void SetUnion(DisjointSet S, SetType Root1, SetType Root2)
{
	if (S[Root2] < S[Root1])
		S[Root1] = Root2;
	else
	{
		if (S[Root2] == S[Root1])
			S[Root1]--;
		S[Root2] = Root1;
	}
}

//返回元素X所在集合的根
SetType Find(ElementType X, DisjointSet S)
{
	if (S[X] <= 0)
		return X;
	else
		return S[X] = Find(S[X], S);
}

int main()
{
    DisjointSet S;
    int i = 0;
    int s = -1;
    int temp = -1, X1, X2;
    SetType root1, root2;

    Initialize(S);
    printf("请依次输入并查集中各元素的上级下标（输入-1以停止）：\n");
    for (int j = 1; j < NumSets+1; j++)
    {
        printf("请输入第%d个元素的上级下标：", j);
        scanf_s("%d", &S[j]);
    }
    while (1)
    {
        printf("请输入接下来的操作:\n");
        printf("1. 求并集\n");
        printf("2. 求root\n");
        printf("3. 退出\n");
        scanf_s("%d", &s);
        switch (s)
        {
        case 1:
            printf("请输入要合并的集合中元素的关键字1：");
            scanf_s("%d", &X1);
            printf("请输入要合并的集合中元素的关键字2：");
            scanf_s("%d", &X2);
            root1 = Find(X1, S);
            printf("X1的根：%d\n", root1);
            root2 = Find(X2, S);
            printf("X2的根：%d\n", root2);
            SetUnion(S, root1, root2);
            root1 = Find(X1, S);
            printf("合并后X1的根：%d\n", root1);
            root2 = Find(X2, S);
            printf("合并后X2的根：%d\n", root2);
        case 2:
            printf("请输入要查找root的下标：");
            scanf_s("%d", &temp);
            temp = Find(temp, S);
            printf("下标为%d\n", temp);
            break;
        case 3:
            exit(-1);
        default:
            break;
        }

    }

    return 0;
}