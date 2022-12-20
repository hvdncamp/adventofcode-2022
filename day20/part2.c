#include <stdio.h>
#include <stdlib.h>
#include "arraylist.h"

#define DECRYPTION_KEY 811589153

typedef struct link Link;
struct link{
    Link* next;
    Link* prev;
    long number;
};

Link* new_link(long number){
    Link* link = malloc(sizeof(Link));
    link->number = number;
    return link;
}

void move_link(Link* link, int n){
    Link* current_link;
    if(link->number > 0){ //move right
        current_link = link;
        for (int i = 0; i < link->number % (n - 1); ++i) {
            current_link = current_link->next;
        }
        link->next->prev = link->prev;
        link->prev->next = link->next;
        link->next = current_link->next;
        current_link->next->prev = link;
        link->prev = current_link;
        current_link->next = link;
    } else if(link->number < 0){//move left
        current_link = link;
        for (int i = 0; i > link->number % (n - 1); --i) {
            current_link = current_link->prev;
        }
        link->prev->next = link->next;
        link->next->prev = link->prev;
        link->prev = current_link->prev;
        current_link->prev->next = link;
        link->next = current_link;
        current_link->prev = link;
    } //if 0 do nothing
}

void print_list(Link* first_link){
    Link* link = first_link;
    do {
        fprintf(stderr, "%ld\n", link->number);
        link = link->next;
    } while (link != first_link);
    fprintf(stderr, "\n");
}

int main(int argc, char ** argv) {
    FILE *file = fopen(argv[3], "r");

    Link* link;
    Link* prev_link;
    Link* first_link;
    ArrayList * links = list();
    long number;

    fscanf(file, "%ld\n", &number);
    prev_link = new_link((long)(DECRYPTION_KEY * number));
    add_list(links, prev_link);
    first_link = prev_link;

    while(fscanf(file, "%ld\n", &number) == 1){
        link = new_link((long)(DECRYPTION_KEY * number));
        add_list(links, link);
        prev_link->next = link;
        link->prev = prev_link;
        prev_link = link;
    }
    first_link->prev = link;
    link->next = first_link;

    //decrypt
    for (int j = 0; j < 10; ++j) {//ten times
        for (int i = 0; i < links->size; ++i) {//move all links in order
            move_link((Link*) get_list(links, i), links->size);
        }
    }

    //find start
    link = first_link;
    while(link->number != 0){
        link = link->next;
    }

    //calculate 1000th, 2000th and 3000th number
    number = 0;
    for (int j = 0; j < 3; ++j) {
        for (int i = 0; i < 1000; ++i) {
            link = link->next;
        }
        number += link->number;
    }

    printf("%ld\n", number);
}
