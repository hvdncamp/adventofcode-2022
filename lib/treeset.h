//
// Created by hvdncamp on 9/12/2022.
//

#ifndef ADVENTOFCODE_2022_TREESET_H
#define ADVENTOFCODE_2022_TREESET_H

typedef struct node Node;
typedef struct tree Tree;

struct node {
    Tree* tree;
    Node* parent;
    Node* left;
    Node* right;
    void* value;
};

struct tree {
    Node* root;
    int number_of_elements;
    int (*compare)(void* first, void* second); //functiepointer om twee elementen te vergelijken
};

Tree* new_tree(int (*compare)(void* first, void* second));
Node* new_node(Tree* tree, void *value);
void free_tree(Tree* tree);
void free_node(Node* node);
int add_value(Tree* tree, void *value);

#endif //ADVENTOFCODE_2022_TREESET_H
