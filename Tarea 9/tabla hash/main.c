#include <stdio.h>
#include <stdlib.h>

#define TAM 10

int tabla[TAM];

// Inicializar tabla con -1 (vacío)
void iniciar() {
    for (int i = 0; i < TAM; i++) tabla[i] = -1;
}

// Función hash simple
int hashFunction(int clave) {
    return clave % TAM;
}

void insertar(int valor) {
    int clave = hashFunction(valor);
    // Manejo simple de colisión (búsqueda lineal)
    while(tabla[clave] != -1) {
        clave = (clave + 1) % TAM;
    }
    tabla[clave] = valor;
}

void imprimir() {
    for(int i = 0; i < TAM; i++) {
        if(tabla[i] != -1)
            printf("Indice %d: %d\n", i, tabla[i]);
    }
}

int main() {
    iniciar();
    printf("--- Tabla Hash ---\n");
    insertar(10); // Indice 0
    insertar(42); // Indice 2
    insertar(4);  // Indice 4
    insertar(12); // Colisión en índice 2 -> mueve al 3
    imprimir();
    return 0;
}
