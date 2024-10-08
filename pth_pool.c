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


VectorClock produce_clock(int id) {
    VectorClock vc;
    for (int i = 0; i < THREAD_NUM; i++) {
        vc.clock[i] = 0;
    }
    vc.clock[id]++;
    return vc;
}


void add_to_buffer(VectorClock vc, int id) {
    pthread_mutex_lock(&mutex);
    while (buffer_count == BUFFER_SIZE) {
        printf("\nFila cheia\n");
        pthread_cond_wait(&can_produce, &mutex);
    }
    
    buffer[buffer_count++] = vc;
    printf("Produtor %d produziu: ", id);
    print_vector_clock(&vc);
    
    pthread_cond_signal(&can_consume);
    pthread_mutex_unlock(&mutex);
}


VectorClock remove_from_buffer(int id) {
    VectorClock vc;
    pthread_mutex_lock(&mutex);
    while (buffer_count == 0) {
        printf("\nFila vazia\n");
        pthread_cond_wait(&can_consume, &mutex);
    }
    
    vc = buffer[--buffer_count];
    printf("Consumidor %d consumiu: ", id);
    print_vector_clock(&vc);
    
    pthread_cond_signal(&can_produce);
    pthread_mutex_unlock(&mutex);
    return vc;
}

// Produtor
void *producer(void *arg) {
    int id = *(int*)arg;
    while (1) {
        
        sleep(1); // Fila cheia
        // sleep(2); // Fila vazia
        
        VectorClock vc = produce_clock(id);
        add_to_buffer(vc, id);
    }
    return NULL;
}

// Consumidor
void *consumer(void *arg) {
    int id = *(int*)arg;
    while (1) {
        
        remove_from_buffer(id);
        
        sleep(2); // Fila cheia
        // sleep(1); // Fila vazia
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
