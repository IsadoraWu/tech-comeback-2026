#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node
{
    int data;
    struct Node* next;
} Node;

Node* create_node(int data);

void print_list(Node* head);

void destroy_list(Node** head);

void insert_front(Node** head, int data);

void insert_back(Node** head, int data);

Node* search(Node* head, int data);

int delete_node(Node** head, int data);

#endif