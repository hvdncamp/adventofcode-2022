//
// Created by hvdc on 4/12/2022.
//
#include "general_functions.h"
#include "macros.h"

char next_character(FILE* file){
    char read = fgetc(file);
    if(PLATFORM_NAME == 'W'){
        if(read == '\t'){
            return fgetc(file);
        } else {
            return read;
        }
    } else {
        return read;
    }
}