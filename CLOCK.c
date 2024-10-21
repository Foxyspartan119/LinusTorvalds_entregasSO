//Codigo con algoritmo CLOCK
//Linus Torvalds 4CV4
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define NUM_FRAMES 4   // Número de frames (páginas físicas en memoria)
#define NUM_PAGES 10   // Número total de páginas virtuales

// Estructura para un frame de página en memoria física
typedef struct Frame {
    int page;           // Número de página almacenada en el frame (valor -1 si está vacío)
    bool valid;         // Indica si el frame está ocupado (true) o vacío (false)
    bool useBit;        // Bit de uso para el algoritmo CLOCK
} Frame;

// Array para representar los frames en memoria
Frame frames[NUM_FRAMES];

// Puntero circular para el algoritmo CLOCK
int clockPointer = 0;

// Función para inicializar los frames en memoria
void initializeFrames() {
    for (int i = 0; i < NUM_FRAMES; i++) {
        frames[i].page = -1;   // Inicialmente no hay página asignada
        frames[i].valid = false;
        frames[i].useBit = false;
    }
}

// Función para verificar si una página ya está cargada en memoria
int findPageInMemory(int page) {
    for (int i = 0; i < NUM_FRAMES; i++) {
        if (frames[i].valid && frames[i].page == page) {
            return i;  // Retorna el índice donde se encuentra la página
        }
    }
    return -1;  // Página no encontrada
}

// Función para cargar una página
void loadPage(int page) {
    int frameIndex = findPageInMemory(page);
    
    if (frameIndex != -1) {
        // Página ya está en memoria, actualizar el bit de uso
        frames[frameIndex].useBit = true;
        return;
    }

    // Si llegamos aquí, la página no está en memoria, necesitamos cargarla
    while (true) {
        // Si el bit de uso es 0, reemplazamos esa página
        if (!frames[clockPointer].useBit) {
            frames[clockPointer].page = page;
            frames[clockPointer].valid = true;
            frames[clockPointer].useBit = true; // Establecemos el bit de uso porque la página ha sido recientemente referenciada
            // Avanzamos el puntero circular al siguiente frame
            clockPointer = (clockPointer + 1) % NUM_FRAMES;
            break;
        }
        
        // Si el bit de uso es 1, lo ponemos a 0 y avanzamos el puntero
        frames[clockPointer].useBit = false;
        clockPointer = (clockPointer + 1) % NUM_FRAMES;
    }
}

// Función para imprimir el estado actual de los frames
void printFrames() {
    printf("Estado actual de los frames:\n");
    for (int i = 0; i < NUM_FRAMES; i++) {
        printf("Frame %d: Página: %d, Estado: %s, Bit de uso: %d\n", i, frames[i].page, 
               frames[i].valid ? "Ocupado" : "Vacío", frames[i].useBit);
    }
    printf("\n");
}

int main() {
    initializeFrames();
    
    // Simulación del orden de accesos a las páginas
    int pageAccesses[NUM_PAGES] = {1, 2, 3, 4, 5, 1, 2, 1, 3, 4};
    
    // Cargar las páginas en memoria
    for (int i = 0; i < NUM_PAGES; i++) {
        loadPage(pageAccesses[i]);
        printFrames();
    }
    
    return 0;
}