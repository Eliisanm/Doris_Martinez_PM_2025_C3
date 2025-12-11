#include <stdio.h>
#include <stdlib.h>

struct Nodo {
    int dato;
    struct Nodo *izquierda, *derecha;
};

// Crear nuevo nodo
struct Nodo* nuevoNodo(int item) {
    struct Nodo* temp = (struct Nodo*)malloc(sizeof(struct Nodo));
    temp->dato = item;
    temp->izquierda = temp->derecha = NULL;
    return temp;
}

// Insertar nodo
struct Nodo* insertar(struct Nodo* nodo, int dato) {
    if (nodo == NULL) return nuevoNodo(dato);
    if (dato < nodo->dato)
        nodo->izquierda = insertar(nodo->izquierda, dato);
    else if (dato > nodo->dato)
        nodo->derecha = insertar(nodo->derecha, dato);
    return nodo;
}

// Recorrido en orden (Inorder)
void inOrden(struct Nodo* raiz) {
    if (raiz != NULL) {
        inOrden(raiz->izquierda);
        printf("%d ", raiz->dato);
        inOrden(raiz->derecha);
    }
}

int main() {
    struct Nodo* raiz = NULL;
    raiz = insertar(raiz, 50);
    insertar(raiz, 30);
    insertar(raiz, 20);
    insertar(raiz, 70);

    printf("--- Arbol Binario (InOrden) ---\n");
    inOrden(raiz);
    return 0;
}
