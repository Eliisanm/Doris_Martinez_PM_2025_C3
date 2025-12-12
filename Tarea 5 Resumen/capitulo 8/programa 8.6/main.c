<<<<<<< HEAD
#include <stdio.h>
#include <string.h>

typedef struct
{
    char mat[20];
    int pro;
} matpro;

typedef struct
{
  int matri;
  char nom[20];
  matpro cal[5];

}alumno;

void Lectura(alumno * , int);
void F1(alumno *, int);
void F2(alumno *, int);
void F3(alumno *, int);

void main(void)
{
    alumno ALU[50];
    int TAM;
    do
    {
        printf("Ingrese el tamano del arreglo: ");
        scanf("%d", &TAM);
    }
    while (TAM > 50 || TAM < 1);

    Lectura(ALU, TAM);
    F1(ALU, TAM);
    F2(ALU, TAM);
    F3 (ALU, TAM);
}

void Lectura(alumno A[], int T)
{
   int I, J;
   for(I=0; I<T; I++)
   {
       printf("nIngrese los datos del alumno %d", I+1);
       printf("\nIngrese la matricula del alumno: ");
       scanf("%d", &A[I].matri);
       fflush(stdin);
       printf("Ingrese el nombre del alumno:");
       gets(A[I].nom);
       for (J=0; J<5; J++)
       {
           printf("\tMateria %d: ", J+1);
           fflush(stdin);
           gets(A[I].cal[J].mat);
           printf("\tPromedio %d:", J+1);
           scanf("%d", &A[I].cal[J].pro);
       }
   }
}
void F1 (alumno A[], int T)
{
    int
}
=======
#include <stdio.h>
#include <string.h>

typedef struct
{
    char mat[20];
    int pro;
} matpro;

typedef struct
{
  int matri;
  char nom[20];
  matpro cal[5];

}alumno;

void Lectura(alumno * , int);
void F1(alumno *, int);
void F2(alumno *, int);
void F3(alumno *, int);

void main(void)
{
    alumno ALU[50];
    int TAM;
    do
    {
        printf("Ingrese el tamano del arreglo: ");
        scanf("%d", &TAM);
    }
    while (TAM > 50 || TAM < 1);

    Lectura(ALU, TAM);
    F1(ALU, TAM);
    F2(ALU, TAM);
    F3 (ALU, TAM);
}

void Lectura(alumno A[], int T)
{
   int I, J;
   for(I=0; I<T; I++)
   {
       printf("nIngrese los datos del alumno %d", I+1);
       printf("\nIngrese la matricula del alumno: ");
       scanf("%d", &A[I].matri);
       fflush(stdin);
       printf("Ingrese el nombre del alumno:");
       gets(A[I].nom);
       for (J=0; J<5; J++)
       {
           printf("\tMateria %d: ", J+1);
           fflush(stdin);
           gets(A[I].cal[J].mat);
           printf("\tPromedio %d:", J+1);
           scanf("%d", &A[I].cal[J].pro);
       }
   }
}
void F1 (alumno A[], int T)
{
    int
}
>>>>>>> 1259009b51d67e398db8ec5b84725807b55a7418
