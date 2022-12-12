//
// Created by hvdncamp on 12/12/2022.
//
#include <stdlib.h>
#include <stdio.h>


#include "queue.h"

#define INITIAL_QUEUE_SIZE 50

Queue* new_queue(){
    Queue* queue = malloc(sizeof(Queue));
    queue->items = malloc(sizeof(void*) * INITIAL_QUEUE_SIZE);
    queue->size = 0;
    queue->capacity = INITIAL_QUEUE_SIZE;
    queue->start = 0;
}

void queue_add(Queue* queue, void* item){
    if(item == NULL){
        fprintf(stderr, "You cannot add NULL to the queue\n");
        exit(1);
    }
    if(queue->size == queue->capacity) {
        queue->items = (void**)realloc(queue->items, sizeof(void*) * queue->capacity * 2);
        for (int i = 0; i < queue->start; ++i) {
            queue->items[queue->capacity + i] = queue->items[i];
        }
        queue->capacity *= 2;
    }
    queue->items[(queue->start + queue->size) % queue->capacity] = item;
    queue->size++;
}
void* queue_pop(Queue* queue){
    if(queue->size == 0){
        return NULL;
    }
    void* item = queue->items[queue->start];
    queue->start = (queue->start + 1) % queue->capacity;
    queue->size--;
    return item;
}
void free_queue(Queue* queue, int also_elements){
    if(also_elements){
        for (int i = queue->start; i !=  (queue->start + queue->size) % queue->capacity; i = (i + 1) % queue->capacity) {
            free(queue->items[i]);
        }
    }
    free(queue->items);
    free(queue);
}
