#include <stdlib.h>
#include <stdio.h> 
#include <string.h>

/*================ listar examenes funcional en windows y linux ======================*/

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dirent.h>
    #include <string.h>
#endif

void LISTAR_EXAMENES_DISPONIBLES(void) {
#ifdef _WIN32
    WIN32_FIND_DATA DATOS_ARCHIVO;
    HANDLE BUSCADOR = FindFirstFile("*.txt", &DATOS_ARCHIVO);

    if (BUSCADOR == INVALID_HANDLE_VALUE) {
        printf("No se encontraron archivos .txt\n");
        return;
    }

    printf("\n--- Examenes disponibles (.txt) ---\n");
    do {
        printf(" - %s\n", DATOS_ARCHIVO.cFileName);
    } while (FindNextFile(BUSCADOR, &DATOS_ARCHIVO) != 0);
    FindClose(BUSCADOR);
    printf("------------------------------------\n");

#else
    DIR *CARPETA = opendir(".");
    struct dirent *ENTRADA;

    if (CARPETA == NULL) {
        printf("No se pudo leer la carpeta actual\n");
        return;
    }

    printf("\n--- Examenes disponibles (.txt) ---\n");
    while ((ENTRADA = readdir(CARPETA)) != NULL) {
        if (strstr(ENTRADA->d_name, ".txt") != NULL) {
            printf(" - %s\n", ENTRADA->d_name);
        }
    }
    printf("------------------------------------\n");
    closedir(CARPETA);
#endif
}

/*================ limpiar pantalla windows y linux ======================*/

void LIMPIAR_PANTALLA(void) {
    fflush(stdout);
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/*================ main struct ======================*/

#define MAX_TEXT 512

typedef struct REACTIVO {
    char PREGUNTA[MAX_TEXT];
    char OPCION_1[MAX_TEXT];
    char OPCION_2[MAX_TEXT];
    char OPCION_3[MAX_TEXT];
    char OPCION_4[MAX_TEXT];
    int  RESPUESTA_CORRECTA;
    int  RESPUESTA_USUARIO;
    float PUNTOS;
    struct REACTIVO *SIGUIENTE;
    struct REACTIVO *ANTERIOR;
} REACTIVO;

/*================ lista doblemente enlazada ======================*/

typedef struct {
    REACTIVO *CABEZA;
    REACTIVO *COLA;
    int TOTAL_REACTIVOS;
} LISTA_EXAMEN;

void INICIALIZAR_LISTA(LISTA_EXAMEN *LISTA) {
    LISTA->CABEZA = NULL;
    LISTA->COLA = NULL;
    LISTA->TOTAL_REACTIVOS = 0;
}

void AGREGAR_REACTIVO(LISTA_EXAMEN *LISTA, REACTIVO *NUEVO) {
    NUEVO->SIGUIENTE = NULL;
    NUEVO->ANTERIOR = LISTA->COLA;

    if (LISTA->CABEZA == NULL) {
        LISTA->CABEZA = NUEVO;
    } else {
        LISTA->COLA->SIGUIENTE = NUEVO;
    }
    LISTA->COLA = NUEVO;
    LISTA->TOTAL_REACTIVOS++;
}

/*================ Crear, borrar, reactivos ======================*/

REACTIVO* CREAR_REACTIVO_VACIO(void) {
    REACTIVO *NUEVO = (REACTIVO*) malloc(sizeof(REACTIVO));

    if (NUEVO == NULL) {
        printf("ERROR: no se pudo reservar memoria\n");
        return NULL;
    }

    NUEVO->PREGUNTA[0] = '\0';
    NUEVO->OPCION_1[0] = '\0';
    NUEVO->OPCION_2[0] = '\0';
    NUEVO->OPCION_3[0] = '\0';
    NUEVO->OPCION_4[0] = '\0';
    NUEVO->RESPUESTA_CORRECTA = 0;
    NUEVO->RESPUESTA_USUARIO = 0;
    NUEVO->PUNTOS = 0;
    NUEVO->PUNTOS = 0;
    NUEVO->SIGUIENTE = NULL;
    NUEVO->ANTERIOR = NULL;

    return NUEVO;
}

void LIBERAR_LISTA(LISTA_EXAMEN *LISTA) {
    REACTIVO *ACTUAL = LISTA->CABEZA;
    REACTIVO *SIGUIENTE_TEMP;

    while (ACTUAL != NULL) {
        SIGUIENTE_TEMP = ACTUAL->SIGUIENTE;
        free(ACTUAL);
        ACTUAL = SIGUIENTE_TEMP;
    }

    LISTA->CABEZA = NULL;
    LISTA->COLA = NULL;
    LISTA->TOTAL_REACTIVOS = 0;
}

/*================ buscar y mostrar reactivos ======================*/

REACTIVO* BUSCAR_REACTIVO_POR_NUMERO(LISTA_EXAMEN *LISTA, int NUM) {
    REACTIVO *ACTUAL = LISTA->CABEZA;
    int CONTADOR = 1;

    while (ACTUAL != NULL) {
        if (CONTADOR == NUM) {
            return ACTUAL;
        }
        ACTUAL = ACTUAL->SIGUIENTE;
        CONTADOR++;
    }
    return NULL;
}

void IMPRIMIR_EXAMEN(LISTA_EXAMEN *LISTA) {
    REACTIVO *ACTUAL = LISTA->CABEZA;
    int NUM = 1;

    printf("\n--- EXAMEN ACTUAL (%d reactivos) ---\n", LISTA->TOTAL_REACTIVOS);
    while (ACTUAL != NULL) {
        printf("%d. %s\n", NUM, ACTUAL->PREGUNTA);
        ACTUAL = ACTUAL->SIGUIENTE;
        NUM++;
    }
    printf("------------------------------------\n");
}

/*================ capturar datos con teclado ======================*/

void CAPTURAR_REACTIVO_TECLADO(REACTIVO *R) {
    printf("Pregunta: ");
    getchar();
    fgets(R->PREGUNTA, MAX_TEXT, stdin);
    R->PREGUNTA[strcspn(R->PREGUNTA, "\n")] = '\0';

    printf("Opcion 1: ");
    fgets(R->OPCION_1, MAX_TEXT, stdin);
    R->OPCION_1[strcspn(R->OPCION_1, "\n")] = '\0';

    printf("Opcion 2: ");
    fgets(R->OPCION_2, MAX_TEXT, stdin);
    R->OPCION_2[strcspn(R->OPCION_2, "\n")] = '\0';

    printf("Opcion 3: ");
    fgets(R->OPCION_3, MAX_TEXT, stdin);
    R->OPCION_3[strcspn(R->OPCION_3, "\n")] = '\0';

    printf("Opcion 4: ");
    fgets(R->OPCION_4, MAX_TEXT, stdin);
    R->OPCION_4[strcspn(R->OPCION_4, "\n")] = '\0';

    printf("Cual es la correcta (1-4): ");
    scanf("%d", &R->RESPUESTA_CORRECTA);

    printf("Puntos asignados: ");
    scanf("%f", &R->PUNTOS);
}

/*================ modificar examen ======================*/

void MODIFICAR_EXAMEN(LISTA_EXAMEN *LISTA) {
    int OPCION_NUM;
    char RESPUESTA;

    if (LISTA->TOTAL_REACTIVOS == 0) {
        printf("No hay reactivos cargados.\n");
        return;
    }

    IMPRIMIR_EXAMEN(LISTA);
    printf("Que reactivo deseas modificar? (numero): ");
    scanf("%d", &OPCION_NUM);

    REACTIVO *OBJETIVO = BUSCAR_REACTIVO_POR_NUMERO(LISTA, OPCION_NUM);

    if (OBJETIVO == NULL) {
        printf("Ese numero no existe.\n");
        return;
    }

    printf("Pregunta actual: %s\n", OBJETIVO->PREGUNTA);
    printf("Deseas modificarla? (S/N): ");
    scanf(" %c", &RESPUESTA);

    if (RESPUESTA == 'S' || RESPUESTA == 's') {
        CAPTURAR_REACTIVO_TECLADO(OBJETIVO);
        printf("Reactivo actualizado.\n");
    } else {
        printf("Sin cambios.\n");
    }
}

/*================ generar examen ======================*/

void GENERAR_EXAMEN(LISTA_EXAMEN *LISTA) {
    char RESPUESTA = 'S';

    while (RESPUESTA == 'S' || RESPUESTA == 's') {
        REACTIVO *NUEVO = CREAR_REACTIVO_VACIO();
        if (NUEVO == NULL) {
            return;
        }

        CAPTURAR_REACTIVO_TECLADO(NUEVO);
        AGREGAR_REACTIVO(LISTA, NUEVO);

        printf("Agregar otro reactivo? (S/N): ");
        scanf(" %c", &RESPUESTA);
    }
}

/*================ guardar examen en archivo ======================*/

void GUARDAR_EXAMEN(LISTA_EXAMEN *LISTA, const char *NOMBRE_ARCHIVO) {
    FILE *ARCHIVO = fopen(NOMBRE_ARCHIVO, "w");

    if (ARCHIVO == NULL) {
        printf("ERROR: no se pudo abrir el archivo para escribir\n");
        return;
    }

    REACTIVO *ACTUAL = LISTA->CABEZA;
    while (ACTUAL != NULL) {
        fprintf(ARCHIVO, ":p;%s\n", ACTUAL->PREGUNTA);
        fprintf(ARCHIVO, ":op1;%s\n", ACTUAL->OPCION_1);
        fprintf(ARCHIVO, ":op2;%s\n", ACTUAL->OPCION_2);
        fprintf(ARCHIVO, ":op3;%s\n", ACTUAL->OPCION_3);
        fprintf(ARCHIVO, ":op4;%s\n", ACTUAL->OPCION_4);
        fprintf(ARCHIVO, ":r;op%d\n", ACTUAL->RESPUESTA_CORRECTA);
        fprintf(ARCHIVO, "%.1f.\n", ACTUAL->PUNTOS);

        ACTUAL = ACTUAL->SIGUIENTE;
    }

    fclose(ARCHIVO);
    printf("Examen guardado en %s\n", NOMBRE_ARCHIVO);
}

/*================ cargar examen desde archivo ======================*/

int CARGAR_EXAMEN(LISTA_EXAMEN *LISTA, const char *NOMBRE_ARCHIVO) {
    FILE *ARCHIVO = fopen(NOMBRE_ARCHIVO, "r");

    if (ARCHIVO == NULL) {
        printf("ERROR: no se pudo abrir el archivo %s\n", NOMBRE_ARCHIVO);
        return 0;
    }

    char LINEA[MAX_TEXT + 10];
    REACTIVO *NUEVO = NULL;

    while (fgets(LINEA, sizeof(LINEA), ARCHIVO) != NULL) {

        LINEA[strcspn(LINEA, "\n")] = '\0';

        if (strncmp(LINEA, ":p;", 3) == 0) {
            NUEVO = CREAR_REACTIVO_VACIO();
            if (NUEVO == NULL) {
                fclose(ARCHIVO);
                return 0;
            }
            strcpy(NUEVO->PREGUNTA, LINEA + 3);
        }
        else if (strncmp(LINEA, ":op1;", 5) == 0) {
            strcpy(NUEVO->OPCION_1, LINEA + 5);
        }
        else if (strncmp(LINEA, ":op2;", 5) == 0) {
            strcpy(NUEVO->OPCION_2, LINEA + 5);
        }
        else if (strncmp(LINEA, ":op3;", 5) == 0) {
            strcpy(NUEVO->OPCION_3, LINEA + 5);
        }
        else if (strncmp(LINEA, ":op4;", 5) == 0) {
            strcpy(NUEVO->OPCION_4, LINEA + 5);
        }
        else if (strncmp(LINEA, ":r;op", 5) == 0) {
            NUEVO->RESPUESTA_CORRECTA = LINEA[5] - '0';
        }
        else if (strlen(LINEA) > 0) {
            sscanf(LINEA, "%f", &NUEVO->PUNTOS);
            AGREGAR_REACTIVO(LISTA, NUEVO);
            NUEVO = NULL;
        }
    }

    fclose(ARCHIVO);
    printf("Examen cargado: %d reactivos\n", LISTA->TOTAL_REACTIVOS);
    return 1;
}
/*================ aplicar examen ======================*/

void MOSTRAR_REACTIVO_APLICAR(REACTIVO *R, int NUMERO, int TOTAL) {
    LIMPIAR_PANTALLA();
    printf("\n========================================\n");
    printf("Pregunta %d de %d\n", NUMERO, TOTAL);
    printf("========================================\n");
    printf("%s\n\n", R->PREGUNTA);
    printf("1. %s\n", R->OPCION_1);
    printf("2. %s\n", R->OPCION_2);
    printf("3. %s\n", R->OPCION_3);
    printf("4. %s\n", R->OPCION_4);

    if (R->RESPUESTA_USUARIO != 0) {
        printf("\n(Tu respuesta actual: %d)\n", R->RESPUESTA_USUARIO);
    } else {
        printf("\n(Sin responder)\n");
    }

    printf("----------------------------------------\n");
    printf("a = anterior | d = siguiente | 1-4 = responder | f = finalizar\n");
    printf("Opcion: ");
}

/*======================Calificar examen ===================================*/

void CALIFICAR_EXAMEN(LISTA_EXAMEN *LISTA) {
    REACTIVO *ACTUAL = LISTA->CABEZA;
    float PUNTOS_OBTENIDOS = 0;
    float PUNTOS_TOTALES = 0;

    while (ACTUAL != NULL) {
        PUNTOS_TOTALES = PUNTOS_TOTALES + ACTUAL->PUNTOS;

        if (ACTUAL->RESPUESTA_USUARIO == ACTUAL->RESPUESTA_CORRECTA) {
            PUNTOS_OBTENIDOS = PUNTOS_OBTENIDOS + ACTUAL->PUNTOS;
        }

        ACTUAL = ACTUAL->SIGUIENTE;
    }

    printf("\n========================================\n");
    printf("   RESULTADO FINAL\n");
    printf("========================================\n");
    printf("Obtuviste %.1f de %.1f puntos\n", PUNTOS_OBTENIDOS, PUNTOS_TOTALES);
    printf("========================================\n");

    printf("\nPresiona ENTER para volver al menu...");
    while (getchar() != '\n');
    getchar();
}
/*==================Aplicar Examen y a,d stuff =====================*/

void APLICAR_EXAMEN(LISTA_EXAMEN *LISTA) {
    if (LISTA->TOTAL_REACTIVOS == 0) {
        printf("No hay reactivos cargados.\n");
        return;
    }

    REACTIVO *ACTUAL = LISTA->CABEZA;
    int NUMERO = 1;
    char TECLA;
    int TERMINAR = 0;

    while (TERMINAR == 0) {
        MOSTRAR_REACTIVO_APLICAR(ACTUAL, NUMERO, LISTA->TOTAL_REACTIVOS);
        scanf(" %c", &TECLA);

        if (TECLA == 'a' || TECLA == 'A') {
            if (ACTUAL->ANTERIOR != NULL) {
                ACTUAL = ACTUAL->ANTERIOR;
                NUMERO--;
            } else {
                printf("Ya estas en la primera pregunta.\n");
            }
        }
        else if (TECLA == 'd' || TECLA == 'D') {
            if (ACTUAL->SIGUIENTE != NULL) {
                ACTUAL = ACTUAL->SIGUIENTE;
                NUMERO++;
            } else {
                printf("Ya estas en la ultima pregunta.\n");
            }
        }
        else if (TECLA >= '1' && TECLA <= '4') {
            ACTUAL->RESPUESTA_USUARIO = TECLA - '0';   // convierte el char '1'-'4' a int 1-4
        }
        else if (TECLA == 'f' || TECLA == 'F') {
            TERMINAR = 1;
        }
        else {
            printf("Opcion invalida.\n");
        }
    }

    CALIFICAR_EXAMEN(LISTA);
}
/*================ menu principal ======================*/

void MOSTRAR_MENU(void) {
    LIMPIAR_PANTALLA();
    printf("\n========================================\n");
    printf("   GENERADOR DE EXAMENES\n");
    printf("========================================\n");
    printf("1. Generar examen\n");
    printf("2. Modificar examen\n");
    printf("3. Aplicar examen\n");
    printf("4. Salir\n");
    printf("Selecciona una opcion: ");
}

/*==================MAIN===============================*/

int main(void) {
    LISTA_EXAMEN LISTA;
    INICIALIZAR_LISTA(&LISTA);

    int OPCION = 0;
    char NOMBRE_ARCHIVO[256];

    while (OPCION != 4) {
        MOSTRAR_MENU();
        scanf("%d", &OPCION);

        switch (OPCION) {
            case 1:
                GENERAR_EXAMEN(&LISTA);
                printf("Nombre del archivo para guardar: ");
                scanf("%s", NOMBRE_ARCHIVO);
                GUARDAR_EXAMEN(&LISTA, NOMBRE_ARCHIVO);
                break;

            case 2:
              if (LISTA.TOTAL_REACTIVOS == 0) {
                printf("Nombre del archivo a cargar: ");
                scanf("%s", NOMBRE_ARCHIVO);
                CARGAR_EXAMEN(&LISTA, NOMBRE_ARCHIVO);
               } 
                MODIFICAR_EXAMEN(&LISTA);
                GUARDAR_EXAMEN(&LISTA, NOMBRE_ARCHIVO);
                break;  

            case 3:
              LISTAR_EXAMENES_DISPONIBLES();
              printf("Nombre del archivo a aplicar: ");
              scanf("%s", NOMBRE_ARCHIVO);
              LIBERAR_LISTA(&LISTA);
              if (CARGAR_EXAMEN(&LISTA, NOMBRE_ARCHIVO)) {
                APLICAR_EXAMEN(&LISTA);
              }
              break; 

            case 4:
                printf("Saliendo...\n");
                break;

            default:
                printf("Opcion invalida\n");
                break;
        }
    }

    LIBERAR_LISTA(&LISTA);
    return 0;
}
