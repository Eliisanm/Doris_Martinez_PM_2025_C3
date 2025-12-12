#include <stdio.h>

/* Conflicto de cariables con el mismo nombre. */

void f1 (void);          /*prototipo de funcion. */
int K = 5;               /* variable global. */

void main (void)
{
    int I;
    for (I= 1; I <= 3; I++)
        f1();
}
void f1 (void)
/*La funcion utiliza tanto la variable local I como la variable global I. */
{
    int K = 2;   /* variable local. */
    K += K;
    printf("\n\nEl valor de la variable local es: %d", K);
::K = ::K + K;            /* Uso de ambas variables. */
    printf("\nEl valor de la variable global es: %d", ::K);
}
