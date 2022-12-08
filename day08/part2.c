#include <stdio.h>
#include <stdlib.h>
#include "../lib/general_functions.h"
#include "../lib/arraylist.h"

int get_tree(ArrayList* trees, int x, int y){
    return *((int*)get_list(get_list(trees, x), y));
}

int scenic_score(ArrayList* trees, int x, int y, int row_count, int column_count){
    int score = 1;
    int distance = 1;
    int this_tree = get_tree(trees, x, y);
    while((x + distance < column_count - 1) && get_tree(trees, x + distance, y) < this_tree){
        distance++;
    }
    score *= distance;
    distance = 1;
    while((x - distance >= 1) && get_tree(trees, x - distance, y) < this_tree){
        distance++;
    }
    score *= distance;
    distance = 1;
    while((y + distance < row_count - 1) && get_tree(trees, x, y + distance) < this_tree){
        distance++;
    }
    score *= distance;
    distance = 1;
    while((y - distance >= 1) && get_tree(trees, x , y - distance) < this_tree){
        distance++;
    }
    score *= distance;
    return score;
}

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


    int highest_score = 0;
    int curr_score;
    for(int i=1; i< column_count - 1; i++){
        for(int j=1; j<row_count - 1; j++){
            //fprintf(stderr, "%d ", visible[i][j]);
            if((curr_score = scenic_score(columns, i, j, row_count, column_count)) > highest_score){
                highest_score = curr_score;
            }
        }
        //fprintf(stderr, "\n");
    }


    printf("The product is %d\n", highest_score);
    return 0;
}
