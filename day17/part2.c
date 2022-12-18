#include <stdio.h>
#include <stdlib.h>
#include "arraylist.h"
#include "general_functions.h"
#include "macros.h"

#define NR_OF_ROCKS 5
#define CHAMBER_WIDTH 7
#define WINDOW_HEIGHT 100
#define TOTAL_ROCKS 1000000

int chamber[CHAMBER_WIDTH][WINDOW_HEIGHT];
ArrayList* rocks[NR_OF_ROCKS];
ArrayList * jets;
int current_rocktype_index;
int current_jet_index;
long height;
long number_of_rocks_fallen;
ArrayList * falling_rock;

void add_rock_part(ArrayList* rock, int x, int y){
    add_list(rock, (void*)new_coordinate(x, y));
}

void fill_rocks(){
    //always put highest rockpart first
    for (int i = 0; i < NR_OF_ROCKS; ++i) {
        rocks[i] = list();
    }
    for (int i = 0; i < 4; ++i) {
        add_rock_part(rocks[0], i,0);
        add_rock_part(rocks[3], 0,i);
    }

    add_rock_part(rocks[2], 2,2);
    add_rock_part(rocks[2], 2,1);

    add_rock_part(rocks[1], 1,2);
    add_rock_part(rocks[1], 1,0);

    for (int i = 0; i < 3; ++i) {
        add_rock_part(rocks[1], i,1);
        add_rock_part(rocks[2], i,0);
    }

    add_rock_part(rocks[4], 0,1);
    add_rock_part(rocks[4], 0,0);
    add_rock_part(rocks[4], 1,1);
    add_rock_part(rocks[4], 1,0);
}

void fall_current_rock(){
    int rock_part_index = 0;
    Coordinate * rock_part;
    long newy;
    long new_height = height;
    rock_part = (Coordinate*)get_list(falling_rock, 0);
    newy = rock_part->y - 1;

    while(rock_part_index < falling_rock->size
          && newy >= 0 && chamber[rock_part->x][newy % WINDOW_HEIGHT] == 0) {
        rock_part->y = newy;
        rock_part_index++;

        if(rock_part_index < falling_rock->size){
            rock_part = (Coordinate*)get_list(falling_rock, rock_part_index);
            newy = rock_part->y - 1;
        }
    }
    if(rock_part_index < falling_rock->size){ // the rock could not move, parts that have moved go back
        rock_part_index--;
        while(rock_part_index >= 0){
            rock_part = (Coordinate*)get_list(falling_rock, rock_part_index);
            rock_part->y += 1;
            rock_part_index--;
        }

        //determine highest point
        rock_part_index = 0;

        while(rock_part_index < falling_rock->size){
            rock_part = (Coordinate*)get_list(falling_rock, rock_part_index);
            if(rock_part->y + 1 > new_height){
                new_height = rock_part->y + 1;
            }
            rock_part_index++;
        }

        if(height < new_height){//new block is higher
            //clean up lowest part
            for (int i = height % WINDOW_HEIGHT ; i != (new_height + 7) % WINDOW_HEIGHT
                    ; i = (i + 1) % WINDOW_HEIGHT) {
//                fprintf(stderr, "delete %d\n", i);
                for (int j = 0; j < CHAMBER_WIDTH; ++j) {
                    chamber[j][i] = 0; //reset below window
                    //fprintf(stderr,  "(%d,%d)\n", j,i);
                }
            }
            for (int j = 0; j < CHAMBER_WIDTH; ++j) {
                chamber[j][(new_height + 8) % WINDOW_HEIGHT] = 1; //make bottom
                //fprintf(stderr,  "(%d,%d)\n", j,i);
            }
            height = new_height;
        }
        //fill in new values
        for (rock_part_index = 0; rock_part_index < falling_rock->size; ++rock_part_index) {
            rock_part = (Coordinate*)get_list(falling_rock, rock_part_index);
            chamber[rock_part->x][rock_part->y % WINDOW_HEIGHT] = 1;
            free(rock_part);
        }

        number_of_rocks_fallen++;
        falling_rock->size = 0;
    }
}

void push_current_rock(int x_direction){
    int rock_part_index = 0;
    Coordinate * rock_part;
    int newx;
    rock_part = (Coordinate*)get_list(falling_rock, 0);
    newx = rock_part->x + x_direction;

    while(rock_part_index < falling_rock->size && newx >= 0
          && newx < CHAMBER_WIDTH && chamber[newx][rock_part->y % WINDOW_HEIGHT] == 0) {
        rock_part->x = newx;
        rock_part_index++;
        if(rock_part_index < falling_rock->size){
            rock_part = (Coordinate*)get_list(falling_rock, rock_part_index);
            newx = rock_part->x + x_direction;
        }
    }

    if(rock_part_index < falling_rock->size){ // the rock could not move, parts that have moved go back
        rock_part_index--;
        while(rock_part_index >= 0){
            rock_part = (Coordinate*)get_list(falling_rock, rock_part_index);
            rock_part->x -= x_direction;
            rock_part_index--;
        }
    }
}

void tick(){
    if(falling_rock->size == 0){ // no rock is falling, choose new rock
        ArrayList * rock_template = rocks[current_rocktype_index];
        current_rocktype_index = (current_rocktype_index + 1) % NR_OF_ROCKS;
        for (int i = 0; i < rock_template->size; ++i) {
            add_rock_part(falling_rock, 2 + ((Coordinate*)get_list(rock_template, i))->x, height +  3 + ((Coordinate*)get_list(rock_template, i))->y);
        }
    } else {
        push_current_rock((int)(size_t)get_list(jets, current_jet_index));
        current_jet_index = (current_jet_index + 1) % jets->size;
//        fprintf(stderr, "(%d,%d) (%d,%d) (%d,%d) (%d,%d)\n"
//                , ((Coordinate*)falling_rock->items[0])->x
//                , ((Coordinate*)falling_rock->items[0])->y
//                , ((Coordinate*)falling_rock->items[1])->x
//                , ((Coordinate*)falling_rock->items[1])->y
//                , ((Coordinate*)falling_rock->items[2])->x
//                , ((Coordinate*)falling_rock->items[2])->y
//                , ((Coordinate*)falling_rock->items[3])->x
//                , ((Coordinate*)falling_rock->items[3])->y
//        );
        fall_current_rock();
//        fprintf(stderr, "(%d,%d) (%d,%d) (%d,%d) (%d,%d)\n"
//                , ((Coordinate*)falling_rock->items[0])->x
//                , ((Coordinate*)falling_rock->items[0])->y
//                , ((Coordinate*)falling_rock->items[1])->x
//                , ((Coordinate*)falling_rock->items[1])->y
//                , ((Coordinate*)falling_rock->items[2])->x
//                , ((Coordinate*)falling_rock->items[2])->y
//                , ((Coordinate*)falling_rock->items[3])->x
//                , ((Coordinate*)falling_rock->items[3])->y
//        );
//        fprintf(stderr, "\n");
    }
}


int main(int argc, char ** argv) {
    FILE *file = fopen(argv[3], "r");

    //rocks
    fill_rocks();

    //jets
    jets = list();
    char read = next_character(file);
    while (read == '>' || read == '<'){
        add_list(jets, (void*)(size_t)(read == '<' ? -1 : 1));
        read = next_character(file);
    }

    //chamber
    for (int i = 0; i < CHAMBER_WIDTH; ++i) {
        for (int j = 0; j < WINDOW_HEIGHT; ++j) {
            chamber[i][j] = 0;
        }
    }
//    for (int i = 0; i < CHAMBER_WIDTH; ++i) {
//        chamber[i][8] = 1;
//    }

    current_rocktype_index = 0;
    current_jet_index = 0;
    height = 0;
    number_of_rocks_fallen = 0;
    falling_rock = list();

    int prev_height = 0;
    int tick_count = 0;
    while(number_of_rocks_fallen < TOTAL_ROCKS){
        tick();
//        if(prev_height < height){
//            fprintf(stderr, "Difference = %ld\n", height - prev_height);
//            prev_height = height;
//        }
        tick_count++;
        if(tick_count % (34 * 10091) == 0){
            fprintf(stderr, "%d: %ld\n", tick_count, height - prev_height);
            prev_height = height;
        }
//        fprintf(stderr, "%ld\n", height);
//
//        for(int i = WINDOW_HEIGHT - 1; i >= 0; i--){
//            for (int j = 0; j < CHAMBER_WIDTH; ++j) {
//                fprintf(stderr, "%c", chamber[j][i] ? '#' : '.');
//            }
//            fprintf(stderr, "\n");
//        }
//        fprintf(stderr, "\n");
    }

    printf("%ld\n", height);
    fprintf(stderr, "%d\n", jets->size);
    fclose(file);
}
