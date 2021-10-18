#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE *f = fopen("t.txt", "r");

    if (feof(f))
        printf("kk");

}
