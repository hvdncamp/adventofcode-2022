#include <stdio.h>
#include <stdlib.h>
#include "../lib/general_functions.h"
#include "../lib/arraylist.h"


int main(int argc, char ** argv) {
    FILE * file = fopen(argv[3], "r");
    int total_visible = 0;
    int numbers[] = {0,1,2,3,4,5,6,7,8,9};
    char read = next_character(file);
    ArrayList* columns = list();
    int column_count = 0;
    int current_column;
    int row_count = 1;
    while(read != '\n'){
        add_list(columns, list()); //nieuwe lijst
        add_list(((ArrayList *)get_list(columns, column_count)), (void*)(numbers + (read - '0')));
        column_count++;
        read = next_character(file);
    }

    read = next_character(file);
    while(read <= '9' && read >= '0'){
        for(current_column = 0; current_column < column_count; current_column++){
            add_list(((ArrayList *)get_list(columns, current_column)), (void*)(numbers + (read - '0')));
            read = next_character(file);
        }
        row_count++;
        read = next_character(file);
    }

    int curr_top_max = 0;
    int curr_bottom_max = 0;

    int* visible[column_count];
    for (int i = 0; i < column_count; ++i) {
        visible[i] = calloc(row_count, sizeof(int));
    }

    for (int i = 0; i < column_count; ++i) {
        curr_top_max = -1;
        curr_bottom_max = -1;
        for (int j = 0; j < row_count; ++j) {
            if((*((int*)get_list(get_list(columns, i), j))) > curr_top_max){
                curr_top_max = *((int*)get_list(get_list(columns, i), j));
                visible[i][j] = 1;
            }
            if((*((int*)get_list(get_list(columns, i), row_count - 1 - j))) > curr_bottom_max){
                curr_bottom_max = *((int*)get_list(get_list(columns, i), row_count - 1 - j));
                visible[i][row_count - 1 - j] = 1;
            }
        }
    }

    int curr_left_max = 0;
    int curr_right_max = 0;

    for (int j = 0; j < row_count; ++j) {
        curr_left_max = -1;
        curr_right_max = -1;
        for (int i = 0; i < column_count; ++i) {
            if((*((int*)get_list(get_list(columns, i), j))) > curr_left_max){
                curr_left_max = *((int*)get_list(get_list(columns, i), j));
                visible[i][j] = 1;
            }
            if((*((int*)get_list(get_list(columns, column_count - 1 - i), j))) > curr_right_max){
                curr_right_max = *((int*)get_list(get_list(columns, column_count - 1 - i), j));
                visible[column_count - 1 - i][j] = 1;
            }
        }
    }

    for(int i=0; i< column_count; i++){
        for(int j=0; j<row_count; j++){
            //fprintf(stderr, "%d ", visible[i][j]);
            if(visible[i][j]){
                total_visible++;
            }
        }
        //fprintf(stderr, "\n");
    }


    printf("The sum is %d\n", total_visible);
    return 0;
}
