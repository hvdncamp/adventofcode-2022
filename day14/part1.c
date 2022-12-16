#include <stdio.h>
#include <stdlib.h>
#include "arraylist.h"



int main(int argc, char ** argv) {
    FILE *file = fopen(argv[3], "r");

    fclose(file);
}
