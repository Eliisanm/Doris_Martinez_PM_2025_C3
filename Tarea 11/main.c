#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Definicion de la estructura del Libro
typedef struct Libro
 {
    int codigo;
    int ano;
    char autor[100];
    char titulo[100];
    char descripcion[250];
    float precio;
    struct Libro *siguiente;
} Libro;

// Puntero global a la cabeza de la lista enlazada
Libro *listaLibros = NULL;
const char *NOMBRE_ARCHIVO = "libros.dat";

// --- Funciones de Utilidad y Gestión de Memoria ---

void liberarMemoria()
{
    Libro *actual = listaLibros;
    Libro *siguiente = NULL;
    while (actual != NULL)
        {
        siguiente = actual->siguiente;
        free(actual);
        actual = siguiente;
    }
    listaLibros = NULL;
}

Libro* crearLibro(int codigo, int ano, const char *autor, const char *titulo, const char *descripcion, float precio)
 {
    Libro *nuevoLibro = (Libro *)malloc(sizeof(Libro));
    if (nuevoLibro == NULL)
        {
        perror("Error al asignar memoria para un nuevo libro");
        return NULL;
    }

    nuevoLibro->codigo = codigo;
    nuevoLibro->ano = ano;
    strncpy(nuevoLibro->autor, autor, sizeof(nuevoLibro->autor) - 1);
    nuevoLibro->autor[sizeof(nuevoLibro->autor) - 1] = '\0';
    strncpy(nuevoLibro->titulo, titulo, sizeof(nuevoLibro->titulo) - 1);
    nuevoLibro->titulo[sizeof(nuevoLibro->titulo) - 1] = '\0';
    strncpy(nuevoLibro->descripcion, descripcion, sizeof(nuevoLibro->descripcion) - 1);
    nuevoLibro->descripcion[sizeof(nuevoLibro->descripcion) - 1] = '\0';
    nuevoLibro->precio = precio;
    nuevoLibro->siguiente = NULL;

    return nuevoLibro;
}

void agregarLibro(Libro *nuevoLibro)
 {
    if (nuevoLibro == NULL) return;

    if (listaLibros == NULL)
        {
        listaLibros = nuevoLibro;
    } else {
        Libro *actual = listaLibros;
        while (actual->siguiente != NULL)
         {
            actual = actual->siguiente;
        }
        actual->siguiente = nuevoLibro;
    }
    printf("\n Libro '%s' agregado a la lista.\n", nuevoLibro->titulo);
}

// --- Funciones de Persistencia (Guardar/Cargar) ---

void guardarLista()
 {
    FILE *archivo = fopen(NOMBRE_ARCHIVO, "wb");
    if (archivo == NULL)
        {
        perror("Error al abrir el archivo para escritura");
        return;
    }

    Libro *actual = listaLibros;
    int count = 0;

    size_t sizeWithoutPointer = sizeof(Libro) - sizeof(Libro*);

    while (actual != NULL) {
        fwrite(actual, sizeWithoutPointer, 1, archivo);
        actual = actual->siguiente;
        count++;
    }

    fclose(archivo);
    printf("\n Lista de %d libros guardada en '%s'.\n", count, NOMBRE_ARCHIVO);
}

void cargarLista() {
    liberarMemoria();

    FILE *archivo = fopen(NOMBRE_ARCHIVO, "rb");
    if (archivo == NULL) {
        printf("Advertencia: No se encontró el archivo de libros ('%s'), iniciando lista vacía.\n", NOMBRE_ARCHIVO);
        return;
    }

    int count = 0;
    Libro tempLibro;

    size_t sizeWithoutPointer = sizeof(Libro) - sizeof(Libro*);

    while (fread(&tempLibro, sizeWithoutPointer, 1, archivo) == 1)
        {
        Libro *nuevoLibro = crearLibro(tempLibro.codigo, tempLibro.ano, tempLibro.autor, tempLibro.titulo, tempLibro.descripcion, tempLibro.precio);

        if (nuevoLibro != NULL)
            {
            if (listaLibros == NULL)
             {
                listaLibros = nuevoLibro;
            } else
            {
                Libro *actual = listaLibros;
                while (actual->siguiente != NULL)
                 {
                    actual = actual->siguiente;
                }
                actual->siguiente = nuevoLibro;
            }
            count++;
        }
    }

    fclose(archivo);
    printf("\n Lista de %d libros cargada desde '%s'.\n", count, NOMBRE_ARCHIVO);
}


// --- Funciones de Búsqueda e Impresión (Requisito 3) ---

void mostrarLibro(Libro *libro) {
    printf("  ----------------------------------\n");
    printf("  Codigo: %d\n", libro->codigo);
    printf("  Titulo: %s\n", libro->titulo);
    printf("  Autor: %s\n", libro->autor);
    printf("  Ano: %d\n", libro->ano);
    printf("  Descripcion: %s\n", libro->descripcion);
    printf("  Precio: $%.2f\n", libro->precio);
}

// Implementación de Requisito 3
void imprimirLista()
{
    printf("\n--- Lista Completa de Libros ---\n");
    if (listaLibros == NULL)
        {
        printf("La lista de libros esta vacia.\n");
    } else
    {
        Libro *actual = listaLibros;
        int i = 1;
        while (actual != NULL)
            {
            printf("\n--- Libro %d ---", i++);
            mostrarLibro(actual);
            actual = actual->siguiente;
        }
        printf("\n----------------------------------\n");
    }
}

// La búsqueda ya incluye por Título y Autor (Requisito 1)
void buscarLibro(const char *terminoBusqueda) {
    if (listaLibros == NULL)
        {
        printf("\nLista vacía. No hay libros para buscar.\n");
        return;
    }

    printf("\n Buscando libros que contengan '%s'...\n", terminoBusqueda);
    Libro *actual = listaLibros;
    int encontrados = 0;

    char *termino_lower = strdup(terminoBusqueda);
    if (termino_lower == NULL) return;
    for(char *p = termino_lower; *p; p++)
        {
        *p = tolower((unsigned char)*p);
    }


    while (actual != NULL)
        {
        char *titulo_lower = strdup(actual->titulo);
        char *autor_lower = strdup(actual->autor);

        if (titulo_lower == NULL || autor_lower == NULL)
            {
            free(titulo_lower);
            free(autor_lower);
            free(termino_lower);
            return;
        }

        for(char *p = titulo_lower; *p; p++) *p = tolower((unsigned char)*p);
        for(char *p = autor_lower; *p; p++) *p = tolower((unsigned char)*p);

        // La búsqueda verifica tanto el Título como el Autor
        if (strstr(titulo_lower, termino_lower) != NULL || strstr(autor_lower, termino_lower) != NULL)
            {
            mostrarLibro(actual);
            encontrados++;
        }

        free(titulo_lower);
        free(autor_lower);

        actual = actual->siguiente;
    }

    free(termino_lower);

    if (encontrados == 0)
        {
        printf("\n No se encontraron libros con el término de búsqueda.\n");
    } else {
        printf("\n Se encontraron %d resultados.\n", encontrados);
    }
}


// --- Función de Ordenamiento (Requisito 2) ---

/**
 * @brief Implementa el algoritmo Bubble Sort para ordenar la lista enlazada por precio (ascendente).
 * 2. Agregar la función de ordenar con el algoritmo BubbleSort la lista por precio
 */
void ordenarListaBubbleSort()
 {
    if (listaLibros == NULL || listaLibros->siguiente == NULL) {
        printf("\nLa lista está vacía o solo tiene un elemento, no se requiere ordenamiento.\n");
        return;
    }

    int swapp;
    Libro *ptr1;
    Libro *lptr = NULL; // Puntero al último nodo ya ordenado

    printf("\n Ordenando lista por precio con Bubble Sort...\n");

    do {
        swapp = 0;
        ptr1 = listaLibros;

        while (ptr1->siguiente != lptr)
            {
            // Comparar por precio
            if (ptr1->precio > ptr1->siguiente->precio)
             {
                // Intercambiar los datos de las estructuras (no los nodos)

                // Paso 1: Crear una estructura temporal para guardar los datos de ptr1
                Libro temp = *ptr1;

                // Paso 2: Copiar los datos de ptr1->siguiente a ptr1
                *ptr1 = *ptr1->siguiente;

                // Paso 3: Copiar los datos temporales (originales de ptr1) a ptr1->siguiente
                *ptr1->siguiente = temp;

                // Paso 4: Restaurar los punteros 'siguiente' originales
                // (Esto es crucial porque no queremos cambiar el orden de los nodos, solo sus contenidos)
                Libro *tempSiguiente = ptr1->siguiente->siguiente;
                ptr1->siguiente->siguiente = ptr1->siguiente;
                ptr1->siguiente = tempSiguiente;

                // Intercambiar los punteros siguientes y restaurarlos.
                // Un enfoque más sencillo y limpio para listas enlazadas es solo intercambiar los campos de datos:

                /*
                // Intercambio de campos de datos (más simple)
                int t_codigo = ptr1->codigo; ptr1->codigo = ptr1->siguiente->codigo; ptr1->siguiente->codigo = t_codigo;
                int t_ano = ptr1->ano; ptr1->ano = ptr1->siguiente->ano; ptr1->siguiente->ano = t_ano;
                char t_autor[100]; strcpy(t_autor, ptr1->autor); strcpy(ptr1->autor, ptr1->siguiente->autor); strcpy(ptr1->siguiente->autor, t_autor);
                char t_titulo[100]; strcpy(t_titulo, ptr1->titulo); strcpy(ptr1->titulo, ptr1->siguiente->titulo); strcpy(ptr1->siguiente->titulo, t_titulo);
                char t_desc[250]; strcpy(t_desc, ptr1->descripcion); strcpy(ptr1->descripcion, ptr1->siguiente->descripcion); strcpy(ptr1->siguiente->descripcion, t_desc);
                float t_precio = ptr1->precio; ptr1->precio = ptr1->siguiente->precio; ptr1->siguiente->precio = t_precio;
                */

                // Para evitar errores al intercambiar nodos completos en una lista simple,
                // utilizaremos la técnica de intercambiar los campos de datos (código comentado arriba)
                // o reescribir el Bubble Sort para manejar punteros.

                // Por simplicidad y evitar errores con punteros complejos: Usaremos la técnica de intercambio de campos

                int t_codigo = ptr1->codigo; ptr1->codigo = ptr1->siguiente->codigo; ptr1->siguiente->codigo = t_codigo;
                int t_ano = ptr1->ano; ptr1->ano = ptr1->siguiente->ano; ptr1->siguiente->ano = t_ano;
                char t_autor[100]; strcpy(t_autor, ptr1->autor); strcpy(ptr1->autor, ptr1->siguiente->autor); strcpy(ptr1->siguiente->autor, t_autor);
                char t_titulo[100]; strcpy(t_titulo, ptr1->titulo); strcpy(ptr1->titulo, ptr1->siguiente->titulo); strcpy(ptr1->siguiente->titulo, t_titulo);
                char t_desc[250]; strcpy(t_desc, ptr1->descripcion); strcpy(ptr1->descripcion, ptr1->siguiente->descripcion); strcpy(ptr1->siguiente->descripcion, t_desc);
                float t_precio = ptr1->precio; ptr1->precio = ptr1->siguiente->precio; ptr1->siguiente->precio = t_precio;

                swapp = 1;
            }
            ptr1 = ptr1->siguiente;
        }
        lptr = ptr1;
    } while (swapp);

    printf("\n Lista ordenada por precio de menor a mayor.\n");
}


// --- Función Principal y Menú ---

void esperarEnter()
{
    printf("\nPresiona ENTER para continuar...");
    while (getchar() != '\n');
    getchar();
}

int main() {
    cargarLista();

    int opcion;
    int codigo = 0;

    // Inicializar el código consecutivo
    if (listaLibros != NULL)
        {
        Libro *actual = listaLibros;
        while (actual != NULL)
        {
            if (actual->codigo > codigo)
             {
                codigo = actual->codigo;
            }
            actual = actual->siguiente;
        }
        codigo++;
    } else {
        codigo = 1;
    }


    do {
        printf("\n\n=== GESTOR DE LIBRERIA ===\n");
        printf("1. Agregar Nuevo Libro\n");
        printf("2. Guardar Lista en Disco\n");
        printf("3. Cargar Lista desde Disco\n");
        printf("4. Buscar Libro por Titulo o Autor\n");
        printf("5. Ordenar Lista por Precio (Bubble Sort)\n"); // Nuevo Requisito 2
        printf("6. Imprimir Lista de Libros\n");               // Nuevo Requisito 3
        printf("7. Salir (y Guardar)\n");
        printf("Selecciona una opcion: ");

        if (scanf("%d", &opcion) != 1)
            {
            printf("\nOpcion invalida. Intenta de nuevo.\n");
            while (getchar() != '\n');
            opcion = 0;
            continue;
        }
        while (getchar() != '\n');

        char autor[100], titulo[100], descripcion[250], terminoBusqueda[100];
        int ano;
        float precio;

        switch (opcion)
         {
            case 1:
                printf("\n--- Agregar Nuevo Libro ---\n");
                printf("Titulo: ");
                fgets(titulo, sizeof(titulo), stdin);
                titulo[strcspn(titulo, "\n")] = 0;

                printf("Autor: ");
                fgets(autor, sizeof(autor), stdin);
                autor[strcspn(autor, "\n")] = 0;

                printf("Ano: ");
                if (scanf("%d", &ano) != 1)
                    {
                    printf("Entrada de ano invalida. Cancelando adicion.\n");
                    while (getchar() != '\n');
                    break;
                }
                while (getchar() != '\n');

                printf("Descripcion: ");
                fgets(descripcion, sizeof(descripcion), stdin);
                descripcion[strcspn(descripcion, "\n")] = 0;

                printf("Precio: ");
                if (scanf("%f", &precio) != 1)
                    {
                    printf("Entrada de precio invalida. Cancelando adicion.\n");
                    while (getchar() != '\n');
                    break;
                }
                while (getchar() != '\n');

                Libro *nuevo = crearLibro(codigo++, ano, autor, titulo, descripcion, precio);
                agregarLibro(nuevo);
                esperarEnter();
                break;

            case 2:
                guardarLista();
                esperarEnter();
                break;

            case 3:
                cargarLista();
                esperarEnter();
                break;

            case 4:
                printf("\n--- Buscar Libro ---\n");
                printf("Ingresa Titulo o Autor a buscar: ");
                fgets(terminoBusqueda, sizeof(terminoBusqueda), stdin);
                terminoBusqueda[strcspn(terminoBusqueda, "\n")] = 0;
                buscarLibro(terminoBusqueda);
                esperarEnter();
                break;

            case 5: // Ordenar por Bubble Sort (Nuevo Requisito 2)
                ordenarListaBubbleSort();
                esperarEnter();
                break;

            case 6: // Imprimir Lista (Nuevo Requisito 3)
                imprimirLista();
                esperarEnter();
                break;

            case 7:
                guardarLista();
                liberarMemoria();
                printf("\nPrograma finalizado. ¡Hasta pronto!\n");
                break;

            default:
                printf("\nOpcion no valida. Intenta de nuevo.\n");
                esperarEnter();
                break;
        }
    } while (opcion != 7);

    return 0;
}
