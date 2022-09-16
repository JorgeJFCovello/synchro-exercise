#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>


int cantidad_sillas = 10;
int cantidad_barberos = 1;
int esperando = 0;
pthread_mutex_t * mutex;
sem_t * contador_barberos;
sem_t * contador_clientes;
void inicializar_semaforos () {
    mutex = malloc(sizeof(pthread_mutex_t));
    sem_init(mutex, 0, 1);
    contador_barberos = malloc(sizeof(sem_t));
    sem_init(contador_barberos, 0, 0);
    contador_clientes = malloc(sizeof(sem_t));
    sem_init(contador_clientes, 0, 1);

}
void cortar_pelo(int id) {
    while(1){
        sem_wait(contador_clientes);
        sem_wait(mutex);
        esperando--;
        printf("corte pelo. atte: %d \n", id);
        sem_post(contador_barberos);
        sem_post(mutex);
        sleep(3);
    }
}
void ir_a_cortarse_pelo(int id) {
        sleep(2);
        sem_wait(mutex);
        if(esperando< cantidad_sillas){
            esperando++;
            sem_post(contador_clientes);
            sem_post(mutex);
            sem_wait(contador_barberos);
            printf("me cortaron el pelo. atte: %d \n", id);
        }else{
            sem_post(mutex);
        }
}

int main() {
    inicializar_semaforos();
    for (int i = 0; i < cantidad_barberos; i++) {
        pthread_t * barbero = malloc(sizeof(pthread_t));
        pthread_create(barbero, NULL, cortar_pelo, i+1);
        pthread_detach(barbero);
    }
    int id = 0;
    while(1){
        pthread_t * cliente = malloc(sizeof(pthread_t));
        pthread_create(cliente, NULL, ir_a_cortarse_pelo, id+1);
        pthread_detach(cliente);
        id++;
        sleep(1);
    }
}
