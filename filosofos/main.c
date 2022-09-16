#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>


int cantidad_filosofos = 10;
pthread_mutex_t * mutex;
sem_t** filosofos;
enum estado{PENSANDO=0, HAMBRIENTO=1, COMIENDO=2};
int* estados;
void inicializar_semaforos () {
    mutex = malloc(sizeof(pthread_mutex_t));
    sem_init(mutex, 0, 1);
    filosofos = malloc(sizeof(sem_t)*cantidad_filosofos);
    for(int i=0; i<cantidad_filosofos; i++){
        filosofos[i] = malloc(sizeof(sem_t));
        sem_init(filosofos[i], 0, 0);
    }

}
void test(int id){
    if(estados[id] == HAMBRIENTO && estados[(id+1)%cantidad_filosofos] != COMIENDO && estados[(id-1)%cantidad_filosofos] != COMIENDO){
        estados[id] = COMIENDO;
        printf("Filosofo %d esta comiendo \n", id);
        sem_post(filosofos[id]);
    }
}
void tomar_tenedores(int id){
    sem_wait(mutex);
    estados[id] = HAMBRIENTO;
    printf("Filosofo %d esta hambriento \n", id);
    test(id);
    sem_post(mutex);
    sem_wait(filosofos[id]);
}
void dejar_tenedores(int id){
    sem_wait(mutex);
    estados[id] = PENSANDO;
    printf("Filosofo %d esta pensando \n", id);
    test((id+1)%cantidad_filosofos);
    test((id-1)%cantidad_filosofos);
    sem_post(mutex);
}


void accion(int id) {
    while(1){
        sleep(1); //pensar
        tomar_tenedores(id);
        sleep(1); //comer
        dejar_tenedores(id);
    }
}
void crear_hilos () {
    for(int i = 0; i<cantidad_filosofos; i++){
        pthread_t * filosofo = malloc(sizeof(pthread_t));
        pthread_create(filosofo, NULL, accion, i);
        pthread_detach(filosofo);
    }
}
int main() {
    inicializar_semaforos();
    estados = malloc(sizeof(int) * cantidad_filosofos);
    crear_hilos();
    while(1){
        sleep(1);
    }
}
