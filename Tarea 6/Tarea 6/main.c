

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definición de la estructura del producto
typedef struct {
    int id;
    char nombre[50];
    float precio;
    int cantidad;
} Producto;

// Nombre del archivo de inventario
#define NOMBRE_ARCHIVO "inventario.txt"

// --- 1. Dividir el problema en varias funciones ---

/**
 * @brief Función para leer el archivo de inventario y cargar los productos.
 * @param inventario Puntero al puntero de la estructura Producto (para asignación dinámica).
 * @param num_productos Puntero al contador de productos.
 */
void leer_inventario(Producto **inventario, int *num_productos) {
    FILE *archivo;
    Producto temp_producto;
    int capacidad_actual = 5; // Capacidad inicial para el array dinámico

    // Intentar abrir el archivo en modo lectura
    archivo = fopen(NOMBRE_ARCHIVO, "r");

    if (archivo == NULL) {
        printf("\nEl archivo '%s' no existe o está vacío. Se creará uno nuevo al guardar.\n", NOMBRE_ARCHIVO);
        // Asignar memoria inicial incluso si el archivo no existe
        *inventario = (Producto *)malloc(capacidad_actual * sizeof(Producto));
        if (*inventario == NULL) {
            perror("Error al asignar memoria inicial");
            exit(EXIT_FAILURE);
        }
        *num_productos = 0;
        return;
    }

    // 4. Usar memoria dinámica para cargar el inventario de archivo
    *num_productos = 0;
    *inventario = NULL; // Inicializar puntero a NULL

    while (fscanf(archivo, "%d;%49[^;];%f;%d\n",
                  &temp_producto.id,
                  temp_producto.nombre,
                  &temp_producto.precio,
                  &temp_producto.cantidad) == 4) {

        // Si la capacidad es superada, se reasigna memoria
        if (*num_productos >= capacidad_actual) {
            capacidad_actual *= 2;
            Producto *temp = (Producto *)realloc(*inventario, capacidad_actual * sizeof(Producto));
            if (temp == NULL) {
                perror("Error al reasignar memoria (realloc)");
                fclose(archivo);
                free(*inventario);
                exit(EXIT_FAILURE);
            }
            *inventario = temp;
        }

        // Copiar el producto a la memoria dinámica
        (*inventario)[*num_productos] = temp_producto;
        (*num_productos)++;
    }

    fclose(archivo);
    printf("\nInventario cargado exitosamente. Total de productos: %d\n", *num_productos);
}


/**
 * @brief Función para escribir un nuevo producto en el inventario (memoria).
 * @param inventario Puntero al puntero de la estructura Producto (para realloc).
 * @param num_productos Puntero al contador de productos.
 */
void escribir_nuevo_producto(Producto **inventario, int *num_productos) {
    char respuesta;
    printf("\n--- AGREGAR NUEVO PRODUCTO ---\n");
    printf("¿Desea escribir un producto nuevo en el inventario (S/N)? ");
    scanf(" %c", &respuesta);

    if (respuesta == 'S' || respuesta == 's') {
        Producto nuevo;

        printf("ID del producto (entero): ");
        scanf("%d", &nuevo.id);

        printf("Nombre del producto (sin espacios): ");
        scanf("%49s", nuevo.nombre);

        printf("Precio del producto (float): ");
        scanf("%f", &nuevo.precio);

        printf("Cantidad en stock (entero): ");
        scanf("%d", &nuevo.cantidad);

        // Aumentar la capacidad del array dinámico en 1
        *inventario = (Producto *)realloc(*inventario, (*num_productos + 1) * sizeof(Producto));
        if (*inventario == NULL) {
            perror("Error al reasignar memoria para el nuevo producto");
            exit(EXIT_FAILURE);
        }

        // Agregar el nuevo producto al final
        (*inventario)[*num_productos] = nuevo;
        (*num_productos)++;

        printf("\nProducto agregado a la lista en memoria.\n");

    } else {
        printf("\nNo se agregaron nuevos productos.\n");
    }
}

/**
 * @brief Función para imprimir los productos en el inventario.
 * @param inventario Puntero al array de productos.
 * @param num_productos Contador de productos.
 */
void imprimir_inventario(Producto *inventario, int num_productos) {
    char respuesta;

    printf("\n--- IMPRIMIR INVENTARIO ---\n");
    printf("¿Desea imprimir los productos en inventario (S/N)? ");
    scanf(" %c", &respuesta);

    if (respuesta == 'S' || respuesta == 's') {
        if (num_productos == 0) {
            printf("\nEl inventario está vacío.\n");
            return;
        }

        printf("\n%-5s %-30s %-10s %-10s\n", "ID", "NOMBRE", "PRECIO", "CANTIDAD");
        printf("-------------------------------------------------------------------\n");
        for (int i = 0; i < num_productos; i++) {
            printf("%-5d %-30s %-10.2f %-10d\n",
                   inventario[i].id,
                   inventario[i].nombre,
                   inventario[i].precio,
                   inventario[i].cantidad);
        }
    } else {
        printf("\nNo se imprimió el inventario.\n");
    }
}

/**
 * @brief Función para guardar todo el inventario de memoria al archivo.
 * @param inventario Puntero al array de productos.
 * @param num_productos Contador de productos.
 */
void guardar_inventario(Producto *inventario, int num_productos) {
    FILE *archivo = fopen(NOMBRE_ARCHIVO, "w"); // Modo 'w' sobreescribe el archivo

    if (archivo == NULL) {
        perror("Error al abrir el archivo para guardar");
        return;
    }

    for (int i = 0; i < num_productos; i++) {
        // Formato: ID;NOMBRE;PRECIO;CANTIDAD\n
        fprintf(archivo, "%d;%s;%.2f;%d\n",
                inventario[i].id,
                inventario[i].nombre,
                inventario[i].precio,
                inventario[i].cantidad);
    }

    fclose(archivo);
    printf("\nInventario guardado en '%s'.\n", NOMBRE_ARCHIVO);
}

// --- Función Principal ---
int main() {
    Producto *inventario = NULL; // Puntero al inventario (inicialmente NULL)
    int num_productos = 0;

    printf("--- GESTOR DE INVENTARIO C ---\n");

    // 2. Leer el archivo de inventario
    // 4. Usar memoria dinámica (implementado dentro de la función)
    leer_inventario(&inventario, &num_productos);

    // 3. Pregunte si se quiere escribir un producto nuevo
    escribir_nuevo_producto(&inventario, &num_productos);

    // 5. Pregunta si quiere imprimir los productos en inventario
    imprimir_inventario(inventario, num_productos);

    // Guardar todos los cambios (cargados y nuevos) en el archivo
    guardar_inventario(inventario, num_productos);

    // Liberar la memoria dinámica
    free(inventario);
    inventario = NULL;

    printf("\nPrograma finalizado.\n");
    return 0;
}
