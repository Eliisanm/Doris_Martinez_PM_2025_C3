#include <stdio.h>

#define V 4 // Número de vértices

// Inicializar matriz
void iniciarGrafo(int arr[][V]) {
    for (int i = 0; i < V; i++)
        for (int j = 0; j < V; j++)
            arr[i][j] = 0;
}

// Agregar arista (conexión)
void agregarArista(int arr[][V], int i, int j) {
    arr[i][j] = 1;
    arr[j][i] = 1; // Grafo no dirigido
}

void imprimirGrafo(int arr[][V]) {
    printf("Matriz de Adyacencia:\n");
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            printf("%d ", arr[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int adjMatrix[V][V];
    printf("--- Grafo ---\n");
    iniciarGrafo(adjMatrix);
    agregarArista(adjMatrix, 0, 1);
    agregarArista(adjMatrix, 0, 2);
    agregarArista(adjMatrix, 1, 2);
    imprimirGrafo(adjMatrix);
    return 0;
}
