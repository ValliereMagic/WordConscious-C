#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sodium.h"

#include "WC_linkedlist.h"

void delete_node_t(node_t* node);

//private function to allocate memory properly
void* allocate_memory(void* value, nodeType_t type) {
    
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
void copy_value(void* dest, void* src, nodeType_t type) {
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
	node_t* current = head;

	while (current != NULL) {

        switch(current->type) {
            case LINKEDLIST_INT:
                printf("%d\n", *(int*)current->val);
                break;

            case LINKEDLIST_CHAR:
                printf("%s\n", (char*)current->val);
                break;

            case LINKEDLIST_DOUBLE:
                printf("%f\n", *(double*)current->val);
                break;
        }

		current = current->next;
	}
}

//return 1 if equal, 0 if not.
int check_equality(void* value, node_t* current) {
    if (current == NULL) {
        fprintf(stderr, "Error. NULL node passed to check_equality().\n");
        return 0;
    }

    switch(current->type) {
        case LINKEDLIST_INT: {
            return (*(int*)value == *(int*)current->val);
            break;
        }
        case LINKEDLIST_CHAR: {
            return (strcmp(current->val, value) == 0);
            break;
        }
        case LINKEDLIST_DOUBLE: {
            return (*(double*)value == *(double*)current->val);
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
int linkedlist_indexOf(void* value, node_t* head, nodeType_t type) {
    node_t* current = head;
    
    int counter = 0;

    while (current != NULL) {
        if (check_equality(value, current)) {
            return counter;
        }
        current = current->next;
        counter++;
    }

    return -1;
}

//return the amount of elements stored in
//the linked list.
int linkedlist_size(node_t* head) {
    node_t* current = head;
    int counter = 0;

    while(current != NULL) {
        current = current->next;
        counter++;
    }

    return counter;
}

//Return the node_t* at the index passed.
//If the index is out of bounds, NULL will
//be returned.
node_t* linkedlist_get(node_t* head, int index) {
    node_t* current = head;

    int counter = 0;

    if (index >= 0) {
        
        while(current != NULL) {
            
            if (counter == index) {
                return current;
            }
            
            current = current->next;
            counter++;
        }
    }
    return NULL;
}

//delete a node at a specified index.
//if the index is out of bounds
//-1 will be returned.
//1 will be returned on success.
int linkedlist_removeAt(node_t* head, int index) {
    node_t* current = head;
    node_t* previous = NULL;

    for (int i = 0; i < index; i++) {
        if (current == NULL) {
            return -1;
        }
        previous = current;
        current = current->next;
    }
    
    if (current == NULL || previous == NULL) {
        return -1;
    }

    previous->next = current->next;

    delete_node_t(current);
    return 1;
}

//remove a specified value from the list
//if the value is not in the list
//-1 will be returned.
//1 will be returned on success
int linkedlist_removeValue(node_t* head, void* value, nodeType_t type) {
    node_t* current = head;
    node_t* previous = NULL;

    while (current != NULL) {
        if (check_equality(value, current)) {
            break;
        }
        previous = current;
        current = current->next;
    }

    if (previous == NULL) {
        return -1;
    
    } else {
        return linkedlist_removeAt(previous, 1);
    }
}

//set the value of a node, at a specific index
int linkedlist_set(node_t* head, void* value, int index, nodeType_t type) {
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
node_t* linkedlist_create(void* firstVal, nodeType_t type) {
    node_t* head = NULL;
    head = malloc(sizeof(node_t));

    if (head != NULL) {
        //allocate memory for new value
        void* new_val = allocate_memory(firstVal, type);

        if (new_val == NULL) {
            fprintf(stderr, "Error. System out of memory.\n");
            return NULL;
        }
        //copy passed value to new_val
        copy_value(new_val, firstVal, type);

        head->val = new_val;
        head->type = type;
        head->next = NULL;
    }

    return head;
}

//return a clone of the passed linked list.
//return NULL on failure
node_t* linkedlist_clone(node_t* head) {
    node_t* current = head;
    node_t* new = NULL;

    //make sure a NULL list was not passed
    //to be copied.
    if (current == NULL) {
        fprintf(stderr, "Error. Attempting to clone NULL linked list.\n");
        return NULL;
    }

    //create the new list from the head of the passed list
    new = linkedlist_create(current->val, current->type);

    //make sure that the creation of the new list didn't fail
    if (new == NULL) {
        fprintf(stderr, "Error. Failed to make new list in linkedlist_clone().\n");
        return NULL;
    }

    //go to the next element of the list to copy before
    //adding the next element, so as to not duplicate
    //the initial element.
    current = current->next;

    //add the rest of the elements until the end of the
    //list to copy is reached.
    while(current != NULL) {
        linkedlist_add(new, current->val, current->type);
        current = current->next;
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

    node_t* current = head;
    
    int sizeOfCurrent = linkedlist_size(current);
    
    for (int i = sizeOfCurrent - 1; i > 0; i--) {
        int index = randombytes_uniform(i + 1);

        //get nodes to swap
        node_t* temp = linkedlist_get(current, index);
        node_t* currentIndex = linkedlist_get(current, i);

        if (temp == NULL || currentIndex == NULL) {
            fprintf(stderr, "Error. linkedlist_get() has returned null in linkedlist_shuffle()");
            return;
        }

        nodeType_t tempType = temp->type;
        
        //make temporary copy of randomly found type
        void* tempValue = allocate_memory(temp->val, tempType);
        copy_value(tempValue, temp->val, tempType);
        
        //set randomly located value to current for loop index
        linkedlist_set(temp, currentIndex->val, 0, currentIndex->type);
        
        //set current index to the value that was at the randomly
        //located index
        linkedlist_set(currentIndex, tempValue, 0, tempType);

        //deallocate the temporary value
        free(tempValue);
    }

}

//add a element to the end of the
//linked list.
//return 1 if successful, -1 if unsuccessful
int linkedlist_add(node_t* head, void* val, nodeType_t type) {
    node_t* current = head;

    //find the end of the list.
    while(current->next != NULL) {
        current = current->next;
    }

    //now current is the last node in the list
    current->next = malloc(sizeof(node_t));
    
    //make sure system is not out of memory
    if (current->next == NULL) {
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

    //set current to freshly made node.
    current = current->next;
    
    current->val = new_val;
    current->type = type;
    current->next = NULL;

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
    node_t* current = head;

    int i = 0;
    node_t* temp;
    
    while(current != NULL) {
        temp = current->next;
        //printf("Deletion: %d.\n", i);
        
        delete_node_t(current);
        
        current = temp;
        i++;
    }
}
