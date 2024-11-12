#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 10

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
int prod_count = 0;

pthread_mutex_t mutex;
pthread_cond_t not_empty;
pthread_cond_t not_full;

void* producer(void* args) {
    for (int i = 1; i <= 10; i++) {
        int id = *((int*)args);
        
        pthread_mutex_lock(&mutex);
        
        
        while ((in + 1) % BUFFER_SIZE == out) {
            pthread_cond_wait(&not_full, &mutex);
        }
        
       
        buffer[in] = i + (id * 5);
        printf("Produced: %d at index %d\n", buffer[in], in);
        in = (in + 1) % BUFFER_SIZE;
        prod_count++;
        
      
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&mutex);
        sleep(1); 
    }
    
    return NULL;
}

void* consumer(void* args) {
    for (int i = 0; i < 20; i++) { 
        pthread_mutex_lock(&mutex);

        
        while (in == out) {
            pthread_cond_wait(&not_empty, &mutex);
        }

        
        int item = buffer[out];
        printf("Consumer: %d from index %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;
        
        
        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t prod1, prod2, cons;
    int* prod_id1 = (int*)malloc(sizeof(int));
    int* prod_id2 = (int*)malloc(sizeof(int));
    *prod_id1 = 0;
    *prod_id2 = 1;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);

    
    pthread_create(&prod1, NULL, producer, (void*)prod_id1);
    pthread_create(&prod2, NULL, producer, (void*)prod_id2);
    pthread_create(&cons, NULL, consumer, NULL);

    
    pthread_join(prod1, NULL);
    pthread_join(prod2, NULL);
    pthread_join(cons, NULL);

    
    free(prod_id1);
    free(prod_id2);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);

    return 0;
}
