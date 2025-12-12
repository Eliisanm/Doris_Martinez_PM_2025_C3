#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Definición de la estructura del Libro
typedef struct Libro
{
    int codigo;
    int ano;
    char autor[100];
    char titulo[100];
    char descripcion[250];
    float precio;
    struct Libro *siguiente; // Puntero al siguiente nodo
} Libro;

// Puntero global a la cabeza de la lista enlazada
Libro *listaLibros = NULL;
const char *NOMBRE_ARCHIVO = "libros.dat";

// --- Funciones de Utilidad ---

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

Libro* crearLibro(int codigo, int ano, const char *autor, const char *titulo, const char *descripcion, float precio) {
    // Uso de malloc() para memoria dinámica
    Libro *nuevoLibro = (Libro *)malloc(sizeof(Libro));
    if (nuevoLibro == NULL) {
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

    // El tamaño a escribir excluye el puntero 'siguiente'
    size_t sizeWithoutPointer = sizeof(Libro) - sizeof(Libro*);

    while (actual != NULL)
        {
        fwrite(actual, sizeWithoutPointer, 1, archivo);
        actual = actual->siguiente;
        count++;
    }

    fclose(archivo);
    printf("\n Lista de %d libros guardada en '%s'.\n", count, NOMBRE_ARCHIVO);
}

void cargarLista()
 {
    liberarMemoria();

    FILE *archivo = fopen(NOMBRE_ARCHIVO, "rb");
    if (archivo == NULL)
        {
        printf("Advertencia: No se encontro el archivo de libros ('%s'), iniciando lista vacia.\n", NOMBRE_ARCHIVO);
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
            } else {
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

void mostrarLibro(Libro *libro) {
    printf("  ----------------------------------\n");
    printf("  Codigo: %d\n", libro->codigo);
    printf("  Titulo: %s\n", libro->titulo);
    printf("  Autor: %s\n", libro->autor);
    printf("  Ano: %d\n", libro->ano);
    printf("  Descripcion: %s\n", libro->descripcion);
    printf("  Precio: %.2f\n", libro->precio);
}

void buscarLibro(const char *terminoBusqueda) {
    if (listaLibros == NULL) {
        printf("\nLista vacia. No hay libros para buscar.\n");
        return;
    }

    printf("\n Buscando libros que contengan '%s'...\n", terminoBusqueda);
    Libro *actual = listaLibros;
    int encontrados = 0;

    // Crear una versión en minúsculas del término de búsqueda
    char *termino_lower = strdup(terminoBusqueda);
    if (termino_lower == NULL) return;
    for(char *p = termino_lower; *p; p++)
        {
        *p = tolower((unsigned char)*p);
    }


    while (actual != NULL) {
        // Crear versiones en minúsculas de Título y Autor
        char *titulo_lower = strdup(actual->titulo);
        char *autor_lower = strdup(actual->autor);

        if (titulo_lower == NULL || autor_lower == NULL)
            {
            free(titulo_lower);
            free(autor_lower);
            free(termino_lower);
            return;
        }

        // Convertir a minúsculas
        for(char *p = titulo_lower; *p; p++) *p = tolower((unsigned char)*p);
        for(char *p = autor_lower; *p; p++) *p = tolower((unsigned char)*p);

        // strstr busca la subcadena en Título o Autor
        if (strstr(titulo_lower, termino_lower) != NULL || strstr(autor_lower, termino_lower) != NULL) {
            mostrarLibro(actual);
            encontrados++;
        }

        // Liberar la memoria temporal
        free(titulo_lower);
        free(autor_lower);

        actual = actual->siguiente;
    }

    free(termino_lower);

    if (encontrados == 0)
        {
        printf("\n No se encontraron libros con el termino de busqueda.\n");
    } else {
        printf("\n Se encontraron %d resultados.\n", encontrados);
    }
}

// --- Función Principal y Menú ---

void esperarEnter() {
    printf("\nPresiona ENTER para continuar...");
    while (getchar() != '\n');
    getchar();
}

int main()
 {
    cargarLista();

    int opcion;
    int codigo = 0;

    // Inicializar el código consecutivo
    if (listaLibros != NULL) {
        Libro *actual = listaLibros;
        while (actual != NULL) {
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
        printf("5. Mostrar Todos los Libros\n");
        printf("6. Salir (y Guardar)\n");
        printf("Selecciona una opcion: ");

        if (scanf("%d", &opcion) != 1)
            {
            printf("\nOpción invalida. Intenta de nuevo.\n");
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

                printf("Descripción: ");
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

            case 5:
                printf("\n--- Lista Completa de Libros ---\n");
                if (listaLibros == NULL)
                    {
                    printf("La lista de libros esta vacia.\n");
                } else {
                    Libro *actual = listaLibros;
                    while (actual != NULL)
                     {
                        mostrarLibro(actual);
                        actual = actual->siguiente;
                    }
                }
                esperarEnter();
                break;

            case 6:
                guardarLista();
                liberarMemoria();
                printf("\nPrograma finalizado. ¡Hasta pronto!\n");
                break;

            default:
                printf("\nOpcion no valida. Intenta de nuevo.\n");
                esperarEnter();
                break;
        }
    } while (opcion != 6);

    return 0;
 }
