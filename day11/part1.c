#include <stdio.h>
#include <stdlib.h>
#include "arraylist.h"

#define NR_OF_ROUNDS 20

enum operator { ADD, MUL };

typedef struct monkey {
    ArrayList * items;
    int left_operand;
    enum operator operator;
    int right_operand;
    int divisor;
    int monkeytrue;
    int monkeyfalse;
} Monkey;

Monkey * read_monkey(FILE *file) {
    int temp;
    char lookahead;
    if(fscanf(file," Monkey %d:\n", &temp)!= 1){
        return NULL;
    }

    Monkey * monkey = malloc(sizeof(Monkey));
    monkey->items = list();

    fscanf(file,"Starting items: ");
    while(fscanf(file, "%d", &temp) == 1){
        add_list(monkey->items, (void*)(size_t)temp);
        lookahead = fgetc(file);
        if (lookahead != ',') break;
    }

    fscanf(file," Operation: new = ");
    if (fscanf(file, "%d ", &temp) == 1) {
        monkey->left_operand = temp;
    } else {
        monkey->left_operand = 0;
    }
    while (fgetc(file) != ' ');
    if (fgetc(file) == '+') {
        monkey->operator = ADD;
    } else {
        monkey->operator = MUL;
    }
    if (fscanf(file, "%d", &temp) == 1) {
        monkey->right_operand = temp;
    } else {
        monkey->right_operand = 0;
    }
    while (fgetc(file) != ' ');

    fscanf(file," Test: divisible by %d", &temp);
    monkey->divisor = temp;

    fscanf(file," If true: throw to monkey %d", &temp);
    monkey->monkeytrue = temp;

    fscanf(file," If false: throw to monkey %d", &temp);
    monkey->monkeyfalse = temp;

    return monkey;
}

void free_monkey(Monkey* monkey){
    free_list(monkey->items);
    free(monkey);
}

void * free_listmonkey(void *acc, int index, void *item) {
    free_monkey((Monkey*)item);
    return NULL;
}

void print_monkey(Monkey * monkey, int index) {
    fprintf(stderr, "Monkey %d", index);
    fprintf(stderr, "\n  Starting items:");
    for (int i = 0; i < monkey->items->size; i++) {
        fprintf(stderr, " %d", (int)(size_t)get_list(monkey->items, i));
    }
    fprintf(stderr, "\n  Operation: new = %d %c %d", monkey->left_operand, monkey->operator == ADD ? '+' : '*', monkey->right_operand);
    fprintf(stderr, "\n  Test: divisible by %d", monkey->divisor);
    fprintf(stderr, "\n    If true: throw to monkey %d", monkey->monkeytrue);
    fprintf(stderr, "\n    If false: throw to monkey %d\n\n", monkey->monkeyfalse);
}

void inspect(Monkey* monkey, int* worry_level){
    switch (monkey->operator) {
        case ADD:
            if(monkey->left_operand){
                fprintf(stderr, "A left operand is not 'old'\n");
                exit(1);
            }
            if(monkey->right_operand){
                (*worry_level) += monkey->right_operand;
            } else {
                (*worry_level) *= 2;
            }
            break;
        case MUL:
            if(monkey->left_operand){
                fprintf(stderr, "A left operand is not 'old'\n");
                exit(1);
            }
            if(monkey->right_operand){
                (*worry_level) *= monkey->right_operand;
            } else {
                (*worry_level) *= *worry_level;
            }
            break;
        default:
            fprintf(stderr, "operand type should be ADD or MUL\n");
            exit(1);
    }
    (*worry_level) = (*worry_level) / 3;
}

int main(int argc, char ** argv) {
    FILE *file = fopen(argv[3], "r");
    ArrayList *monkeys = list();
    Monkey* monkey_read;
    int worry_level;
    //input
    while((monkey_read = read_monkey(file)) != NULL){
        add_list(monkeys, (void*)monkey_read);
    }

    //calculation
    int inspection_count[10] = { 0 };
    Monkey* current_monkey;
    for (int round = 0; round < NR_OF_ROUNDS; ++round) {
        for (int monkey_nr = 0; monkey_nr < monkeys->size; ++monkey_nr) {
            current_monkey = (Monkey*)get_list(monkeys, monkey_nr);
            for(int item_index = 0; item_index < ((Monkey*)get_list(monkeys, monkey_nr))->items->size; item_index++){
                worry_level = (int)(size_t)get_list(current_monkey->items, item_index);
                inspect(current_monkey, &worry_level);
                inspection_count[monkey_nr]++;
                if(worry_level % (current_monkey)->divisor == 0){
                    add_list(((Monkey*)get_list(monkeys, (current_monkey)->monkeytrue))->items, (void*)(size_t)worry_level);
                } else {
                    add_list(((Monkey*)get_list(monkeys, (current_monkey)->monkeyfalse))->items, (void*)(size_t)worry_level);
                }
            }
            clear_list(current_monkey->items);
        }
    }

    int max_inspection_count = 0;
    int max_inspection_count2 = 0;
    for (int i = 0; i < monkeys->size; ++i) {
        if(inspection_count[i] > max_inspection_count){
            max_inspection_count2 = max_inspection_count;
            max_inspection_count = inspection_count[i];
        } else if (inspection_count[i] > max_inspection_count2){
            max_inspection_count2 = inspection_count[i];
        }
    }

    printf("The level of monkey business is %d\n", max_inspection_count * max_inspection_count2);

    //free monkeys
    fold_list(monkeys, NULL, free_listmonkey);
    fclose(file);
}
