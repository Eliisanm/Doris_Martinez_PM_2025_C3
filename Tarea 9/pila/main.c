#include <stdio.h>
#define MAX 5

int pila[MAX];
int tope = -1;

void push(int valor) {
    if (tope == MAX - 1) {
        printf("Desbordamiento de pila\n");
    } else {
        tope++;
        pila[tope] = valor;
        printf("%d insertado en la pila\n", valor);
    }
}

void pop() {
    if (tope == -1) {
        printf("Pila vacia\n");
    } else {
        printf("%d eliminado de la pila\n", pila[tope]);
        tope--;
    }
}

int main() {
    printf("--- Pila (Stack) ---\n");
    push(10);
    push(20);
    push(30);
    pop(); // Saca el 30
    return 0;
}
