#include <stdio.h>
#include <stdlib.h>
#include "../lib/general_functions.h"
#include "../lib/treeset.h"
#include "macros.h"

#define NR_OF_KNOTS 2

void move_in_direction(Coordinate* point, char direction){
    switch (direction) {
        case 'R':
            point->x++;
            break;
        case 'L':
            point->x--;
            break;
        case 'U':
            point->y++;
            break;
        case 'D':
            point->y--;
            break;
        default:
            fprintf(stderr, "Direction should be R, L, U or D, not %c\n", direction);
            exit(1);
    }
}

void follow(Coordinate* head, Coordinate* tail){
    int xdiff = tail->x - head->x;
    int ydiff = tail->y - head->y;

    if(ABS(xdiff) >= 2 || ABS(ydiff) >= 2){ //the tail is no longer next to head
        tail->x -= (ABS(xdiff) == 1) ? xdiff : xdiff/2;
        tail->y -= (ABS(ydiff) == 1) ? ydiff : ydiff/2;
    } //else tail does not move
}

//moves the entire rope (10 knots)
//returns the coordinate of tail after the move
Coordinate* move_one_step(Coordinate* knots, char direction){
    //move the head
    move_in_direction(knots, direction);

    //move the tail
    for (int i = 1; i < NR_OF_KNOTS; ++i) {
        follow(knots + i - 1, knots + i);
    }

    //return a new coordinate of the last knot
    return new_coordinate(knots[NR_OF_KNOTS - 1].x, knots[NR_OF_KNOTS - 1].y);
}

int main(int argc, char ** argv) {
    FILE * file = fopen(argv[3], "r");
    Tree* visited = new_tree(compare_coordinate);
    add_value(visited, new_coordinate(0,0));
    Coordinate knots[NR_OF_KNOTS];
    for (int i = 0; i < NR_OF_KNOTS; ++i) {
        knots[i].x = 0;
        knots[i].y = 0;
    }
    char direction;
    int nr_of_steps;
    Coordinate* new_place;

    while(fscanf(file, "%c %d\n", &direction, &nr_of_steps) == 2){
        while(nr_of_steps > 0){
            new_place = move_one_step(knots, direction);
            if(!add_value(visited, (void*)(new_place))){
                free_coordinate(new_place);
            }
            nr_of_steps--;
        }
    }

    printf("The sum is %d\n", visited->number_of_elements);
    return 0;
}
