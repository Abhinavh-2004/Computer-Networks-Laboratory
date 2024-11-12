#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];
int in = 0; // Index for the next produced item
int out = 0; // Index for the next consumed item

void* producer(void* arg) {
    for (int i = 0; i < 10; i++) {
        // Produce an item
        buffer[in] = i;
        printf("Produced: %d at index %d\n", buffer[in], in);
        in = (in + 1) % BUFFER_SIZE; // Circular buffer

        // Simulate time taken to produce
        sleep(1);
    }
    return NULL;
}

void* consumer(void* arg) {
    for (int i = 0; i < 10; i++) {
        // Consume an item
        int item = buffer[out];
        printf("Consumed: %d from index %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE; // Circular buffer

        // Simulate time taken to consume
        sleep(1);
    }
    return NULL;
}

int main() {
    pthread_t prod, cons;

    // Create producer and consumer threads
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // Wait for threads to finish
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    return 0;
}
