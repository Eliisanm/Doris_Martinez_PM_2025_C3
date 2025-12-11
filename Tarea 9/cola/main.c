#include <stdio.h>
#define MAX 5

int cola[MAX];
int frente = -1, final = -1;

void encolar(int valor) {
    if (final == MAX - 1)
        printf("Cola llena\n");
    else {
        if (frente == -1) frente = 0;
        final++;
        cola[final] = valor;
        printf("%d encolado\n", valor);
    }
}

void desencolar() {
    if (frente == -1)
        printf("Cola vacia\n");
    else {
        printf("%d desencolado\n", cola[frente]);
        frente++;
        if (frente > final) frente = final = -1;
    }
}

int main() {
    printf("--- Cola (Queue) ---\n");
    encolar(100);
    encolar(200);
    desencolar(); // Sale el 100
    encolar(300);
    return 0;
}
