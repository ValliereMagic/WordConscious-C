#include <stdio.h>
#include "config.h"

#ifdef HAVE_STRING_H
    #include <string.h>
#else
    #include <strings.h>
#endif

#include <stdlib.h>
#include "sodium.h"

#include "WC_linkedlist.h"

void delete_node_t(node_t* node);

//private function to allocate memory properly
void* allocate_memory(void* value, node_type_t type) {
    if (value == NULL) {
        fprintf(stderr, "Error. allocate_memory() passed NULL value to allocate memory for.\n");
        return NULL;
    }
    
    if (type == LINKEDLIST_CHAR) {
        return malloc(sizeof(char) * (strlen(value) + 1));
    
    } else if (type == LINKEDLIST_INT) {
        return malloc(sizeof(int));
    
    } else if (type == LINKEDLIST_DOUBLE) {
        return malloc(sizeof(double));
    }

    fprintf(stderr, "Error. Unknown type passed to allocate_memory()\n");
    return NULL;
}

//private function to copy values passed to allocated memory
void copy_value(void* dest, void* src, node_type_t type) {
    if (type == LINKEDLIST_CHAR) {
        strcpy(dest, src);
    
    } else if (type == LINKEDLIST_INT) {
        *(int*)dest = *(int*)src;
    
    } else if (type == LINKEDLIST_DOUBLE) {
        *(double*)dest = *(double*)src;
    }
}

//print out each value stored in the linked
//list.
void linkedlist_print(node_t* head) {
	while (head != NULL) {

        switch(head->type) {
            case LINKEDLIST_INT:
                printf("%d", *(int*)head->val);
                break;

            case LINKEDLIST_CHAR:
                printf("%s", (char*)head->val);
                break;

            case LINKEDLIST_DOUBLE:
                printf("%f", *(double*)head->val);
                break;
        }

		head = head->next;

        if (head != NULL) {
            printf("%c ", ',');
        }
	}
    printf("\n");
}

//return 1 if equal, 0 if not.
int check_equality(void* value, node_t* head) {
    if (head == NULL) {
        fprintf(stderr, "Error. NULL node passed to check_equality().\n");
        return 0;
    }

    switch(head->type) {
        case LINKEDLIST_INT: {
            return (*(int*)value == *(int*)head->val);
            break;
        }
        case LINKEDLIST_CHAR: {
            return (strcmp(head->val, value) == 0);
            break;
        }
        case LINKEDLIST_DOUBLE: {
            return (*(double*)value == *(double*)head->val);
            break;
        }
    }
    fprintf(stderr, "Error. Unknown type passed to check_equality()\n");
    return 0;
}

//takes a void pointer, and the head
//of the linkedlist as parameters.
//if the element is found in the list
//index will be returned.
//otherwise -1 will be returned.
int linkedlist_index_of(void* value, node_t* head, node_type_t type) {
    int counter = 0;

    while (head != NULL) {
        if (check_equality(value, head)) {
            return counter;
        }
        head = head->next;
        counter++;
    }

    return -1;
}

//return the amount of elements stored in
//the linked list.
int linkedlist_size(node_t* head) {
    int counter = 0;

    while(head != NULL) {
        head = head->next;
        counter++;
    }

    return counter;
}

//Return the node_t* at the index passed.
//If the index is out of bounds, NULL will
//be returned.
node_t* linkedlist_get(node_t* head, int index) {
    int counter = 0;

    if (index >= 0) {
        
        while(head != NULL) {
            
            if (counter == index) {
                return head;
            }
            
            head = head->next;
            counter++;
        }
    }
    return NULL;
}

//delete a node at a specified index.
//if the index is out of bounds
//-1 will be returned.
//1 will be returned on success.
int linkedlist_remove_at(node_t* head, int index) {
    node_t* previous = NULL;

    for (int i = 0; i < index; i++) {
        if (head == NULL) {
            return -1;
        }
        previous = head;
        head = head->next;
    }
    
    if (head == NULL || previous == NULL) {
        return -1;
    }

    previous->next = head->next;

    delete_node_t(head);
    return 1;
}

//remove a specified value from the list
//if the value is not in the list
//-1 will be returned.
//1 will be returned on success
int linkedlist_remove_value(node_t* head, void* value, node_type_t type) {
    node_t* previous = NULL;

    while (head != NULL) {
        if (check_equality(value, head)) {
            break;
        }
        previous = head;
        head = head->next;
    }

    if (previous == NULL) {
        return -1;
    
    } else {
        return linkedlist_remove_at(previous, 1);
    }
}

//set the value of a node, at a specific index
int linkedlist_set(node_t* head, void* value, int index, node_type_t type) {
    node_t* current = linkedlist_get(head, index);

    if (current == NULL) {
        fprintf(stderr, "linklist_get() returned NULL current in linkedlist_set(). Index out of bounds.\n");
        return -1;
    }

    void* new_val = allocate_memory(value, type);

    if (new_val == NULL) {
        fprintf(stderr, "Error. System out of memory.\n");
        return -1;
    }

    copy_value(new_val, value, type);
    
    free(current->val);
    current->val = new_val;
    current->type = type;

    return 1;
}

//return new linked list if
//successful. Return NULL otherwise.
node_t* linkedlist_create(void* first_val, node_type_t type) {
    node_t* head = NULL;
    head = malloc(sizeof(node_t));

    if (head != NULL) {
        //allocate memory for new value
        void* new_val = allocate_memory(first_val, type);

        if (new_val == NULL) {
            fprintf(stderr, "Error. System out of memory.\n");
            return NULL;
        }
        //copy passed value to new_val
        copy_value(new_val, first_val, type);

        head->val = new_val;
        head->type = type;
        head->next = NULL;
    }

    return head;
}

//return a clone of the passed linked list.
//return NULL on failure
node_t* linkedlist_clone(node_t* head) {
    node_t* new = NULL;

    //make sure a NULL list was not passed
    //to be copied.
    if (head == NULL) {
        fprintf(stderr, "Error. Attempting to clone NULL linked list.\n");
        return NULL;
    }

    //create the new list from the head of the passed list
    new = linkedlist_create(head->val, head->type);

    //make sure that the creation of the new list didn't fail
    if (new == NULL) {
        fprintf(stderr, "Error. Failed to make new list in linkedlist_clone().\n");
        return NULL;
    }

    //go to the next element of the list to copy before
    //adding the next element, so as to not duplicate
    //the initial element.
    head = head->next;

    //add the rest of the elements until the end of the
    //list to copy is reached.
    while(head != NULL) {
        linkedlist_add(new, head->val, head->type);
        head = head->next;
    }

    return new;

}

//return the same linked list shuffled
//in a random order.
void linkedlist_shuffle(node_t* head) {
    if (sodium_init() < 0) {
        fprintf(stderr, "Error. failed to initiate libsodium in linkedlist_shuffle().\n");
        return;
    }
    
    int size_of_current = linkedlist_size(head);
    
    for (int i = size_of_current - 1; i > 0; i--) {
        int index = randombytes_uniform(i + 1);

        //get nodes to swap
        node_t* temp = linkedlist_get(head, index);
        node_t* current_index = linkedlist_get(head, i);

        if (temp == NULL || current_index == NULL) {
            fprintf(stderr, "Error. linkedlist_get() has returned null in linkedlist_shuffle()");
            return;
        }

        node_type_t temp_type = temp->type;
        
        //make temporary copy of randomly found type
        void* temp_value = allocate_memory(temp->val, temp_type);
        copy_value(temp_value, temp->val, temp_type);
        
        //set randomly located value to current for loop index
        linkedlist_set(temp, current_index->val, 0, current_index->type);
        
        //set current index to the value that was at the randomly
        //located index
        linkedlist_set(current_index, temp_value, 0, temp_type);

        //deallocate the temporary value
        free(temp_value);
    }

}

//add a element to the end of the
//linked list.
//return 1 if successful, -1 if unsuccessful
int linkedlist_add(node_t* head, void* val, node_type_t type) {
    //find the end of the list.
    while(head->next != NULL) {
        head = head->next;
    }

    //now head is the last node in the list
    head->next = malloc(sizeof(node_t));
    
    //make sure system is not out of memory
    if (head->next == NULL) {
        fprintf(stderr, "Error. System out of memory.\n");
        return -1;
    }

    //allocate memory for the value of the new node.
    void* new_val = allocate_memory(val, type);

    if (new_val == NULL) {
        fprintf(stderr, "Error. System out of memory.\n");
        return -1;
    }
    
    //copy the passed value to the allocated memory
    copy_value(new_val, val, type);

    //set head to freshly made node.
    head = head->next;
    
    head->val = new_val;
    head->type = type;
    head->next = NULL;

    return 1;

}

//private list function for
//deleting an individual node
//from the list.
void delete_node_t(node_t* node) {
    free(node->val);
    free(node);
}

//delete linked list using head passed.
void linkedlist_delete(node_t* head) {
    int i = 0;
    node_t* temp;
    
    while(head != NULL) {
        temp = head->next;
        //printf("Deletion: %d.\n", i);
        
        delete_node_t(head);
        
        head = temp;
        i++;
    }
}
