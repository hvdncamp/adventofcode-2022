//
// Created by hvdncamp on 9/12/2022.
//

#include <stdlib.h>
#include "treeset.h"

Tree* new_tree(int (*compare)(void* first, void* second)){
    Tree* tree = malloc(sizeof(Tree));
    tree->root = NULL;
    tree->number_of_elements = 0;
    tree->compare = compare;
    return tree;
}

Node* new_node(Tree* tree, void *value){
    Node* node = malloc(sizeof(Node));
    node->tree = tree;
    node->value = value;
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void free_tree(Tree* tree){
    if(tree->root != NULL){
        free_node(tree->root);
    }
    free(tree);
}

void free_node(Node* node){
    if(node->left != NULL){
        free_node(node->left);
    }
    if(node->right != NULL){
        free_node(node->right);
    }
    free(node);
}

void make_right_child(Node* node, Node* child){
    node->right = child;
    child->parent = node;
}

void make_left_child(Node* node, Node* child){
    node->left = child;
    child->parent = node;
}

int add_value(Tree* tree, void *value){
    if(tree->number_of_elements == 0){
        tree->root = new_node(tree, value);
        tree->number_of_elements++;
        return 1;
    }
    Node* current_node = tree->root;
    int compared;
    while(current_node != NULL){
        compared = tree->compare(current_node->value, value);
        if( compared == 0){
            return 0; // value is already in tree
        }
        if(compared < 0){
            if(current_node->right == NULL){
                make_right_child(current_node, new_node(tree, value));
                tree->number_of_elements++;
                return 1;
            }
            current_node = current_node->right;
        } else {
            if(current_node->left == NULL){
                make_left_child(current_node, new_node(tree, value));
                tree->number_of_elements++;
                return 1;
            }
            current_node = current_node->left;
        }
    }
    return 0; //this point cannot be reached
}