//Linus Torvalds 4CV4

#include <stdio.h>
#include <pthread.h>

// Variable global para el saldo
int saldo = 0;

// Definimos un candado global
pthread_mutex_t lock;

// Función para depositar dinero
void *depositar(void *arg) {
    int cantidad;
    pthread_mutex_lock(&lock);
    printf("Ingrese la cantidad a depositar: ");
    scanf("%d", &cantidad);
    saldo += cantidad;
    printf("Depósito de %d, saldo actual: %d\n", cantidad, saldo);
    pthread_mutex_unlock(&lock);
    return NULL;
}

// Función para retirar dinero
void *retirar(void *arg) {
    int cantidad;
    pthread_mutex_lock(&lock);
    printf("Ingrese la cantidad a retirar: ");
    scanf("%d", &cantidad);
    if (saldo >= cantidad) {
        saldo -= cantidad;
        printf("Retiro de %d, saldo actual: %d\n", cantidad, saldo);
    } else {
        printf("Retiro fallido de %d, saldo insuficiente: %d\n", cantidad, saldo);
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main() {
    pthread_t hilo_depositar, hilo_retirar;

    // Inicializamos el candado
    pthread_mutex_init(&lock, NULL);

    // Creamos los hilos
    pthread_create(&hilo_depositar, NULL, depositar, NULL);
    pthread_create(&hilo_retirar, NULL, retirar, NULL);

    // Esperamos a que los hilos terminen
    pthread_join(hilo_depositar, NULL);
    pthread_join(hilo_retirar, NULL);

    // Destruimos el candado
    pthread_mutex_destroy(&lock);

    printf("Saldo final: %d\n", saldo);
    return 0;
}
