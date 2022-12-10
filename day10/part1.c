#include <stdio.h>
#include "../lib/general_functions.h"

int tick(int* cycle, int x){
    (*cycle)++;
    if(*cycle % 40 == 20){
        return x * (*cycle);
    }
    return 0;
}

int main(int argc, char ** argv) {
    FILE * file = fopen(argv[3], "r");
    char read;

    int cycle = 0;
    int x = 1;
    int to_add;
    int total_sum = 0;

    while((read = fgetc(file)) != EOF){
        switch (read) {
            case 'n':
                skip_next_n_characters(file, 4);//oop\n
                total_sum += tick(&cycle, x);
                break;
            case 'a':
                fscanf(file, "ddx %d\n", &to_add);
                total_sum += tick(&cycle, x);
                total_sum += tick(&cycle, x);
                x +=  to_add;
                break;
            default:
                break;
        }
    }
    printf("The sum is %d\n", total_sum);
    return 0;
}
