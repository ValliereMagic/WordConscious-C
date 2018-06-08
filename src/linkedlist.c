#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "linkedlist.h"

void delete_node_t(node_t* node);

//private function to allocate memory properly
void* allocate_memory(void* value, nodeType_t type) {
    if (type == CHAR) {
        if (value == NULL) {
            return NULL;
        }
        return malloc(sizeof(char) * (strlen(value) + 1));
    
    } else if (type == INT) {
        return malloc(sizeof(int));
    
    } else if (type == DOUBLE) {
        return malloc(sizeof(double));
    }

}

//private function to copy values passed to allocated memory
void copy_value(void* dest, void* src, nodeType_t type) {
    if (type == CHAR) {
        strcpy(dest, src);
    
    } else if (type == INT) {
        *(int*)dest = *(int*)src;
    
    } else if (type == DOUBLE) {
        *(double*)dest = *(double*)src;
    }
}

//print out each value stored in the linked
//list.
void linkedlist_print(node_t* head) {
	node_t* current = head;

	while (current != NULL) {

        switch(current->type) {
            case INT:
                printf("%d\n", *(int*)current->val);
                break;

            case CHAR:
                printf("%s\n", (char*)current->val);
                break;

            case DOUBLE:
                printf("%f\n", *(double*)current->val);
                break;
        }

		current = current->next;
	}
}

//find the index of an integer in the linked list
//return -1 if not found
int indexOf_INT(int* value, node_t* current) {
    int counter = 0;

    if (value == NULL) {
        return -1;
    }

    while(current != NULL) {
        if (current->type == INT && *value == *(int*)current->val) {
            return counter;
        }
        current = current->next;
        counter++;
    }
    return -1;
}

//find the index of a character in the linked list
//return -1 if not found
int indexOf_CHAR(char* value, node_t* current) {
    int counter = 0;

    if(value == NULL) {
        return -1;
    }

    while(current != NULL) {
        if (current->type == CHAR && strcmp(value, current->val) == 0) {
            return counter;
        }
        current = current->next;
        counter++;
    }
    return -1;
}

//find the index of a double in the linked list
//return -1 if not found
int indexOf_DOUBLE(double* value, node_t* current) {
    int counter = 0;

    if (value == NULL) {
        return -1;
    }

    while(current != NULL) {
        if (current->type == DOUBLE && *value == *(double*)current->val) {
            return counter;
        }
        current = current->next;
        counter++;
    }
    return -1;
}

//takes a void pointer, and the head
//of the linkedlist as parameters.
//if the element is found in the list
//index will be returned.
//otherwise -1 will be returned.
int linkedlist_indexOf(void* value, node_t* head, nodeType_t type) {
    node_t* current = head;

    switch(type) {
        case INT:
            return indexOf_INT((int*)value, current);
            break;
        
        case CHAR: 
            return indexOf_CHAR((char*)value, current);
            break;

        case DOUBLE: 
            return indexOf_DOUBLE((double*)value, current);
            break;
    }

    return -1;
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
    int location = linkedlist_indexOf(value, head, type);

    if (location == -1) {
        return location;
    }

    return linkedlist_removeAt(head, location);
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

//private list function for getting the
//node pointer at a specific index
node_t* linkedlist_get_p(node_t* head, int index) {
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

//locate element at specified index, and
//return a copy. If the index is out of
//bounds a node_t with NULL values and
//a type of INT will be returned.
node_t linkedlist_get(node_t* head, int index) {
    node_t* current = linkedlist_get_p(head, index);
    node_t returnedNode;

    if (current != NULL) {
        returnedNode.val = current->val;
        returnedNode.type = current->type;
        returnedNode.next = NULL;
    } else {
        returnedNode.val = NULL;
        returnedNode.type = INT;
        returnedNode.next = NULL;
    }

    return returnedNode;
}

//set the value of a node, at a specific index
int linkedlist_set(node_t* head, void* value, int index, nodeType_t type) {
    node_t* current = linkedlist_get_p(head, index);

    if (current == NULL) {
        return -1;
    }

    void* new_val = allocate_memory(value, type);

    if (new_val == NULL) {
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
        return -1;
    }

    //allocate memory for the value of the new node.
    void* new_val = allocate_memory(val, type);

    if (new_val == NULL) {
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
