//Linus Torvalds 4CV4

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define TOTAL_VEHICULOS 100
#define VEHICULOS_IZQUIERDA 50
#define VEHICULOS_DERECHA 50

sem_t mutex; // Semáforo para controlar el acceso al puente
int en_puente = 0; // Contador de vehículos en el puente
int sentido_actual = -1; // -1 indica vacío, 0 para izquierda, 1 para derecha

void* vehiculo(void* arg) {
    int id = (int)arg;
    int direccion = (id <= VEHICULOS_IZQUIERDA) ? 0 : 1; // 0 para izquierda, 1 para derecha

    // Espera aleatoria antes de intentar entrar al puente
    sleep(rand() % 3);

    sem_wait(&mutex); // Espera para entrar a la sección crítica

    // Si el puente está vacío o el vehículo es del mismo sentido
    if (sentido_actual == -1 || sentido_actual == direccion) {
        sentido_actual = direccion; // Establecer el sentido actual
        en_puente++; // Incrementar el contador de vehículos en el puente
        printf("Vehículo %d desde la %s está ENTRANDO al puente\n", 
               id, direccion == 0 ? "izquierda" : "derecha");
        sleep(1); // Simular tiempo en el puente
        printf("Vehículo %d desde la %s está SALIENDO del puente\n", 
               id, direccion == 0 ? "izquierda" : "derecha");
        en_puente--; // Decrementar el contador de vehículos en el puente

        // Si ya no hay vehículos en el puente, liberar el sentido
        if (en_puente == 0) {
            sentido_actual = -1; // Reiniciar el sentido
        }
    }

    sem_post(&mutex); // Liberar el semáforo
    return NULL;
}

int main() {
    pthread_t hilos[TOTAL_VEHICULOS];
    int ids[TOTAL_VEHICULOS];

    sem_init(&mutex, 0, 1); // Inicializar el semáforo

    // Creacion de vehiculos
    for (int i = 0; i < TOTAL_VEHICULOS; i++) {
        ids[i] = i + 1;
        pthread_create(&hilos[i], NULL, vehiculo, &ids[i]);
        sleep(1); // Esperar un segundo antes de crear el siguiente vehículo
    }

    // Esperar a que todos los vehículos terminen
    for (int i = 0; i < TOTAL_VEHICULOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    sem_destroy(&mutex); // Destruir el semáforo

    return 0;
}
