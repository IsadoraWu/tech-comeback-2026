#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"

Node* create_node(int data)
{
    Node* new_node = malloc(sizeof(Node));

    if (new_node == NULL)
    {
        return NULL;
    }

    new_node->data = data;
    new_node->next = NULL;

    return new_node;
}
// Prints all elements in the linked list starting from the given head node.
void print_list(Node* head)
{
    Node* current = head;

    while (current != NULL)
    {
        printf("Node data: %d\n", current->data);
        current = current->next;
    }
}
void destroy_list(Node** head)
{
    if (head == NULL || *head == NULL)
        return;

    Node* current = *head;

    while (current != NULL)
    {
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    *head = NULL;
}
void insert_front(Node** head, int data)
{
    Node* new_node = create_node(data);

    if(new_node == NULL)
    {
        return;
    }
    if(*head == NULL)
    {
        new_node->next = NULL;
    }
    else
    {
        new_node->next = *head;
    }

    *head = new_node;
}

void insert_back(Node** head, int data)
{
    if (head == NULL || *head == NULL)
        return;

    Node* current = *head;
    Node* new_node = create_node(data);

    if(new_node == NULL)
    {
        return;
    }

    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = new_node;
}

Node* search(Node* head, int data)
{
    if (head == NULL)
        return NULL;

    Node* current = head;
    while(current != NULL)
    {
        if(current->data == data)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

int delete_node(Node** head, int data)
{
    int ret_deleted = 0;

    if (head == NULL || *head == NULL)
        return ret_deleted;
    
    Node* current = *head;
    Node* previous = NULL;

    while(current != NULL)
    {
        if(current->data == data)
        {
            if(previous == NULL) // delete the first node
            {
                *head = current->next;
                free(current);
                current = *head;
            }
            else if(current->next == NULL) // delete the last node
            {
                previous->next = NULL;
                free(current);
                current = NULL;
            }
            else // delete a middle node
            {
                previous->next = current->next;
                free(current);
                current = previous->next;
            }
            ret_deleted = 1;
        }
        else
        {
            previous = current;
            current = current->next;
        }
    }
    return ret_deleted;
}
