#include <stdio.h>
#include <stdlib.h>
#include "general_functions.h"

enum STATE {EMPTY = 0, WALL, SAND};

int main(int argc, char ** argv) {
    FILE *file = fopen(argv[3], "r");

    int x;
    int y;
    int xmax = 500;
    int xmin = 500;
    int ymax = 0;
    int ymin = 0;
    char next;
    while(fscanf(file, "%d,%d", &x, &y) == 2){
        if(x > xmax){
            xmax = x;
        } else if (x < xmin){
            xmin = x;
        }
        if(y > ymax){
            ymax = y;
        } else if (y < ymin){
            ymin = y;
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
    fprintf(stderr, "%d,%d,%d,%d\n", xmin, xmax, ymin, ymax);

    file = fopen(argv[3], "r");
    int grid[xmax - xmin][ymax + 1]; //herschaald
    int start_x = 500 - xmin;
    int prev_x, prev_y;

    next = next_character(file);
    while(next >= '0' && next <= '9'){
        ungetc(next, file);
        fscanf(file, "%d,%d", &x, &y);
        prev_x = x;
        prev_y = y;
        next = next_character(file);
        while(next == ' '){
            fscanf(file, "-> %d,%d", &x, &y);
            for (int i = prev_x - xmin; i <= x - xmin; ++i) {
                for (int j = prev_y; j <= y; ++j) {
                    grid[i][j] = WALL;
                }
            }
            next = next_character(file);
        }
        //now next is \n

        next = next_character(file);
    }

    fclose(file);

}
