/* File:  
 *    pth_pool.c
 *
 * Purpose:
 *    Implementação de um pool de threads
 *
 *
 * Compile:  gcc -g -Wall -o pth_pool pth_pool.c -lpthread -lrt
 * Usage:    ./pth_hello
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define THREAD_NUM 3   
#define BUFFER_SIZE 10  

typedef struct {
    int clock[THREAD_NUM];
} VectorClock;

VectorClock buffer[BUFFER_SIZE];
int buffer_count = 0;
pthread_mutex_t mutex;
pthread_cond_t can_produce, can_consume;

void print_vector_clock(VectorClock *vc) {
    printf("[");
    for (int i = 0; i < THREAD_NUM; i++) {
        printf("%d", vc->clock[i]);
        if (i < THREAD_NUM - 1) printf(", ");
    }
    printf("]\n");
}

//Produtor
void *producer(void *arg) {
    int id = *(int*)arg;
    VectorClock vc;
    
    while (1) {
        //Fila cheia
        sleep(1); 
        
        //Fila vazia
        //sleep(2); 
        
        for (int i = 0; i < THREAD_NUM; i++) {
            vc.clock[i] = 0;
        }
        vc.clock[id]++;
        
        pthread_mutex_lock(&mutex);
        while (buffer_count == BUFFER_SIZE) {
            pthread_cond_wait(&can_produce, &mutex);
        }
        
        buffer[buffer_count++] = vc;
        printf("Produtor %d produziu: ", id);
        print_vector_clock(&vc);
        
        pthread_cond_signal(&can_consume);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}


//Consumidor
void *consumer(void *arg) {
    int id = *(int*)arg;
    VectorClock vc;
    
    while (1) {
        pthread_mutex_lock(&mutex);
        while (buffer_count == 0) {
            pthread_cond_wait(&can_consume, &mutex);
        }
        
        vc = buffer[--buffer_count];
        printf("Consumidor %d consumiu: ", id);
        print_vector_clock(&vc);
        
        pthread_cond_signal(&can_produce);
        pthread_mutex_unlock(&mutex);
        
        //Fila cheia
        sleep(2);
        
        //Fila vazia
        //sleep(1);
    }
    return NULL;
}


int main() {
    pthread_t producers[THREAD_NUM], consumers[THREAD_NUM];
    int producer_ids[THREAD_NUM], consumer_ids[THREAD_NUM];
    
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&can_produce, NULL);
    pthread_cond_init(&can_consume, NULL);
    
    for (int i = 0; i < THREAD_NUM; i++) {
        producer_ids[i] = i;
        consumer_ids[i] = i;
        pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
        pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]);
    }
    
   
    sleep(30);
    
   
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&can_produce);
    pthread_cond_destroy(&can_consume);
    
    return 0;
}
