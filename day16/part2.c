#include <stdio.h>
#include "general_functions.h"
#include "macros.h"

#define MAX_NR_OF_VALVES 70
#define NR_OF_MINUTES 26

int flowrates[MAX_NR_OF_VALVES];
int distance[MAX_NR_OF_VALVES][MAX_NR_OF_VALVES];
int valve_count;
int used[MAX_NR_OF_VALVES];

int index_of_name(char char1, char char2){
    return (char1 - 'A') * 26 + (char2 - 'A');
}

void calculate_distances(int initial_distance[][MAX_NR_OF_VALVES], int initial_valvecount){
    //Floyd Warshall -> distances
    for (int k = 0; k < initial_valvecount; ++k) {
        for (int i = 0;  i< initial_valvecount; ++i) {
            for (int j = 0; j < initial_valvecount; ++j) {
                if(initial_distance[i][k] + initial_distance[k][j] < initial_distance[i][j]){
                    initial_distance[i][j] = initial_distance[i][k] + initial_distance[k][j];
                }
            }
        }
    }
}

//reads the input and calculates the distances between valves with non-zero flowrate
void read_valves(FILE* file){
    char char1;
    char char2;
    int current_valve;
    int flow_rate_read;

    int name_to_number[26*26];
    int finalnumber_to_firstnumber[MAX_NR_OF_VALVES];
    for (int i = 0; i < 26*26; ++i) {
        name_to_number[i] = -1;
    }
    name_to_number[0] = 0; //AA always on place 0
    finalnumber_to_firstnumber[0] = 0;

    flowrates[0] = 0;
    int initial_valve_count = 1;
    valve_count = 1;

    int initial_distance[MAX_NR_OF_VALVES][MAX_NR_OF_VALVES];
    for (int i = 0; i < MAX_NR_OF_VALVES; ++i) {
        for (int j = 0; j < MAX_NR_OF_VALVES; ++j) {
            initial_distance[i][j] = MAX_NR_OF_VALVES;//infinity
        }
        initial_distance[i][i] = 0;
    }

    while(fscanf(file, "Valve %c%c has flow rate=%d; tunnel", &char1, &char2, &flow_rate_read)==3){
        skip_next_n_characters(file, 15); //[s] lead[s] to valve
        if(name_to_number[index_of_name(char1, char2)] == -1){
            name_to_number[index_of_name(char1, char2)] = initial_valve_count;
            initial_valve_count++;
        }
        current_valve = name_to_number[index_of_name(char1, char2)];
        if(flow_rate_read != 0){
            finalnumber_to_firstnumber[valve_count] = current_valve;
            flowrates[valve_count] = flow_rate_read;
            valve_count++;
        }

        if(next_character(file) != 's'){
            ungetc(' ', file);
        }

        do {
            next_character(file); // ' '
            fscanf(file, "%c%c", &char1, &char2);
            if(name_to_number[index_of_name(char1, char2)] == -1){
                name_to_number[index_of_name(char1, char2)] = initial_valve_count;
                initial_valve_count++;
            }
            initial_distance[current_valve][name_to_number[index_of_name(char1, char2)]] = 1;
        } while (next_character(file)==',');
        //we have read the '\n'
    }

    calculate_distances(initial_distance, initial_valve_count);

    for (int i = 0; i < valve_count; ++i) {
        for (int j = 0; j < valve_count; ++j) {
            distance[i][j] = initial_distance[finalnumber_to_firstnumber[i]][finalnumber_to_firstnumber[j]];
        }
    }
}

int max_pressure_release(int time_left, int time_left_elephant, int location, int location_elephant, int current_release, int current_max){
    int new_release;
    int upperbound = current_release;
    if (current_release > current_max){
        current_max = current_release;
    } else {
        for (int i = 0; i < valve_count; ++i) {
            if(!used[i]){
                upperbound += flowrates[i] * (MAX(time_left_elephant, time_left) - 2);
            }
        }
        //pruning
        if(upperbound < current_max){ //there is no way this configuration leads to more pressure release
            return current_max;
        }
    }

    if(time_left >= time_left_elephant){// the elephant is still moving or has just stopped
        for (int i = 0; i < valve_count; ++i) {
            if(!used[i] && distance[location][i] < time_left){ //you can reach your next location
                used[i] = 1;
                new_release = max_pressure_release(time_left - distance[location][i] - 1, time_left_elephant
                                                   , i, location_elephant, current_release + (time_left - distance[location][i] - 1)*flowrates[i], current_max);
                used[i] = 0;
                if(new_release > current_max){
                    current_max = new_release;
                }
            }
        }
    } else {// you are still moving
        for (int i = 0; i < valve_count; ++i) {
            if(!used[i] && distance[location_elephant][i] < time_left_elephant){ //elephant can reach the next location
                used[i] = 1;
                new_release = max_pressure_release(time_left, time_left_elephant  - distance[location_elephant][i] - 1
                        , location, i, current_release + (time_left_elephant - distance[location_elephant][i] - 1)*flowrates[i], current_max);
                used[i] = 0;
                if(new_release > current_max){
                    current_max = new_release;
                }
            }
        }
    }
    return current_max;
}

int main(int argc, char ** argv) {
    FILE *file = fopen(argv[3], "r");
    read_valves(file);
    for (int i = 0; i < MAX_NR_OF_VALVES; ++i) {
        used[i] = 0;
    }

    printf("%d\n", max_pressure_release(NR_OF_MINUTES, NR_OF_MINUTES, 0, 0, 0, 1500));

    fclose(file);
}
