#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>


int contador_subida = 0;
int contador_bajada = 0;
pthread_mutex_t * mutex_subida;
pthread_mutex_t * mutex_bajada;
sem_t * puente;
void inicializar_semaforos () {
    mutex_bajada = malloc(sizeof(pthread_mutex_t));
    sem_init(mutex_bajada, 0, 1);
    mutex_subida = malloc(sizeof(pthread_mutex_t));
    sem_init(mutex_subida, 0, 1);
    puente = malloc(sizeof(sem_t));
    sem_init(puente, 0, 1);

}
void subida(int id) {
    sem_wait(mutex_subida);
    contador_subida++;
    if(contador_subida == 1){
        sem_wait(puente);
    }
    sem_post(mutex_subida);
        printf("Subi al puente :D atte: auto nro %d .Cantidad de autos %d  \n", id, contador_subida);
    sem_wait(mutex_subida);
    contador_subida--;
    if(contador_subida == 0){
        sem_post(puente);
    }
    sem_post(mutex_subida);

}
void bajada(int id) {
        sem_wait(mutex_bajada);
        contador_bajada++;
        if(contador_bajada == 1){
            sem_wait(puente);
        }
        sem_post(mutex_bajada);
        printf("Baje al puente :D atte: auto nro %d. Cantidad de autos %d \n", id, contador_bajada);
    sem_wait(mutex_bajada);
    contador_bajada--;
    if(contador_bajada == 0){
        sem_post(puente);
    }
    sem_post(mutex_bajada);
}
void crear_hilos (int id) {
    void * accion = rand()%2 == 0 ? subida : bajada;
        pthread_t * autito = malloc(sizeof(pthread_t));
        pthread_create(autito, NULL, accion, id);
        pthread_detach(autito);
}
int main() {
    inicializar_semaforos();
    int id=1;
    while(1){
        crear_hilos(id);
        id++;
    }
}
