#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lib/general_functions.h"
#include "../lib/arraylist.h"

typedef enum node_type{FOLDER, ITEM} node_type; //cannot use file because that already exists
typedef union size_or_children{
    ArrayList* children;
    int file_size;
} Size_or_children;

typedef struct node Node;
struct node {
    Node* parent;
    node_type type;
    Size_or_children contents;
    char name[20];
};

Node* new_node(char* name, node_type type){
    Node* node = malloc(sizeof(Node));
    node->type = type;
    strcpy(node->name, name);
    return node;
}

Node* new_folder(char* name){
    Node* node = new_node(name, FOLDER);
    node->contents.children = list();
    return node;
}

Node* new_file(char* name, int size){
    Node* node = new_node(name, ITEM);
    node->contents.file_size = size;
    return node;
}

void add_file_to_folder(Node* file, Node* folder){
    add_list(folder->contents.children, file);
    file->parent = folder;
}

Node* get_child(Node* folder, char* name){
    if(folder->type != FOLDER){
        fprintf(stderr, "This should be a folder\n");
        exit(1);
    }
    for (int i = 0; i < folder->contents.children->size; ++i) {
        if(strcmp(name, ((Node*)folder->contents.children->items[i])->name) == 0){
            return folder->contents.children->items[i];
        }
    }
    fprintf(stderr, "Child named %s not found in folder %s\n", name, folder->name);
    exit(1);
}

void free_node(Node* node){
    if(node->type == FOLDER){
        for (int i = 0; i < node->contents.children->size; ++i) {
            free_node((Node*)node->contents.children->items[i]);
        }
        free_list(node->contents.children);
    }
    free(node);
}

int node_size(Node* node){
    int size = 0;
    if(node->type == ITEM){
        return node->contents.file_size;
    }
    for (int i = 0; i < node->contents.children->size; ++i) {
        size += node_size(node->contents.children->items[i]);
    }
    return size;
}

int find_minimal_removable(Node* node, int* minimal, int space_needed){
    int size = 0;
    if(node->type == ITEM){
        return node->contents.file_size;
    }
    for (int i = 0; i < node->contents.children->size; ++i) {
        size += find_minimal_removable(node->contents.children->items[i], minimal, space_needed);
    }
    if(size >= space_needed && size < *minimal){
        (*minimal) = size;
    }
    return size;
}


int main(int argc, char ** argv) {
    FILE * file = fopen(argv[3], "r");
    int minimal_removable;
    char read = next_character(file);
    char name_buffer[20];
    int size_read;
    int stop;
    int space_needed;

    Node* root = new_folder("/");
    Node* current_node;

    while(read != EOF && read != '\n'){
        if(read != '$'){
            fprintf(stderr, "A $ should be read\n");
            exit(1);
        }
        next_character(file);//' '
        read = next_character(file);
        switch (read) {
            case 'c':
                skip_next_n_characters(file, 2); //"d "
                fscanf(file, "%s", name_buffer);
                next_character(file);//newline
                if(name_buffer[0]=='/'){
                    current_node = root;
                } else if (name_buffer[0] == '.' && name_buffer[1] == '.'){
                    current_node = current_node->parent;
                } else {
                    current_node = get_child(current_node, name_buffer);
                }
                break;
            case 'l':
                skip_next_n_characters(file, 2); //"s\n "
                stop = 0;
                while(!stop){
                    read = fgetc(file);
                    if(read == 'd'){//folder
                        skip_next_n_characters(file, 3);//"ir "
                        fscanf(file, "%s", name_buffer);
                        next_character(file);//newline
                        add_file_to_folder(new_folder(name_buffer), current_node);
                    } else if (read == '$' || read == '\n' || read == EOF){//done reading contents
                        ungetc(read, file);
                        stop = 1;
                    } else {//file
                        ungetc(read, file);
                        fscanf(file, "%d %s", &size_read, name_buffer);
                        next_character(file);//newline
                        add_file_to_folder(new_file(name_buffer, size_read), current_node);
                    }
                }
                break;
            default:
                break;
        }
        read = next_character(file);
    }

    minimal_removable = node_size(root);
    space_needed = minimal_removable - 40000000;
    find_minimal_removable(root, &minimal_removable, space_needed);

    free_node(root);
    printf("The smallest removable folder has size %d\n", minimal_removable);
    return 0;
}
