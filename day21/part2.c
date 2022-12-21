#include <stdio.h>
#include <stdlib.h>
#include "general_functions.h"

#define NAME_LENGTH 4
#define YOU "humn"

enum operation {PLUS, MINUS, MULT, DIV, CONST};
typedef struct monkey Monkey;
struct monkey {
    char name[NAME_LENGTH];
    enum operation type;
    char first[NAME_LENGTH];
    char second[NAME_LENGTH];
    Monkey* left;
    Monkey* right;
    int calculated;
    long value;
};

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

//returns 1 if it worked, 0 if not
//calculated is 1 if the value is calculated and -1 if it depends on YOU
int calculate_all_values(Monkey* monkey){
    if (monkey->name[0] == YOU[0] && monkey->name[1] == YOU[1] && monkey->name[2] == YOU[2] && monkey->name[3] == YOU[3]) {
        monkey->calculated = -1;
        return 0;
    }

    int left_calculated, right_calculated;

    if(!monkey->calculated){
        left_calculated = calculate_all_values(monkey->left);
        right_calculated =  calculate_all_values(monkey->right);
        if(left_calculated && right_calculated){
            switch (monkey->type) {
                case PLUS:
                    monkey->value = monkey->left->value + monkey->right->value;
                    break;
                case MINUS:
                    monkey->value = monkey->left->value - monkey->right->value;
                    break;
                case MULT:
                    monkey->value = monkey->left->value * monkey->right->value;
                    break;
                case DIV:
                    monkey->value = monkey->left->value / monkey->right->value;
                    break;
                default:
                    break;
            }
            monkey->calculated = 1;
        } else {
            monkey->calculated = -1;
        }
    }
    return monkey->calculated == 1 ? 1 : 0;
}

//calculates what YOU must shout so that monkey shouts value
long shout_to_get(Monkey* monkey, long value){
    fprintf(stderr, "Monkey %s should shout %ld\n", monkey->name, value);
    if (monkey->name[0] == YOU[0] && monkey->name[1] == YOU[1] && monkey->name[2] == YOU[2] && monkey->name[3] == YOU[3]){
        return value;
    }
    Monkey* done_monkey;
    Monkey* next_monkey;
    if(monkey->left->calculated == -1 && monkey->right->calculated == 1){
        done_monkey = monkey->right;
        next_monkey = monkey->left;
    } else if (monkey->left->calculated == 1 && monkey->right->calculated == -1){
        done_monkey = monkey->left;
        next_monkey = monkey->right;
    } else {
        fprintf(stderr, "%d, %d\n",monkey->left->calculated, monkey->right->calculated );
        exit(1);
    }
    switch (monkey->type) {
        case PLUS:
            return shout_to_get(next_monkey, value - done_monkey->value);
        case MINUS:
            if (done_monkey == monkey->left){
                return shout_to_get(next_monkey,  done_monkey->value - value);
            } else {
                return shout_to_get(next_monkey, value + done_monkey->value);
            }
        case MULT:
            return shout_to_get(next_monkey, value / done_monkey->value);
        case DIV:
            if (done_monkey == monkey->left){
                return shout_to_get(next_monkey,  done_monkey->value / value);
            } else {
                return shout_to_get(next_monkey, value * done_monkey->value);
            }
        default:
            fprintf(stderr, "unexpected\n");
            exit(1);
    }
}

//don't use tree anymore
void set_monkey_pointers(Node* tree_root, const char name[4]){
    Monkey* monkey = find_monkey(name, tree_root);
    if(monkey->type != CONST){
        set_monkey_pointers(tree_root, monkey->first);
        monkey->left = find_monkey(monkey->first, tree_root);
        set_monkey_pointers(tree_root, monkey->second);
        monkey->right = find_monkey(monkey->second, tree_root);
    }
}

int main(int argc, char ** argv) {
    FILE *file = fopen(argv[3], "r");
    Monkey* monkey;
    Node* tree_root = new_node();

    long you_shout = 0;
    while((monkey = read_monkey(file))){
        add_monkey(tree_root, monkey);
    }
    set_monkey_pointers(tree_root, "root");

    monkey = find_monkey("root", tree_root);
    calculate_all_values(monkey);
    if(monkey->left->calculated == -1 && monkey->right->calculated == 1){
        you_shout = shout_to_get(monkey->left, monkey->right->value);
    } else if (monkey->left->calculated == 1 && monkey->right->calculated == -1){
        you_shout = shout_to_get(monkey->right, monkey->left->value);
    } else {
        fprintf(stderr, "%d, %d\n",monkey->left->calculated, monkey->right->calculated );
    }

    printf("%ld\n", you_shout);
    fclose(file);
}
