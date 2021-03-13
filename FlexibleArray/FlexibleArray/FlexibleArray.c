#include <stdio.h>
#include <stdlib.h>

struct FlexibleArray
{
    int Length;
    int Array[0];
};

int main()
{
    struct FlexibleArray* A;

    if ((A = (struct FlexibleArray*)malloc(sizeof(struct FlexibleArray) + sizeof(int) * 10)) != NULL)
    {
        A->Length = 10;
        printf("A->Length = %d\n", A->Length);
        for (int i = 0; i < A->Length; i++)
        {
            A->Array[i] = i;
            printf("A->Array[%d] = %d\n", i, A->Array[i]);
        }
        printf("\n");
        free(A);
    }

    return 0;
}