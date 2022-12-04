#include <stdio.h>
#include "../lib/general_functions.h"

int main(int argc, char ** argv) {
    FILE * file = fopen(argv[3], "r");
    int start1, end1, start2, end2;
    int pair_count = 0;
    while(fscanf(file, "%d-%d,%d-%d",&start1, &end1, &start2, &end2) == 4){
        next_character(file);//newline
        if(!(start1 > end2 || start2 > end1)) {
            pair_count++;
        }
    }
    printf("There are %d such pairs\n", pair_count);
    return 0;
}
