//
// Created by hvdc on 4/12/2022.
//

#ifndef ADVENTOFCODE_2022_GENERAL_FUNCTIONS_H
#define ADVENTOFCODE_2022_GENERAL_FUNCTIONS_H
#include <stdio.h>

typedef struct coordinate{
    int x;
    int y;
} Coordinate;

//reads the next character. If this is executed on windows '\r' will be ignored
char next_character(FILE* file);
char skip_next_n_characters(FILE* file, int n);
Coordinate* new_coordinate(int x, int y);
void free_coordinate(Coordinate* coordinate);
int compare_coordinate(void* coo1, void* coo2);
Coordinate add_coordinate(Coordinate first, Coordinate second);

#endif //ADVENTOFCODE_2022_GENERAL_FUNCTIONS_H
