#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Para tolower

// --- DEFINICIÓN DE ESTRUCTURAS ---

// 2. Propiedades vehículos
typedef struct {
    int id;   // 4. ID (Identificador único)
    char marca[30]; // 1. Marca
    char modelo[30];  // 2. Modelo
    int anio;    // 3. Anio
    char placa[10];  // 5. Placa
    float consumo_ciudad; // 6. Consumo ciudad L/100Km
    float consumo_carretera; // 7. Consumo carretera L/100Km
    float costo_seguro;  // 8. Costo seguro año
    float costo_cambio_km;  // 9. Costo cambio por Km (mantenimiento)
    int depreciacion_anios; // 10. Tiempo de depreciación años
    float costo_vehiculo; // 11. Costo Vehiculo
    float costo_limpieza;  // 14. Costo Limpieza
    // Los campos 12 y 13 (Costo de depreciación y Km promedio año) se calculan.
} Vehiculo;

// Estructura para almacenar el costo del combustible (asumimos un solo tipo por simplicidad)
typedef struct
{
    char nombre[20];
    float precio_litro;
} Combustible;


// --- VARIABLES GLOBALES DE ARCHIVO (para simplificar el ejemplo) ---
#define ARCHIVO_VEHICULOS "vehiculos.txt"
#define ARCHIVO_COMBUSTIBLE "combustible.txt"

// --- DECLARACIÓN DE FUNCIONES ---
void menu_principal();
// Funciones de Vehículo
void agregar_vehiculo(Vehiculo **v, int *n);
void lista_vehiculo(Vehiculo *v, int n);
void modificar_vehiculo(Vehiculo *v, int n);
// Funciones de Combustible
void agregar_modificar_combustible(Combustible *c);
void lista_combustible(Combustible c);
// Funciones de Viaje
void calcular_viaje(Vehiculo *v, int n, Combustible c);
// Funciones de Persistencia (Carga/Guardado)
void cargar_vehiculos(Vehiculo **v, int *n);
void guardar_vehiculos(Vehiculo *v, int n);
void cargar_combustible(Combustible *c);
void guardar_combustible(Combustible c);
// Función de Utilidad
void limpiar_buffer();

// --- FUNCIÓN PRINCIPAL ---
int main()
{
    Vehiculo *vehiculos = NULL;
    int num_vehiculos = 0;
    Combustible combustible = {"Gasolina Regular", 0.0}; // Valor inicial

    // Cargar datos al iniciar
    cargar_vehiculos(&vehiculos, &num_vehiculos);
    cargar_combustible(&combustible);

    menu_principal(vehiculos, num_vehiculos, combustible);

    // Liberar memoria antes de salir
    free(vehiculos);
    return 0;
}

// --- 1. SISTEMA CÁLCULO COSTO DE VIAJE (MENU) ---
void menu_principal(Vehiculo *v, int n, Combustible c)
{
    int opcion;

    do {
        printf("\n\n--- MENU PRINCIPAL ---\n");
        printf("1. Agregar Vehiculo\n");
        printf("2. Lista Vehiculo\n");
        printf("3. Modificar Vehiculo\n");
        printf("4. Agregar/Modificar Combustible\n");
        printf("5. Lista Combustible\n");
        printf("6. Calcular Viaje\n");
        printf("7. Salir\n");
        printf("Elija una opcion: ");

        if (scanf("%d", &opcion) != 1)
            {
            limpiar_buffer();
            opcion = 0; // Opción inválida
        }

        switch (opcion) {
            case 1: agregar_vehiculo(&v, &n); guardar_vehiculos(v, n); break;
            case 2: lista_vehiculo(v, n); break;
            case 3: modificar_vehiculo(v, n); guardar_vehiculos(v, n); break;
            case 4: agregar_modificar_combustible(&c); guardar_combustible(c); break;
            case 5: lista_combustible(c); break;
            case 6: calcular_viaje(v, n, c); break;
            case 7:
                printf("\nGuardando y saliendo del sistema...\n");
                break;
            default:
                printf("Opcion no valida. Intente de nuevo.\n");
        }
    } while (opcion != 7);
}

// --- FUNCIONES DE PERSISTENCIA ---

void limpiar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Carga los datos de vehículos desde el archivo
void cargar_vehiculos(Vehiculo **v, int *n)
{
    FILE *f = fopen(ARCHIVO_VEHICULOS, "r");
    if (f == NULL) {
        printf("[INFO] Archivo de vehiculos no encontrado. Iniciando con inventario vacio.\n");
        *n = 0;
        *v = NULL;
        return;
    }

    // Contar líneas para determinar el número de vehículos
    int count = 0;
    Vehiculo temp;
    while (fscanf(f, "%d;%29[^;];%29[^;];%d;%9[^;];%f;%f;%f;%f;%d;%f;%f\n",
                  &temp.id, temp.marca, temp.modelo, &temp.anio, temp.placa,
                  &temp.consumo_ciudad, &temp.consumo_carretera, &temp.costo_seguro,
                  &temp.costo_cambio_km, &temp.depreciacion_anios, &temp.costo_vehiculo,
                  &temp.costo_limpieza) == 12) {
        count++;
    }
    rewind(f); // Volver al inicio del archivo

    *n = count;
    *v = (Vehiculo *)malloc(*n * sizeof(Vehiculo));
    if (*v == NULL)
        {
        perror("Error de memoria al cargar vehiculos");
        exit(EXIT_FAILURE);
    }

    // Leer los datos
    for (int i = 0; i < *n; i++)
        {
        fscanf(f, "%d;%29[^;];%29[^;];%d;%9[^;];%f;%f;%f;%f;%d;%f;%f\n",
               &(*v)[i].id, (*v)[i].marca, (*v)[i].modelo, &(*v)[i].anio, (*v)[i].placa,
               &(*v)[i].consumo_ciudad, &(*v)[i].consumo_carretera, &(*v)[i].costo_seguro,
               &(*v)[i].costo_cambio_km, &(*v)[i].depreciacion_anios, &(*v)[i].costo_vehiculo,
               &(*v)[i].costo_limpieza);
    }

    fclose(f);
    printf("[INFO] %d vehiculo(s) cargado(s).\n", *n);
}

void guardar_vehiculos(Vehiculo *v, int n)
{
    FILE *f = fopen(ARCHIVO_VEHICULOS, "w");
    if (f == NULL)
        {
        perror("Error al abrir archivo para guardar vehiculos");
        return;
    }

    for (int i = 0; i < n; i++) {
        fprintf(f, "%d;%s;%s;%d;%s;%.2f;%.2f;%.2f;%.2f;%d;%.2f;%.2f\n",
                v[i].id, v[i].marca, v[i].modelo, v[i].anio, v[i].placa,
                v[i].consumo_ciudad, v[i].consumo_carretera, v[i].costo_seguro,
                v[i].costo_cambio_km, v[i].depreciacion_anios, v[i].costo_vehiculo,
                v[i].costo_limpieza);
    }
    fclose(f);
    printf("[INFO] Vehiculos guardados.\n");
}

void cargar_combustible(Combustible *c)
 {
    FILE *f = fopen(ARCHIVO_COMBUSTIBLE, "r");
    if (f == NULL || fscanf(f, "%19[^;];%f", c->nombre, &c->precio_litro) != 2)
        {
        printf("[INFO] Archivo de combustible no encontrado o vacio. Usando valores por defecto.\n");
        c->precio_litro = 0.0;
        return;
    }
    fclose(f);
    printf("[INFO] Costo de combustible cargado (Precio: %.2f).\n", c->precio_litro);
}

void guardar_combustible(Combustible c)
{
    FILE *f = fopen(ARCHIVO_COMBUSTIBLE, "w");
    if (f == NULL)
        {
        perror("Error al abrir archivo para guardar combustible");
        return;
    }
    fprintf(f, "%s;%.2f", c.nombre, c.precio_litro);
    fclose(f);
    printf("[INFO] Costo de combustible guardado.\n");
}


// --- 1.1 AGREGAR VEHÍCULO ---
void agregar_vehiculo(Vehiculo **v, int *n)
{
    Vehiculo nuevo;

    printf("\n--- AGREGAR VEHICULO ---\n");
    printf("ID (entero, unico): ");
    scanf("%d", &nuevo.id);
    limpiar_buffer();

    printf("Marca: "); fgets(nuevo.marca, 30, stdin); nuevo.marca[strcspn(nuevo.marca, "\n")] = 0;
    printf("Modelo: "); fgets(nuevo.modelo, 30, stdin); nuevo.modelo[strcspn(nuevo.modelo, "\n")] = 0;
    printf("Anio: "); scanf("%d", &nuevo.anio);
    printf("Placa: "); scanf("%9s", nuevo.placa);

    printf("Consumo ciudad (L/100Km): "); scanf("%f", &nuevo.consumo_ciudad);
    printf("Consumo carretera (L/100Km): "); scanf("%f", &nuevo.consumo_carretera);
    printf("Costo seguro anio: "); scanf("%f", &nuevo.costo_seguro);
    printf("Costo cambio por Km (mantenimiento): "); scanf("%f", &nuevo.costo_cambio_km);
    printf("Tiempo de depreciacion (anios): "); scanf("%d", &nuevo.depreciacion_anios);
    printf("Costo Vehiculo (compra): "); scanf("%f", &nuevo.costo_vehiculo);
    printf("Costo Limpieza: "); scanf("%f", &nuevo.costo_limpieza);

    // Reasignar memoria para el nuevo vehículo
    *v = (Vehiculo *)realloc(*v, (*n + 1) * sizeof(Vehiculo));
    if (*v == NULL)
        {
        perror("Error de memoria al agregar vehiculo");
        exit(EXIT_FAILURE);
    }

    (*v)[*n] = nuevo;
    (*n)++;
    printf("\nVehiculo '%s %s' agregado (ID: %d).\n", nuevo.marca, nuevo.modelo, nuevo.id);
}

// --- 1.2 LISTA VEHÍCULO ---
void lista_vehiculo(Vehiculo *v, int n)
 {
    if (n == 0)
        {
        printf("\nNo hay vehiculos registrados.\n");
        return;
    }
    printf("\n--- LISTA DE VEHICULOS (%d) ---\n", n);
    printf("%-5s %-15s %-15s %-10s %-10s\n", "ID", "MARCA", "MODELO", "ANIO", "PLACA");
    printf("------------------------------------------------------------------\n");
    for (int i = 0; i < n; i++)
    {
        printf("%-5d %-15s %-15s %-10d %-10s\n", v[i].id, v[i].marca, v[i].modelo, v[i].anio, v[i].placa);
    }
}

// --- 1.3 MODIFICAR VEHÍCULO ---
void modificar_vehiculo(Vehiculo *v, int n)
{
    int id_modificar, indice = -1;
    if (n == 0)
        {
        printf("\nNo hay vehiculos para modificar.\n");
        return;
    }

    lista_vehiculo(v, n);
    printf("\nIngrese el ID del vehiculo a modificar: ");
    if (scanf("%d", &id_modificar) != 1)
        {
        limpiar_buffer();
        printf("ID invalido.\n");
        return;
    }

    for (int i = 0; i < n; i++)
        {
        if (v[i].id == id_modificar)
         {
            indice = i;
            break;
        }
    }

    if (indice == -1)
        {
        printf("Vehiculo con ID %d no encontrado.\n", id_modificar);
        return;
    }

    printf("\nModificando vehiculo ID: %d, Marca: %s, Modelo: %s\n", v[indice].id, v[indice].marca, v[indice].modelo);
    printf("Nuevo Consumo ciudad (L/100Km) [%.2f]: ", v[indice].consumo_ciudad);
    scanf("%f", &v[indice].consumo_ciudad);
    printf("Nuevo Costo seguro anio [%.2f]: ", v[indice].costo_seguro);
    scanf("%f", &v[indice].costo_seguro);
    // Podrías añadir más campos para modificar aquí

    printf("Vehiculo modificado exitosamente.\n");
}

// --- 1.4, 1.5, 1.6 GESTIÓN DE COMBUSTIBLE ---
void agregar_modificar_combustible(Combustible *c)
{
    printf("\n--- GESTIONAR COMBUSTIBLE ---\n");
    printf("Tipo de combustible actual: %s\n", c->nombre);
    printf("Precio por litro actual: %.2f\n", c->precio_litro);

    printf("Ingrese nuevo nombre del combustible (ej: Gasolina Premium): ");
    limpiar_buffer();
    fgets(c->nombre, 20, stdin); c->nombre[strcspn(c->nombre, "\n")] = 0;

    printf("Ingrese nuevo precio por litro: ");
    scanf("%f", &c->precio_litro);

    printf("Combustible actualizado.\n");
}

void lista_combustible(Combustible c)
{
    printf("\n--- COSTO DE COMBUSTIBLE ---\n");
    printf("Tipo: %s\n", c.nombre);
    printf("Precio por Litro: %.2f\n", c.precio_litro);
}

// --- 1.7 CALCULAR VIAJE ---
void calcular_viaje(Vehiculo *v, int n, Combustible c)
 {
    int id_elegido, km_total;
    float porcentaje_ciudad;
    int indice = -1;

    if (n == 0 || c.precio_litro == 0.0)
        {
        printf("\nERROR: Asegurese de registrar un vehiculo y el costo del combustible.\n");
        return;
    }

    // 1. Elegir vehículo
    lista_vehiculo(v, n);
    printf("\nIngrese el ID del vehiculo a utilizar: ");
    if (scanf("%d", &id_elegido) != 1)
        {
        limpiar_buffer();
        return;
    }

    for (int i = 0; i < n; i++)
        {
        if (v[i].id == id_elegido)
         {
            indice = i;
            break;
        }
    }
    if (indice == -1) {
        printf("Vehiculo con ID %d no encontrado.\n", id_elegido);
        return;
    }
    Vehiculo vehiculo_select = v[indice];

    // 2. Cantidad KM
    printf("Ingrese la cantidad total de KM del viaje: ");
    scanf("%d", &km_total);

    // 3. % KM Ciudad
    printf("Ingrese el porcentaje de KM que sera en ciudad (0-100): ");
    scanf("%f", &porcentaje_ciudad);
    if (porcentaje_ciudad < 0.0 || porcentaje_ciudad > 100.0)
        {
        printf("Porcentaje invalido.\n");
        return;
    }

    // --- CALCULOS ---
    float km_ciudad = km_total * (porcentaje_ciudad / 100.0);
    float km_carretera = km_total - km_ciudad;

    // Costo Combustible
    float consumo_ciudad_litros = km_ciudad * (vehiculo_select.consumo_ciudad / 100.0);
    float consumo_carretera_litros = km_carretera * (vehiculo_select.consumo_carretera / 100.0);
    float litros_total = consumo_ciudad_litros + consumo_carretera_litros;
    float costo_combustible = litros_total * c.precio_litro;

    // Costo Mantenimiento (cambio por Km)
    float costo_mantenimiento = km_total * vehiculo_select.costo_cambio_km;

    // Otros costos (por viaje, simplificando: seguro/limpieza/depreciacion por Km)
    // Nota: Para ser más precisos, necesitaríamos el Km promedio año y la vida útil
    //       Aquí haremos una simplificación que debe ajustarse a los requerimientos

    // Asumimos 365 días al año para prorratear el seguro
    float costo_seguro_viaje = (vehiculo_select.costo_seguro / 365.0) * (km_total / 200.0); // Estimacion de 200km/dia
    float costo_limpieza_viaje = vehiculo_select.costo_limpieza; // Costo fijo por viaje

    // Costo de depreciación por viaje (Necesitas definir "Km promedio año" para ser exactos)
    // Suponemos una depreciación total a lo largo de 150,000 Km
    float costo_depreciacion_km = vehiculo_select.costo_vehiculo / 150000.0;
    float costo_depreciacion_viaje = costo_depreciacion_km * km_total;

    float costo_total = costo_combustible + costo_mantenimiento + costo_seguro_viaje + costo_limpieza_viaje + costo_depreciacion_viaje;
    float costo_por_km = costo_total / km_total;

    // --- 3. SALIDAS ---
    printf("\n--- RESULTADOS DEL VIAJE ---\n");
    printf("Vehiculo: %s %s (ID: %d)\n", vehiculo_select.marca, vehiculo_select.modelo, vehiculo_select.id);
    printf("Distancia Total: %d Km (Ciudad: %.0f Km, Carretera: %.0f Km)\n", km_total, km_ciudad, km_carretera);
    printf("Litros Consumidos: %.2f L\n", litros_total);
    printf("----------------------------------------\n");
    printf("Costo Combustible: %.2f\n", costo_combustible);
    printf("Costo Mantenimiento (Km): %.2f\n", costo_mantenimiento);
    printf("Costo Depreciacion: %.2f\n", costo_depreciacion_viaje);
    printf("Costo Seguro (prorrateado): %.2f\n", costo_seguro_viaje);
    printf("Costo Limpieza: %.2f\n", costo_limpieza_viaje);
    printf("----------------------------------------\n");
    printf("1. Costo Total: %.2f\n", costo_total);
    printf("2. Costo por Km: %.2f\n", costo_por_km);
    printf("----------------------------------------\n");
}
