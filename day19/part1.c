#include <stdio.h>
#include "macros.h"

enum resource {ORE = 0, CLAY = 1, OBSIDIAN = 2, GEODE = 3};
#define TIME_AVAILABLE 24

void collect_resources(const int robots[4], int resources[4]){
    for (int i = ORE; i <= GEODE ; ++i) {
        resources[i] += robots[i];
    }
}

void uncollect_resources(const int robots[4], int resources[4]){
    for (int i = ORE; i <= GEODE ; ++i) {
        resources[i] -= robots[i];
    }
}

int max_geodes(int blueprint[4][4], int robots[4], int resources[4], int time_left, int current_max){
    int new_value;
    if(time_left == 1){
        new_value = robots[GEODE] + resources[GEODE];
        if(current_max < new_value){
            fprintf(stderr, "time: %d, new value: %d\n", time_left, new_value);
            return new_value;
        } else {
            return current_max;
        }
        //return MAX(current_max, robots[GEODE] + resources[GEODE]);
    }
    //snoeien
    if((time_left * robots[GEODE] + ((time_left * (time_left - 1)) / 2)) + resources[GEODE] < current_max){
        return current_max;
    }
    int enough_resources;

    if(robots[ORE] >= blueprint[GEODE][ORE] && robots[OBSIDIAN] >= blueprint[GEODE][OBSIDIAN]){
        //making anything other than GEODE-robot is not useful
        if(resources[ORE] >= blueprint[GEODE][ORE] && resources[OBSIDIAN] >= blueprint[GEODE][OBSIDIAN]){
            new_value = resources[GEODE] + (time_left * robots[GEODE]) + ((time_left * (time_left - 1)) / 2);
            if(new_value > current_max){
                fprintf(stderr, "time: %d, new value: %d\n", time_left, new_value);
            }
            return MAX(new_value, current_max);
        } else {//not enough resources
            collect_resources(robots, resources);
            new_value = max_geodes(blueprint, robots, resources, time_left - 1, current_max);
            if(new_value > current_max){
                fprintf(stderr, "time: %d, new value: %d\n", time_left, new_value);
            }
            current_max = new_value > current_max ? new_value : current_max;
            uncollect_resources(robots, resources);
        }
        return current_max;
    }

    for (int i = GEODE; i >= ORE; --i) { // try making every robot
        //don't make obsidian robot if not necessary
        if(!(i == OBSIDIAN && robots[OBSIDIAN] >= blueprint[GEODE][OBSIDIAN]) && (time_left > 3 || i == GEODE)) {
            enough_resources = 1;
            for (int j = ORE; j <= GEODE; ++j) {
                if (resources[j] < blueprint[i][j]) {
                    enough_resources = 0;
                }
            }
            if (enough_resources) {
                collect_resources(robots, resources);
                robots[i]++;
                for (int j = ORE; j <= GEODE; ++j) {
                    resources[j] -= blueprint[i][j];
                }
                new_value = max_geodes(blueprint, robots, resources, time_left - 1, current_max);
                if (new_value > current_max) {
                    fprintf(stderr, "time: %d, new value: %d\n", time_left, new_value);
                }
                current_max = new_value > current_max ? new_value : current_max;

                robots[i]--;
                for (int j = ORE; j <= GEODE; ++j) {
                    resources[j] += blueprint[i][j];
                }
                uncollect_resources(robots, resources);
            }
        }
    }
    //try doing nothing
    collect_resources(robots, resources);
    new_value = max_geodes(blueprint, robots, resources, time_left - 1, current_max);
    if(new_value > current_max){
        fprintf(stderr, "time: %d, new value: %d\n", time_left, new_value);
    }
    current_max = new_value > current_max ? new_value : current_max;
    uncollect_resources(robots, resources);
    return current_max;
}

int main(int argc, char ** argv) {
    FILE *file = fopen(argv[3], "r");

    int robots[4];//count by type, indexed by enum resource
    int resources[4];
    int blueprint_number;
    int blueprint[4][4] = { 0 };//nr of resources needed, indexed by enum resource;

    int quality_level_sum = 0;

    while(fscanf(file, " Blueprint %d: Each ore robot costs %d ore. Each clay robot costs %d ore. Each obsidian robot costs %d ore and %d clay. Each geode robot costs %d ore and %d obsidian. "
    , &blueprint_number, &blueprint[ORE][ORE], &blueprint[CLAY][ORE], &blueprint[OBSIDIAN][ORE]
    , &blueprint[OBSIDIAN][CLAY], &blueprint[GEODE][ORE], &blueprint[GEODE][OBSIDIAN]) == 7){

        //initialising
        for (int i = ORE; i <= GEODE; ++i) {
            robots[i] = 0;
            resources[i] = 0;
        }
        robots[0] = 1;

        //calculate
        quality_level_sum += blueprint_number * max_geodes(blueprint, robots, resources, TIME_AVAILABLE, 0);

        //reset for next iteration
        for (int i = ORE; i <= GEODE; ++i) {
            for (int j = ORE; j <= GEODE; ++j) {
                blueprint[i][j] = 0;
            }
        };
    }

    printf("%d\n", quality_level_sum);

    fclose(file);
}
