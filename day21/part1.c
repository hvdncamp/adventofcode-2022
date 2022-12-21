#include <stdio.h>
#include <stdlib.h>
#include "general_functions.h"

#define NAME_LENGTH 4

enum operation {PLUS, MINUS, MULT, DIV, CONST};

typedef struct monkey {
    char name[NAME_LENGTH];
    enum operation type;
    char first[NAME_LENGTH];
    char second[NAME_LENGTH];
    int calculated;
    long value;
} Monkey;

typedef struct node Node;
struct node{
    Node* children[26];
    Monkey* monkey;
};

Node* new_node(){
    Node* node = malloc(sizeof(Node));
    for (int i = 0; i < 26; ++i) {
        node->children[i] = NULL;
    }
    node->monkey = NULL;
    return node;
}

Monkey* read_monkey(FILE* file){
    Monkey* monkey = malloc(sizeof(Monkey));
    for (int i = 0; i < NAME_LENGTH; ++i) {
        monkey->name[i] = next_character(file);
    }
    skip_next_n_characters(file, 2);//": "
    char read = next_character(file);
    if(read <= '9' && read >= '0'){//constant
        monkey->type = CONST;
        ungetc(read, file);
        fscanf(file, "%ld\n", &monkey->value);
        monkey->calculated = 1;
    } else if(read <= 'z' && read >= 'a') {//operation
        monkey->first[0] = read;
        for (int i = 1; i < NAME_LENGTH ; ++i) {
            monkey->first[i] = next_character(file);
        }
        skip_next_n_characters(file, 1);// ' '
        read = next_character(file);
        switch (read) {
            case '+':
                monkey->type = PLUS;
                break;
            case '-':
                monkey->type = MINUS;
                break;
            case '*':
                monkey->type = MULT;
                break;
            case '/':
                monkey->type = DIV;
                break;
            default:
                break;
        }
        skip_next_n_characters(file, 1); // ' '
        for (int i = 0; i < NAME_LENGTH; ++i) {
            monkey->second[i] = next_character(file);
        }
        skip_next_n_characters(file, 1); // '\n'
        monkey->calculated = 0;
    } else {
        return NULL;
    }
    return monkey;
}

void add_monkey(Node* root, Monkey* monkey){
    Node* current_node = root;
    for (int i = 0; i < NAME_LENGTH; ++i) {
        if(current_node->children[monkey->name[i] - 'a'] == NULL){
            current_node->children[monkey->name[i] - 'a'] = new_node();
        }
        current_node = current_node->children[monkey->name[i] - 'a'];
    }
    current_node->monkey = monkey;
}

Monkey* find_monkey(const char name[4], Node* root_node){
    Node* current_node = root_node;
    for (int i = 0; i < NAME_LENGTH; ++i) {
        if(current_node->children[name[i] - 'a'] == NULL){
            fprintf(stderr, "This monkey does not exist\n");
            exit(1);
        }
        current_node = current_node->children[name[i] - 'a'];
    }
    return current_node->monkey;
}

long value_of(char name[4], Node* root_node){
    Monkey* monkey = find_monkey(name, root_node);
    if(!monkey->calculated){
        switch (monkey->type) {
            case PLUS:
                monkey->value = value_of(monkey->first, root_node) + value_of(monkey->second, root_node);
                monkey->calculated = 1;
                break;
            case MINUS:
                monkey->value = value_of(monkey->first, root_node) - value_of(monkey->second, root_node);
                monkey->calculated = 1;
                break;
            case MULT:
                monkey->value = value_of(monkey->first, root_node) * value_of(monkey->second, root_node);
                monkey->calculated = 1;
                break;
            case DIV:
                monkey->value = value_of(monkey->first, root_node) / value_of(monkey->second, root_node);
                monkey->calculated = 1;
                break;
            default:
                break;
        }
    }
    return monkey->value;
}

int main(int argc, char ** argv) {
    FILE *file = fopen(argv[3], "r");
    Monkey* monkey;
    Node* tree_root = new_node();

    int monkey_count = 0;
    while((monkey = read_monkey(file))){
        add_monkey(tree_root, monkey);
        monkey_count++;
    }

    printf("%d\n", monkey_count);
    printf("%ld\n", value_of("root", tree_root));

    fclose(file);
}
