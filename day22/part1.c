#include <stdio.h>
#include <stdlib.h>
#include "general_functions.h"
#include "macros.h"

enum square_type {VOID = 0, OPEN, WALL};
enum direction {UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3};

typedef struct position {
    int x;
    int y;
    enum direction direction;
} Position;

void determine_grid_size(FILE* file, int* x, int* y){
    int curr_x;
    *x = 0;
    *y = 0;
    while(next_character(file) != '\n'){
        curr_x = 1;
        while(next_character(file) != '\n'){
            curr_x++;
        }
        (*x) = MAX(*x, curr_x);
        (*y)++;
    }
}

void read_grid(FILE* file, enum square_type** grid, int y_length){
    int x = 0;
    char read;
    for (int y = 0; y < y_length; ++y) {
        while((read=(next_character(file))) == ' '){
            x++;
        }
        switch (read) {
            case '.':
                grid[x][y] = OPEN;
                break;
            case '#':
                grid[x][y] = WALL;
                break;
            case ' ':
                grid[x][y] = VOID;
                break;
            default:
                fprintf(stderr, "%c should not be read here\n", read);
                exit(1);
        }
        x++;
        while ((read=next_character(file)) != '\n'){
            switch (read) {
                case '.':
                    grid[x][y] = OPEN;
                    break;
                case '#':
                    grid[x][y] = WALL;
                    break;
                case ' ':
                    grid[x][y] = VOID;
                    break;
                default:
                    fprintf(stderr, "%c should not be read here\n", read);
                    exit(1);
            }
            x++;
        }
        x = 0;
    }
    skip_next_n_characters(file, 1);
}

void print_grid(enum square_type** grid, int xmax, int ymax){
    for (int i = 0; i < ymax; ++i) {
        for (int j = 0; j < xmax; ++j) {
            if(grid[j][i] == VOID){
                fprintf(stderr, " ");
            } else if (grid[j][i] == OPEN){
                fprintf(stderr, ".");
            } else{
                fprintf(stderr, "#");
            }
        }
        fprintf(stderr, "\n");

    }
}

enum square_type get_type(enum square_type** grid, int xmax, int ymax, int x, int y){
    if(x < 0 || y < 0 || x >= xmax || y >= ymax){
        return VOID;
    } else {
        return grid[x][y];
    }
}

int loop(enum square_type** grid, int xmax, int ymax, Position* position){
    //now position is in VOID. Move it back to a valid position
    int x_temp = position->x;
    int y_temp = position->y;
    switch (position->direction) {
        case UP:
            while (y_temp + 1 < ymax && grid[x_temp][y_temp + 1] != VOID){
                y_temp++;
            }
            if (grid[x_temp][y_temp] == WALL){
                position->y += 1;
                return 0;
            } else {
                position->y = y_temp;
                return 1;
            }
        case RIGHT:
            while (x_temp - 1 >= 0 && grid[x_temp - 1][y_temp] != VOID){
                x_temp--;
            }
            if (grid[x_temp][y_temp] == WALL){
                position->x -= 1;
                return 0;
            } else {
                position->x = x_temp;
                return 1;
            }
        case DOWN:
            while (y_temp - 1 >= 0 && grid[x_temp][y_temp - 1] != VOID){
                y_temp--;
            }
            if (grid[x_temp][y_temp] == WALL){
                position->y -= 1;
                return 0;
            } else {
                position->y = y_temp;
                return 1;
            }
        case LEFT:
            while (x_temp + 1 < xmax && grid[x_temp + 1][y_temp] != VOID){
                x_temp++;
            }
            if (grid[x_temp][y_temp] == WALL){
                position->x += 1;
                return 0;
            } else {
                position->x = x_temp;
                return 1;
            }
    }
}

int move_one(Position* position, enum square_type** grid, int xmax, int ymax){
    int new_x;
    int new_y;
    switch (position->direction) {
        case UP:
            new_x = position->x;
            new_y = position->y - 1;
            break;
        case RIGHT:
            new_x = position->x + 1;
            new_y = position->y ;
            break;
        case DOWN:
            new_x = position->x;
            new_y = position->y + 1;
            break;
        case LEFT:
            new_x = position->x - 1;
            new_y = position->y;
            break;
    }

    switch (get_type(grid, xmax, ymax, new_x, new_y)) {
        case WALL:
            return 0;
        case OPEN:
            position->x = new_x;
            position->y = new_y;
            return 1;
        case VOID:
            position->x = new_x;
            position->y = new_y;
            return loop(grid, xmax, ymax, position);
        default:
            fprintf(stderr, "impossible\n");
            exit(1);
    }
}

void move(Position* position, int number, enum square_type** grid, int xmax, int ymax){
    int count = 0;
    while(count < number && move_one(position, grid, xmax, ymax)){
        fprintf(stderr, "x: %d, y:%d, dir:%d\n", position->x, position->y, position->direction);
        count++;
    }
}

void turn(Position* position, char direction){
    if(direction == 'R'){
        position->direction = (position->direction + 1) % 4;
    } else {
        position->direction = (position->direction + 3) % 4;
    }
}

int main(int argc, char ** argv) {
    FILE *file = fopen(argv[3], "r");

    int x_length;
    int y_length;
    determine_grid_size(file, &x_length, &y_length);
    fclose(file);
    enum square_type* grid[x_length];
    for (int i = 0; i < x_length; ++i) {
        grid[i] = calloc(y_length, sizeof(enum square_type));
    }

    file = fopen(argv[3], "r");

    //read grid
    read_grid(file, grid, y_length);

    //move
    int number_of_steps;
    char turn_to;
    Position position;
    position.y = 0;
    position.direction = RIGHT;
    position.x = 0;
    while(grid[position.x][0] != OPEN){
        position.x++;
    }

    print_grid(grid, x_length, y_length);

    while(fscanf(file, "%d%c", &number_of_steps, &turn_to) == 2) {
        move(&position, number_of_steps, grid, x_length, y_length);
        turn(&position, turn_to);
    }

    printf("%d\n", (position.x + 1)* 4 + 1000 *( position.y + 1) + position.direction);


    for (int i = 0; i < x_length; ++i) {
        free(grid[i]);
    }
    fclose(file);
}