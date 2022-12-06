#include <stdio.h>
#include "../lib/general_functions.h"

#define PACKET_LENGTH 14
#define INCREMENT_MOD(x) (((x) + 1) % PACKET_LENGTH)
#define DECREMENT_MOD(x) (((x) + PACKET_LENGTH - 1) % PACKET_LENGTH)

int find_last_equal_index(char* chars, char new, int start, int end){
    int index_equal = -1;
    for (int i = start; i != INCREMENT_MOD(end); i = INCREMENT_MOD(i)) {
        if(chars[i]==new){
            index_equal = i;
        }
    }
    return index_equal;
}

int main(int argc, char ** argv) {
    FILE * file = fopen(argv[3], "r");
    char last_chars[PACKET_LENGTH];
    last_chars[0] = next_character(file);
    int start = 0;
    int end = 0;
    int number_processed = 1;
    int last_equal_index;
    int done=0;
    while(!done && last_chars[end] != EOF){
        end = INCREMENT_MOD(end);
        last_chars[end] = next_character(file);
        number_processed++;
        if((last_equal_index = find_last_equal_index(last_chars, last_chars[end], start, DECREMENT_MOD(end))) == -1){
            //different from all previous
            if(INCREMENT_MOD(end) == start){
                done = 1;
            }
        } else {
            start = INCREMENT_MOD(last_equal_index);
        }
    }
    printf("After %d characters\n", number_processed);
    return 0;
}
