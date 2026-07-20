#include <stdio.h>
#include "linked_list.h"

int main(void)
{
    Node* node = create_node(10);

    if (node == NULL)
    {
        printf("Failed to create node.\n");
        return 1;
    }

    print_list(node);
    
    destroy_list(&node);

    return 0;
}