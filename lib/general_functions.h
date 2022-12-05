//
// Created by hvdc on 4/12/2022.
//

#ifndef ADVENTOFCODE_2022_GENERAL_FUNCTIONS_H
#define ADVENTOFCODE_2022_GENERAL_FUNCTIONS_H
#include <stdio.h>

//reads the next character. If this is executed on windows '\r' will be ignored
char next_character(FILE* file);
char skip_next_n_characters(FILE* file, int n);

#endif //ADVENTOFCODE_2022_GENERAL_FUNCTIONS_H
