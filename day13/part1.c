#include <stdio.h>
#include <stdlib.h>
#include "arraylist.h"
#include "general_functions.h"

enum packet_type {LIST, NUMBER};

typedef struct packet {
    enum packet_type type;
    ArrayList* contents;
    int value;
} Packet;

Packet* new_list_packet(){
    Packet* packet = malloc(sizeof(Packet));
    packet->type = LIST;
    packet->contents = list();
    return packet;
}

Packet* new_number_packet(){
    Packet* packet = malloc(sizeof(Packet));
    packet->type = NUMBER;
    return packet;
}

void free_packet(Packet* packet){
    if(packet->type == LIST){
        for (int i = 0; i < packet->contents->size; ++i) {
            free_packet((Packet*)packet->contents->items[i]);
        }
        free(packet->contents);
    }
    free(packet);
}

int compare_packets(Packet* packet1, Packet* packet2){
    //fprintf(stderr, "Compare %d\n", count);
    Packet* new_packet;
    Packet* number_packet;
    //both are numbers
    if(packet1->type == NUMBER && packet2->type == NUMBER){
        return packet2->value - packet1->value;
    }
    //only one number
    if (packet1->type == NUMBER || packet2->type == NUMBER){
        number_packet = packet1->type == NUMBER ? packet1 : packet2;
        number_packet->type = LIST;
        number_packet->contents = list();
        new_packet = new_number_packet();
        new_packet->value = number_packet->value;
        add_list(number_packet->contents, (void*)new_packet);
    }
    //both are lists
    int comparison;
    for (int i = 0; i < packet1->contents->size; ++i) {
        if(packet2->contents->size <= i){//packet2 is shorter
            return -1;
        }
        comparison = compare_packets(packet1->contents->items[i], packet2->contents->items[i]);
        if(comparison != 0){
            return comparison;
        }
    }
    return packet2->contents->size - packet1->contents->size;
    //0 if equal, positive if not
}

Packet* read_packet(FILE* file){
    char read;
    Packet* packet;
    if ((read = next_character(file)) == '[') { //list
        packet = new_list_packet();
        if ((read = next_character(file)) != ']') {
            ungetc(read, file);
            do {
                add_list(packet->contents, (void*)read_packet(file));
            } while ((read = next_character(file)) != ']');//reads ',' or ']'
        }
    } else if(read <= '9' && read >= '0'){ //number
        ungetc(read, file);
        packet = new_number_packet();
        fscanf(file, "%d", &packet->value);
    } else {
        return NULL;
    }
    return packet;
}

int main(int argc, char ** argv) {
    FILE *file = fopen(argv[3], "r");
    int correct_indices_sum = 0;
    Packet* first_packet;
    Packet* second_packet;
    int count = 0;

    while((first_packet = read_packet(file)) != NULL){
        count++;
        next_character(file); //'\n'
        second_packet = read_packet(file);

        if(compare_packets(first_packet, second_packet) > 0){
            correct_indices_sum += count;
        }

        skip_next_n_characters(file, 2); //"\n\n"
    }

    printf("%d\n", correct_indices_sum);
    fclose(file);
}