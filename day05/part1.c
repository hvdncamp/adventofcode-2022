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

void move(CharList** stacks, int number, int from, int to){
    //TODO
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
                next_character(file); // ']'
                next_character(file); // ' ' or '\n'
                column = (column + 1) % 9;
                break;
            case ' ':
                next_character(file); // ' '
                next_character(file); // ' '
                next_character(file); // ' '
                column = (column + 1) % 9;
                break;
            default:
                break;
        }
    }

    ungetc('m', file);

    int number, from, to;
    while(fscanf(file, "move %d from %d to %d\n", &number, &from, &to) == 3){
        move(stacks, number, from, to);
    }

//    for (int i = 0; i < 9; ++i) {
//        reverse_order(stacks[i], height[i]);
//        for (int j = 0; j < height[i]; ++j) {
//            printf("%c", get_charlist(stacks[i], j));
//        }
//        printf("\n");
//    }

    for (int i = 0; i < 9; ++i) {
        printf("%c", get_charlist(stacks[i], height[i] - 1));
    }
    printf("\n");
    return 0;
}