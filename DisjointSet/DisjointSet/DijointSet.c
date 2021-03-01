#include <stdio.h>
#include <stdlib.h>
#include "DisjointSet.h"

#define Error(str) FatalError(str)
#define FatalError(str) fprintf(stderr, "%s\n", str), exit(1)

//��ʼ��
void Initialize(DisjointSet S)
{
	int i;

	for (i = NumSets + 1; i > 0; i--)
		S[i] = 0;
}

//���ϵĲ�
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

//����Ԫ��X���ڼ��ϵĸ�
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
    printf("���������벢�鼯�и�Ԫ�ص��ϼ��±꣨����-1��ֹͣ����\n");
    for (int j = 1; j < NumSets+1; j++)
    {
        printf("�������%d��Ԫ�ص��ϼ��±꣺", j);
        scanf_s("%d", &S[j]);
    }
    while (1)
    {
        printf("������������Ĳ���:\n");
        printf("1. �󲢼�\n");
        printf("2. ��root\n");
        printf("3. �˳�\n");
        scanf_s("%d", &s);
        switch (s)
        {
        case 1:
            printf("������Ҫ�ϲ��ļ�����Ԫ�صĹؼ���1��");
            scanf_s("%d", &X1);
            printf("������Ҫ�ϲ��ļ�����Ԫ�صĹؼ���2��");
            scanf_s("%d", &X2);
            root1 = Find(X1, S);
            printf("X1�ĸ���%d\n", root1);
            root2 = Find(X2, S);
            printf("X2�ĸ���%d\n", root2);
            SetUnion(S, root1, root2);
            root1 = Find(X1, S);
            printf("�ϲ���X1�ĸ���%d\n", root1);
            root2 = Find(X2, S);
            printf("�ϲ���X2�ĸ���%d\n", root2);
        case 2:
            printf("������Ҫ����root���±꣺");
            scanf_s("%d", &temp);
            temp = Find(temp, S);
            printf("�±�Ϊ%d\n", temp);
            break;
        case 3:
            exit(-1);
        default:
            break;
        }

    }

    return 0;
}