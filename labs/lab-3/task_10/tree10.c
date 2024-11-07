#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/types.h>
#include "l_10.h"

Node* create_node(char data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = data;
    new_node->child_count = 0;
    new_node->child_capacity = 2;
    new_node->children = (Node**)malloc(new_node->child_capacity * sizeof(Node*));
    if (new_node->children == NULL){
        return NULL;
    }

    return new_node;
}

errorCode add_child(Node* parent, Node* child) {
    if(parent->child_capacity == parent->child_count){
        parent->child_capacity *= 2;
        Node** new = (Node**)realloc(parent->children, parent->child_capacity * sizeof(Node*));
        if (new == NULL){
            return OPT_ERROR_MEMORY;
        }
        parent->children = new;
    }
    parent->children[parent->child_count++] = child;
    return OPT_SUCCESS;
}

void free_tree(Node* root) {
    if (root == NULL) return;
    for (int i = 0; i < root->child_count; i++) {
        free_tree(root->children[i]);
    }
    free(root->children);
    free(root);
}

void print_tree(FILE* file, Node* root, int h) {
    if (root == NULL) return;
    for (int i = 0; i < h; i++){
        fprintf(file, "  ");
    }
    fprintf(file, "%c\n", root->data);
    for (int i = 0; i < root->child_count; i++) {
        print_tree(file, root->children[i], h + 1);
    }
}

