#include <stdio.h>


/*rango de calificaciones.
El programa, al recibir como dato una serie de calificaciones, obtiene el rango en el que se encuentran. */

void rango (int);   /* prototipo de funcion. */

int RA1 = 0;
int RA2 = 0;
int RA3 = 0;        /* variables globales de tipo entero. */
int RA4 = 0;
int RA5 = 0;
/* El uso de variables globales no es muy recomendable. En estos problemas se utilizan unicamente con el objetivo de que el lector pueda observar la forma en que se aplican. */

void main (void)
{
    float CAl;
    printf("Ingreasa la primera calificacion del alumno:");
    scanf("%f", &CAL);
    while (CAL != -1)
    {
     Rango (CAL);  /*Llamada a la funcion rango. Se pasa un parametro por valor. */
     printf("Ingresa la siguiente calificacion del alumno: ");
     scanf("%f", &CAL);
    }
    printf("\n0..3.99 = %d", RA1);
    printf("\n4..5.99 = %d", RA2);
    printf("\n6..7.99 = %d", RA3);
    printf("\n8..8.99 = %d", RA4);
    printf("\n9..10.99 = %d", RA5);
}
void Rango (int VAL)
/*La funcion incrementa una variable dependiendo del valor del parametro VAL. */
{
    if (VAL < 4)
        RA1++;
    else
        if (VAL <6)
        RA2++;
    if (VAL <8)
        RA3++;
        if (VAL <9)
        RA4++;
    else
        RA5++;
}
