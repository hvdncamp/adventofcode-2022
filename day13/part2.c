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

int compare_packets(const void* first, const void* second){
    const Packet* const packet1 = *(const Packet**)first;
    const Packet* const packet2 = *(const Packet**)second;
    int comparison;

    //both are numbers
    if(packet1->type == NUMBER && packet2->type == NUMBER){
        return - packet2->value + packet1->value;
    }
    //only one number
    if(packet1->type == NUMBER){
        if(packet2->contents->size == 0){//packet2 is shorter
            return 1;
        }
        comparison = compare_packets((void*)(&packet1), (void*)(&packet2->contents->items[0]));
        if(comparison != 0){
            return comparison;
        }
        return - packet2->contents->size + 1;
    }
    if (packet2->type == NUMBER) {
        if (packet1->contents->size == 0) {//packet1 is shorter
            return -1;
        }
        comparison = compare_packets((void *)(&packet1->contents->items[0]), (void *)(&packet2));
        if (comparison != 0) {
            return comparison;
        }
        return -1 + packet1->contents->size;
    }

    //two lists
    for (int i = 0; i < packet1->contents->size; ++i) {
        if(packet2->contents->size <= i){//packet2 is shorter
            return 1;
        }
        comparison = compare_packets((void*)(&packet1->contents->items[i]), (void*)(&packet2->contents->items[i]));
        if(comparison != 0){
            return comparison;
        }
    }
    return - (packet2->contents->size - packet1->contents->size);
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
    int result = 1;
    Packet* first_packet;
    Packet* second_packet;
    int count = 0;
    ArrayList* packets = list();

    while((first_packet = read_packet(file)) != NULL){
        count++;
        next_character(file); //'\n'
        second_packet = read_packet(file);

        add_list(packets, (void*)first_packet);
        add_list(packets, (void*)second_packet);

        skip_next_n_characters(file, 2); //"\n\n"
    }

    Packet* divider1 = new_list_packet();
    add_list(divider1->contents, new_list_packet());
    add_list(((Packet *)(divider1->contents->items[0]))->contents, new_number_packet());
    ((Packet*)(((Packet *)(divider1->contents->items[0]))->contents)->items[0])->value = 2;
    Packet* divider2 = new_list_packet();
    add_list(divider2->contents, new_list_packet());
    add_list(((Packet *)(divider2->contents->items[0]))->contents, new_number_packet());
    ((Packet*)(((Packet *)(divider2->contents->items[0]))->contents)->items[0])->value = 6;

    add_list(packets, divider1);
    add_list(packets, divider2);

    qsort(packets->items, packets->size, sizeof(void*), compare_packets);

    for (int i = 0; i < packets->size; ++i) {
        if(get_list(packets, i) == divider1 || get_list(packets, i) == divider2){
            result *= i + 1;
        }
    }

    printf("%d\n", result);
    fclose(file);
}