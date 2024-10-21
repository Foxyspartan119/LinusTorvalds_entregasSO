//Conversión de LFU a NRU
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
    bool referenced;    // Bit de referencia (para saber si ha sido usada recientemente)
    bool modified;      // Bit de modificación (para saber si la página fue escrita)
    struct Frame *prev; // Puntero al frame previo (para lista doblemente enlazada)
    struct Frame *next; // Puntero al frame siguiente (para lista doblemente enlazada)
} Frame;

// Estructura para la lista de frames en memoria física
typedef struct FrameList {
    int numFrames;      // Número de frames actualmente ocupados
    Frame *head;        // Puntero al primer frame de la lista
    Frame *tail;        // Puntero al último frame de la lista
} FrameList;

// Función para crear un nuevo frame
Frame* createFrame() {
    Frame *frame = (Frame *)malloc(sizeof(Frame));
    if (frame != NULL) {
        frame->page = -1;   // Inicialmente no hay página asignada
        frame->valid = false;
        frame->referenced = false;
        frame->modified = false;
        frame->prev = NULL;
        frame->next = NULL;
    }
    return frame;
}

// Función para inicializar la lista de frames en memoria física
FrameList* createFrameList() {
    FrameList *frameList = (FrameList *)malloc(sizeof(FrameList));
    if (frameList != NULL) {
        frameList->numFrames = 0;
        frameList->head = NULL;
        frameList->tail = NULL;
    }
    return frameList;
}

// Función para insertar un frame al frente de la lista (más recientemente usado)
void insertFrame(FrameList *frameList, Frame *frame) {
    if (frameList->head == NULL) {
        // Lista vacía
        frameList->head = frame;
        frameList->tail = frame;
    } else {
        // Insertar al frente de la lista
        frame->next = frameList->head;
        frameList->head->prev = frame;
        frameList->head = frame;
    }
    frameList->numFrames++;
}

// Función para eliminar un frame de la lista
void removeFrame(FrameList *frameList, Frame *frame) {
    if (frame->prev != NULL) {
        frame->prev->next = frame->next;
    } else {
        frameList->head = frame->next;
    }
    if (frame->next != NULL) {
        frame->next->prev = frame->prev;
    } else {
        frameList->tail = frame->prev;
    }
    frameList->numFrames--;
    free(frame);
}

// Función para buscar un frame específico por número de página
Frame* findFrame(FrameList *frameList, int page) {
    Frame *current = frameList->head;
    while (current != NULL) {
        if (current->page == page) {
            current->referenced = true; // Actualizamos el bit de referencia
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Función para encontrar el frame a reemplazar
Frame* findNRUFrame(FrameList *frameList) {
    Frame *candidates[4] = {NULL, NULL, NULL, NULL};
    Frame *current = frameList->head;

    while (current != NULL) {
        int class = (current->referenced << 1) | current->modified;
        if (candidates[class] == NULL) {
            candidates[class] = current;
        }
        current = current->next;
    }

    // Retorna el primer candidato encontrado en la clase más baja posible (0 a 3)
    for (int i = 0; i < 4; i++) {
        if (candidates[i] != NULL) {
            return candidates[i];
        }
    }
    return NULL;
}

// Función para simular la carga de una página a memoria física
void loadPage(FrameList *frameList, int page, bool isWrite) {
    Frame *frame = findFrame(frameList, page);
    
    if (frame != NULL) {
        // La página ya está en memoria, solo actualizamos el bit de modificación si es necesario
        if (isWrite) {
            frame->modified = true;
        }
        return;
    }
    
    // La página no está en memoria, así que la cargamos
    frame = createFrame();
    frame->page = page;
    frame->valid = true;
    frame->referenced = true;
    frame->modified = isWrite;
    
    // Si la lista de frames ya está llena, eliminar el frame menos recientemente usado
    if (frameList->numFrames == NUM_FRAMES) {
        Frame *nruFrame = findNRUFrame(frameList);
        removeFrame(frameList, nruFrame);
    }
    
    insertFrame(frameList, frame);
}

// Función para limpiar periódicamente los bits de referencia
void resetReferencedBits(FrameList *frameList) {
    Frame *current = frameList->head;
    while (current != NULL) {
        current->referenced = false;
        current = current->next;
    }
}

// Función para imprimir el estado actual de la lista de frames
void printFrameList(FrameList *frameList) {
    printf("Estado actual de la lista de frames:\n");
    Frame *current = frameList->head;
    while (current != NULL) {
        printf("Página: %d, Referenciado: %d, Modificado: %d\n", current->page, current->referenced, current->modified);
        current = current->next;
    }
    printf("\n");
}

int main() {
    FrameList *frameList = createFrameList();
    
    // Simular la carga de varias páginas a memoria física
    loadPage(frameList, 1, false);
    loadPage(frameList, 2, true);
    loadPage(frameList, 3, false);
    loadPage(frameList, 4, true);
    printFrameList(frameList);  // Debería imprimir el estado actual de los frames
    
    // Simular un reinicio de los bits de referencia 
    resetReferencedBits(frameList);
    
    // Intentar cargar otra página cuando todos los frames están ocupados
    loadPage(frameList, 5, false);
    printFrameList(frameList);  // Debería imprimir el estado actual después de la sustitución
    
    // Liberar la memoria utilizada por la lista de frames
    free(frameList);
    
    return 0;
}