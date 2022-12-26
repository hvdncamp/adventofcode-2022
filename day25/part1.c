#include <stdio.h>
#include <stdlib.h>
#include "general_functions.h"
#include "arraylist.h"

long power(int base, int exponent){
    long number = 1;
    for (int _ = 0; _ < exponent; _++){
        number *= base;
    }
    return number;
}

long snafu_to_int(char* snafu){
    int charactercount = 0;
    while(snafu[charactercount] != '\0'){
        charactercount++;
    }

    long number = 0;

    for (int i = 0; i < charactercount; ++i) {
        switch (snafu[i]) {
            case '-':
                number -= power(5, charactercount - 1 - i);
                break;
            case '=':
                number -= 2 * power(5, charactercount - 1 - i);
                break;
            default:
                number += (snafu[i] - '0') * power(5, charactercount - 1 - i);
                break;
        }
    }
    return number;
}

void int_to_snafu(long integer, char* snafu){
    char characters[5] = { '0', '1', '2', '-', '='};
    ArrayList* to_add = list();

    while(integer > 0){
        switch (integer % 5) {
            case 3:
                add_list(to_add, characters + 4);
                integer = (integer + 2) / 5;
                break;
            case 4:
                add_list(to_add, characters + 3);
                integer = (integer + 1) / 5;
                break;
            default:
                add_list(to_add, characters + (integer % 5));
                integer = integer / 5;
                break;
        }
    }

    for (int i = 0; i < to_add->size; ++i) {
        snafu[i] = *(char*)(get_list(to_add, to_add->size - 1 - i));
    }
    snafu[to_add->size] = '\0';
}

int main(int argc, char ** argv) {
    FILE *file = fopen(argv[3], "r");

    long sum = 0;
    char digit;
    char snafu[40];
    while ((digit=next_character(file))!= EOF){
        ungetc(digit, file);
        fscanf(file, "%s ", snafu);
        sum += snafu_to_int(snafu);
    }
    printf("%ld\n", sum);

    int_to_snafu(sum, snafu);
    printf("%ld\n", snafu_to_int(snafu));
    printf("%s\n", snafu);

    fclose(file);
}