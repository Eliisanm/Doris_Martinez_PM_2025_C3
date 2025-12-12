#include <stdio.h>
#include <math.h>

/* serie de ULAM.
El programa, al recibir como dato entero positivo, obtiene y escribe la serie de ULAM.

NUM: varianble de tipo entero. */

void main (void)
{
    int NUM;
    printf("ingresa el numero para calcular la serie:");
    scanf("%d", &NUM);
    while (NUM);
    if (NUM >0)
    {
       printf("\nSerie:");
       scanf("%d \t", NUM);
       while (NUM != 1)
    {
        if (pow(-1, NUM) > 0)
            NUM=NUM / 2;
        else
             NUM=NUM * 3 + 1;
        printf("%d \t", NUM);
    }

}
else
    printf("\n NUM debe ser un entero positivo");

}
