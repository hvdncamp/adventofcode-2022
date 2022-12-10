#include <stdio.h>
#include "../lib/general_functions.h"
#include "macros.h"

int tick(int* cycle, int x){
    (*cycle)++;
    if(*cycle % 40 == 0){
        printf("\n");
    }
    if(ABS(*cycle % 40 - x) <= 1){
        printf("#");
    } else {
        printf(".");
    }
    return 0;
}

int main(int argc, char ** argv) {
    FILE * file = fopen(argv[3], "r");
    char read;

    int cycle = -1;
    int x = 1;
    int to_add;

    while((read = fgetc(file)) != EOF){
        switch (read) {
            case 'n':
                skip_next_n_characters(file, 4);//oop\n
                tick(&cycle, x);
                break;
            case 'a':
                fscanf(file, "ddx %d\n", &to_add);
                tick(&cycle, x);
                tick(&cycle, x);
                x +=  to_add;
                break;
            default:
                break;
        }
    }
    return 0;
}
