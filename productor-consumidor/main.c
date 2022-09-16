#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>


int cantidad_consumidores = 3;
int cantidad_productores = 1;
pthread_mutex_t * mutex_recurso;
sem_t * contador_vacios;
sem_t * contador_llenos;
void inicializar_semaforos () {
    mutex_recurso = malloc(sizeof(pthread_mutex_t));
    sem_init(mutex_recurso, 0, 1);
    contador_vacios = malloc(sizeof(sem_t));
    sem_init(contador_vacios, 0, 0);
    contador_llenos = malloc(sizeof(sem_t));
    sem_init(contador_llenos, 0, 1);

}
void producir(int id) {
    while(1){
        sem_wait(contador_vacios);
        sem_wait(mutex_recurso);
        printf("Producido por %d \n", id);
        sem_post(mutex_recurso);
        sem_post(contador_llenos);
        sleep(3);
    }
}
void consumir(int id) {
    while(1){
        sem_wait(contador_llenos);
        sem_wait(mutex_recurso);
        printf("consumido por %d \n", id);
        sem_post(mutex_recurso);
        sem_post(contador_vacios);
        sleep(2);
    }
}
void crear_hilos () {
    for (int i = 0; i < cantidad_productores; i++) {
        pthread_t * productor = malloc(sizeof(pthread_t));
        pthread_create(productor, NULL, producir, i+1);
        pthread_detach(productor);
    }
    for (int i = 0; i < cantidad_consumidores; i++) {
        pthread_t * consumidor = malloc(sizeof(pthread_t));
        pthread_create(consumidor, NULL, consumir, i+1);
        pthread_detach(consumidor);
    }
}
int main() {
    inicializar_semaforos();
    crear_hilos();
    while(1){
        sleep(1);
    }
}
