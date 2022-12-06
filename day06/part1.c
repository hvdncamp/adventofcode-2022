#include <stdio.h>
#include "../lib/general_functions.h"

int are_different(char* chars, int number){
    for (int i = 0; i < number - 1; ++i) {
        for(int j = i+1; j < number;j++){
            if(chars[i] == chars[j]){
                return 0;
            }
        }
    }
    return 1;
}

int main(int argc, char ** argv) {
    FILE * file = fopen(argv[3], "r");
    char last_chars[4];
    for (int i = 0; i < 4; ++i) {
        last_chars[i] = next_character(file);
    }
    int next_index = 0;
    int number_processed = 4;
    while(!are_different(last_chars, 4)){
        last_chars[next_index] = next_character(file);
        next_index = (next_index + 1) % 4;
        number_processed++;
    }
    printf("After %d characters\n", number_processed);
    return 0;
}
