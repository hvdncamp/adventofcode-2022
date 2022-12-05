#include <stdio.h>
#include "../lib/general_functions.h"
#include "../lib/arraylist.h"

void reverse_order(CharList* stack, int height){
    char temp;
    for (int i = 0; i < height/2; ++i) {
        temp = get_charlist(stack, i);
        set_charlist(stack, i, get_charlist(stack, height - 1 - i));
        set_charlist(stack, height - 1 - i, temp);
    }
}

void move_crates(CharList** stacks, int* heights, int number, int from, int to){
    for (int i = 0; i < number; ++i) {
        add_charlist(stacks[to], get_charlist(stacks[from], heights[from] - number + i));
    }
    for (int i = 0; i < number; ++i) {
        pop_charlist(stacks[from]);
    }
    heights[from] -= number;
    heights[to] += number;
}

int main(int argc, char ** argv) {
    FILE * file = fopen(argv[3], "r");
    char read;
    int column;
    CharList * stacks[9];
    int height[9];
    for (int i = 0; i < 9; ++i) {
        stacks[i] = charlist();
        height[i] = 0;
    }

    while((read = next_character(file)) != 'm'){
        switch (read) {
            case '[':
                add_charlist(stacks[column], next_character(file));
                height[column]++;
                skip_next_n_characters(file, 2); //skip ']' and (' ' or '\n')
                column = (column + 1) % 9;
                break;
            case ' ':
                skip_next_n_characters(file, 3);
                column = (column + 1) % 9;
                break;
            default:
                break;
        }
    }
    ungetc('m', file);

    for (int i = 0; i < 9; ++i) {
        reverse_order(stacks[i], height[i]);
    }

    int number, from, to;
    while(fscanf(file, "move %d from %d to %d\n", &number, &from, &to) == 3){
        move_crates(stacks, height, number, from - 1, to - 1);
    }

    for (int i = 0; i < 9; ++i) {
        printf("%c", get_charlist(stacks[i], height[i] - 1));
        free_charlist(stacks[i]);
    }
    printf("\n");
    return 0;
}