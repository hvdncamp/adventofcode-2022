//
// Created by hvdc on 4/12/2022.
//
#include <stdlib.h>
#include "general_functions.h"
#include "macros.h"

char next_character(FILE* file){
    char read = fgetc(file);
    if(PLATFORM_NAME == 'W'){
        if(read == '\t'){
            return fgetc(file);
        } else {
            return read;
        }
    } else {
        return read;
    }
}

char skip_next_n_characters(FILE* file, int n){
    for (int i = 0; i < n; ++i) {
        next_character(file);
    }
}

Coordinate* new_coordinate(int x, int y) {
    Coordinate *coo = malloc(sizeof(Coordinate));
    coo->x = x;
    coo->y = y;
    return coo;
}

void free_coordinate(Coordinate* coordinate){
    free(coordinate);
}

int compare_coordinate(void* coo1, void* coo2){
    int xdiff = ((Coordinate*)coo1)->x - ((Coordinate*)coo2)->x;
    return (xdiff != 0 ? xdiff : ((Coordinate*)coo1)->y - ((Coordinate*)coo2)->y);
}

Coordinate add_coordinate(Coordinate first, Coordinate second){
    return (Coordinate){first.x + second.x, first.y + second.y};
}