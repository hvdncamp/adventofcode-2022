#include <stdio.h>
#include <stdlib.h>
#include "../lib/queue.h"
#include "../lib/general_functions.h"
#include "arraylist.h"

#define NORTH (Coordinate){0,1}
#define EAST (Coordinate){1,0}
#define SOUTH (Coordinate){0,-1}
#define WEST (Coordinate){-1,0}

typedef struct directed_edge{
    Coordinate origin;
    Coordinate direction;
} Directed_edge;

Directed_edge* edge_from(Coordinate origin, Coordinate direction){
    Directed_edge * edge = malloc(sizeof(Directed_edge));
    edge->origin = origin;
    edge->direction = direction;
    return edge;
}

ArrayList* read_grid(FILE* file, int* line_count, int* column_count, Coordinate* start, Coordinate* end){
    ArrayList* heights = list();
    char read;
    *column_count = 0;
    *line_count = 1;
    while ((read = next_character(file)) != '\n'){
        add_list(heights, (void*)list());
        if(read == 'S'){
            start->x = *column_count;
            start->y = *line_count - 1;
            read = 'a';
        } else if (read == 'E'){
            end->x = *column_count;
            end->y = *line_count - 1;
            read = 'z';
        }
        add_list((ArrayList *)get_list(heights, *column_count), (void*)(size_t)(read - 'a'));
        (*column_count)++;
    }
    *column_count = 0;
    int start_new_line = 1;

    while((read = next_character(file)) != EOF){
        if(read == '\n') {
            start_new_line = 1;
        } else if((read >= 'a' && read <= 'z') || read == 'S' || read == 'E'){
            if(start_new_line){
                (*line_count)++;
                *column_count = 0;
                start_new_line = 0;
            }
            if(read == 'S'){
                start->x = *column_count;
                start->y = *line_count - 1;
                read = 'a';
            } else if (read == 'E'){
                end->x = *column_count;
                end->y = *line_count - 1;
                read = 'z';
            }
            add_list((ArrayList *)get_list(heights, *column_count), (void*)(size_t)(read - 'a'));
            (*column_count)++;
        }//else ignore
    }
    return heights;
}

int get_height(ArrayList* heights, Coordinate* coo){
    return (int)(size_t)get_list((ArrayList*) get_list(heights, coo->x), coo->y);
}

int shortest_path(Coordinate start, ArrayList* heights, int** distance, Coordinate destination, int column_count, int line_count){
    for (int i = 0; i < column_count; ++i) {
        for (int j = 0; j < line_count; ++j) {
            distance[i][j] = -1;
        }
    }

    Queue* edge_queue = new_queue();
    queue_add(edge_queue, edge_from(start, NORTH));
    queue_add(edge_queue, edge_from(start, SOUTH));
    queue_add(edge_queue, edge_from(start, EAST));
    queue_add(edge_queue, edge_from(start, WEST));

    distance[start.x][start.y] = 0;

    Directed_edge* next;
    Coordinate next_coo;
    while((next = (Directed_edge *)queue_pop(edge_queue)) != NULL) {
        next_coo = add_coordinate(next->direction, next->origin);
        if ((next_coo.x < column_count && next_coo.y < line_count && next_coo.x >= 0 && next_coo.y >= 0)
            && (get_height(heights, &next_coo) <= (get_height(heights, &next->origin) + 1))) {
            if (next_coo.x == destination.x && next_coo.y == destination.y) {
                free_queue(edge_queue, 1);
                return distance[next->origin.x][next->origin.y] + 1;
            }
            if (distance[next_coo.x][next_coo.y] == -1) {//unvisited
                distance[next_coo.x][next_coo.y] = distance[next->origin.x][next->origin.y] + 1;
                if (next->direction.x == 0) {
                    queue_add(edge_queue, edge_from(next_coo, (Coordinate) {0, next->direction.y}));
                    queue_add(edge_queue, edge_from(next_coo, EAST));
                    queue_add(edge_queue, edge_from(next_coo, WEST));
                } else {
                    queue_add(edge_queue, edge_from(next_coo, (Coordinate) {next->direction.x, 0}));
                    queue_add(edge_queue, edge_from(next_coo, NORTH));
                    queue_add(edge_queue, edge_from(next_coo, SOUTH));
                }
            }
        }
    }
    return column_count * line_count;
}

int main(int argc, char ** argv) {
    FILE *file = fopen(argv[3], "r");
    int line_count;
    int column_count;
    Coordinate start;
    Coordinate destination;

    ArrayList * heights = read_grid(file, &line_count, &column_count, &start, &destination);

    int* distance[column_count];
    for (int i = 0; i < column_count; ++i) {
        distance[i] = malloc(line_count * sizeof(int));
    }

    int scenic_length = 408;//antwoord deel 1
    int temp;
    for (int i = 0; i < column_count; ++i) {
        for (int j = 0; j < line_count; ++j) {
            if(get_height(heights, &(Coordinate){i,j}) == 0) {
                temp = shortest_path((Coordinate) {i, j}, heights, distance, destination, column_count, line_count);
                if (temp < scenic_length) {
                    scenic_length = temp;
                }
            }
        }
    }

    printf("%d\n", scenic_length);
    fclose(file);
}
