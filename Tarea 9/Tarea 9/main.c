#include <stdio.h>
#include <stdlib.h>

// Definición del nodo
struct Nodo {
    int dato;
    struct Nodo* siguiente;
};

// Función para insertar al inicio
void insertar(struct Nodo** cabeza, int nuevo_dato) {
    struct Nodo* nuevo_nodo = (struct Nodo*) malloc(sizeof(struct Nodo));
    nuevo_nodo->dato = nuevo_dato;
    nuevo_nodo->siguiente = (*cabeza);
    (*cabeza) = nuevo_nodo;
}

// Función para imprimir la lista
void imprimirLista(struct Nodo* nodo) {
    while (nodo != NULL) {
        printf(" %d ->", nodo->dato);
        nodo = nodo->siguiente;
    }
    printf(" NULL\n");
}

int main() {
    struct Nodo* cabeza = NULL;
    printf("--- Lista Enlazada ---\n");
    insertar(&cabeza, 30);
    insertar(&cabeza, 20);
    insertar(&cabeza, 10);
    imprimirLista(cabeza);
    return 0;
}
