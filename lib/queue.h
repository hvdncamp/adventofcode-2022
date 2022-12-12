//
// Created by hvdncamp on 12/12/2022.
//

#ifndef ADVENTOFCODE_2022_QUEUE_H
#define ADVENTOFCODE_2022_QUEUE_H

typedef struct queue {
    void** items;
    int start;
    int size;
    int capacity;
} Queue;

Queue* new_queue();
void queue_add(Queue* queue, void* item);
void* queue_pop(Queue* queue);
void free_queue(Queue* queue, int also_elements);

#endif //ADVENTOFCODE_2022_QUEUE_H
