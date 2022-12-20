#include <stdio.h>
#include <stdlib.h>
#include "general_functions.h"
#include "macros.h"
#include "arraylist.h"

enum STATE {EMPTY = 0, WALL, SAND};

void print_grid(int** grid, int xmax, int ymax){
    for (int r = 0; r < ymax ; ++r) {
        for (int c = 0; c < xmax ; ++c) {
            switch (grid[c][r]) {
                case EMPTY:
                    fprintf(stderr, ".");
                    break;
                case WALL:
                    fprintf(stderr, "#");
                    break;
                case SAND:
                    fprintf(stderr, "o");
                    break;
            }
        }
        fprintf(stderr, "\n");
    }
}

Coordinate* falls_to(Coordinate* from, int** grid, int gridx, int gridy){
    if(from->y + 1 >= gridy){
        return NULL;
    }
    if(grid[from->x][from->y + 1] == EMPTY){
        return (Coordinate*) new_coordinate(from->x, from->y + 1);
    } else if ((from->x - 1 >= 0) && grid[from->x - 1][from->y + 1] == EMPTY){
        return (Coordinate*) new_coordinate(from->x - 1, from->y + 1);
    } else if ((from->x + 1 < gridx) && grid[from->x + 1][from->y + 1] == EMPTY){
        return (Coordinate*) new_coordinate(from->x + 1, from->y + 1);
    } else if( from->x - 1 < 0 || from->x + 1 >= gridx) {
        return NULL;
    } else {
        return from;
    }
}

int fall(ArrayList* sand_stack, int** grid, int gridx, int gridy){
    free(pop_list(sand_stack));
    Coordinate * previous_position = (Coordinate*)peek_list(sand_stack);
    Coordinate* new_position;
    while((new_position = falls_to(previous_position, grid, gridx, gridy)) !=NULL && new_position != previous_position){
        add_list(sand_stack, new_position);
        previous_position = new_position;
    }
    if(new_position == previous_position){
        grid[previous_position->x][previous_position->y] = SAND;
        return 1;
    } else {
        return 0;
    }
}

int main(int argc, char ** argv) {
    FILE *file = fopen(argv[3], "r");

    int x;
    int y;
    int xmax = 500;
    int ymax = 0;
    char next;
    while(fscanf(file, "%d,%d", &x, &y) == 2){
        if(x > xmax){
            xmax = x;
        }
        if(y > ymax){
            ymax = y;
        }
        next = next_character(file);
        while(next != '\n' && (next > '9' || next < '0')){
            next = next_character(file);
        }
        if(next != '\n'){
            ungetc(next, file);
        }
    }
    fclose(file);

    file = fopen(argv[3], "r");
    int* grid[2*xmax + 1];
    for (int i = 0; i < 2*xmax + 1; ++i) {
        grid[i] = calloc(ymax + 3, sizeof(int));
        grid[i][ymax + 2] = WALL;
    }
    int prev_x, prev_y;

    next = next_character(file);
    while(next >= '0' && next <= '9'){
        ungetc(next, file);
        fscanf(file, "%d,%d ", &x, &y);
        prev_x = x;
        prev_y = y;
        next = next_character(file);
        while(next == '-'){
            fscanf(file, "> %d,%d ", &x, &y);
            for (int i = MIN(prev_x, x); i <= MAX(prev_x, x); ++i) {
                for (int j = MIN(prev_y, y); j <= MAX(prev_y, y); ++j) {
                    grid[i][j] = WALL;
                }
            }
            prev_x = x;
            prev_y = y;
            next = next_character(file);
        }
        //now next is \n
    }

    int sand_fallen = 0;
    ArrayList* sand_stack = list();
    add_list(sand_stack, (Coordinate*) new_coordinate(500, 0));
    add_list(sand_stack, (Coordinate*) new_coordinate(500, 0));

    while (fall(sand_stack, grid, 2*xmax + 1, ymax + 3) && grid[500][0] != SAND){
        sand_fallen++;
    }
    sand_fallen++;
    print_grid((int **) grid, 2*xmax + 1, ymax + 3);

    printf("%d\n", sand_fallen );
    fclose(file);
}
