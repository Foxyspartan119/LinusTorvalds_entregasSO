//Linus Torvalds 4CV4

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LEN 1024

// Función para distribuir líneas pares e impares
void leerYDistribuir(FILE *file, FILE *pares, FILE *impares) {
    char buffer[MAX_LINE_LEN];
    int lineNum = 0;
    
    // Leer línea por línea de stdin
    while (fgets(buffer, MAX_LINE_LEN, file) != NULL) {
        if (strcmp(buffer, "\n") == 0) {  // Detectar línea vacía para terminar
            break;
        }
        if (lineNum % 2 == 0) {
            fprintf(pares, "%s", buffer);   // Línea par
        } else {
            fprintf(impares, "%s", buffer); // Línea impar
        }
        lineNum++;
    }
    fflush(pares);
    fflush(impares);
}

// Función que filtra las líneas con una palabra específica
void filtrarConGrep(FILE *entrada, const char *palabra) {
    char buffer[MAX_LINE_LEN];
    
    // Leer línea por línea del archivo
    while (fgets(buffer, MAX_LINE_LEN, entrada) != NULL) {
        if (strstr(buffer, palabra) != NULL) {
            printf("%s", buffer);  // Imprimir si la línea contiene la palabra
        }
    }
}

// Función para ordenar las líneas
void ordenarYLlamarSort(FILE *entrada) {
    char *lineas[100];
    char buffer[MAX_LINE_LEN];
    int count = 0;
    
    // Leer líneas
    while (fgets(buffer, MAX_LINE_LEN, entrada) != NULL) {
        lineas[count] = strdup(buffer);
        count++;
    }
    
    // Ordenar (simple bubble sort para mantenerlo simple)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(lineas[j], lineas[j + 1]) > 0) {
                char *temp = lineas[j];
                lineas[j] = lineas[j + 1];
                lineas[j + 1] = temp;
            }
        }
    }
    
    // Imprimir las líneas ordenadas
    printf("\nLíneas ordenadas:\n");
    for (int i = 0; i < count; i++) {
        printf("%s", lineas[i]);
        free(lineas[i]);  // Liberar memoria
    }
}

int main() {
    // Palabras fijas para los filtros
    const char *palabraPares = "prueba";
    const char *palabraImpares = "linea";
    
    // Archivos temporales para pares e impares
    FILE *pares = tmpfile();
    FILE *impares = tmpfile();
    
    // Leer de stdin y distribuir en archivos temporales
    printf("Introduce texto (línea vacía para terminar):\n");
    leerYDistribuir(stdin, pares, impares);
    
    // Rebobinar archivos para lectura
    rewind(pares);
    rewind(impares);
    
    // Filtrar líneas pares
    printf("\nFiltrando líneas pares con la palabra '%s':\n", palabraPares);
    filtrarConGrep(pares, palabraPares);
    
    // Filtrar líneas impares
    printf("\nFiltrando líneas impares con la palabra '%s':\n", palabraImpares);
    filtrarConGrep(impares, palabraImpares);
    
    // Rebobinar para ordenar las líneas pares filtradas
    rewind(pares);
    printf("\nOrdenando las líneas pares filtradas:\n");
    ordenarYLlamarSort(pares);
    
    return 0;
}
