#include <stdio.h>

 /* incremento salario.
 El programa al recibir como dato el nivel de un profesor, incrementa su salario en funcion de la tabla 2.3.

 NIV: variable de tipo entero.
 SAL: variable de tipo real. */

void main(void)
{
    float SAL;
    int NIV;
    printf("Ingrese el nivel académico del profesor: ");
    scanf("%d", &NIV);
    printf("Ingrese salario: ");
    scanf("%f", &SAL);
    switch(NIV)
    {
        case 1: SAL = SAL * 1.0035;  break;
        case 2: SAL = SAL * 1.0041; break;
        case 3:  SAL = SAL * 1.0040;  break;
        case 4: SAL = SAL * 1.0053;  break;
    }

    printf("\n\nNivel: %d \tNuevo salario: %8.2f", NIV, SAL);
}
