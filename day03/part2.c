#include <stdio.h>
#include "../lib/general_functions.h"

int priority(char c) {
    if (('A' <= c) && (c <= 'Z')) {
        return c - 'A' + 26;
    } else {
        return c - 'a';
    }
}

void initialize_occurrences(int * occurrence) {
    int i;
    for (i = 0; i < 52; i++) {
        occurrence[i] = 0;
    }
}

int main(int argc, char ** argv) {
    FILE * file = fopen(argv[3], "r");
    char item;
    int carried_by_elf1[52];
    int carried_by_elf2[52];
    int elf_number = 1;
    int priority_sum = 0;

    initialize_occurrences(carried_by_elf1);
    initialize_occurrences(carried_by_elf2);
    while ((item = next_character(file)) != EOF) {
        if (item == '\n') {
            elf_number = (elf_number % 3) + 1;
        } else {
            switch (elf_number) {
                case 1:
                    carried_by_elf1[priority(item)] = 1;
                    break;
                case 2:
                    carried_by_elf2[priority(item)] = 1;
                    break;
                case 3:
                    if(carried_by_elf1[priority(item)] && carried_by_elf2[priority(item)]){
                        //dit zou maar 1 keer mogen gebeuren per groepje
                        priority_sum += (priority(item) + 1); //score bijtellen
                        initialize_occurrences(carried_by_elf1); //resetten, zodat het item maar een keer gevonden wordt
                        initialize_occurrences(carried_by_elf2);
                    }
                    break;
                default:
                    break;
            }
        }
    }
    printf("%d\n", priority_sum);
    return 0;
}
