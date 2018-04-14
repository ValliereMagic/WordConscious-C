#include "linkedlist.h"

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

//takes a void pointer, and the head
//of the linkedlist as parameters.
//if the element is found in the list
//index will be returned.
//otherwise -1 will be returned.
int linkedlist_indexOf(void* value, node_t* head, nodeType_t type) {
    node_t* current = head;

    int counter = 0;

    switch(type) {
        case INT: {

            int* searchFor = (int*) value;

            if (searchFor == NULL) {
                return -1;
            }

            while(current != NULL) {
                if (current->type == INT && *searchFor == *(int*)current->val) {
                    return counter;
                }
                current = current->next;
                counter++;
            }
            break;
        }
        case CHAR: {

            char* searchFor = (char*) value;

            if(searchFor == NULL) {
                return -1;
            }

            while(current != NULL) {
                if (current->type == CHAR && strcmp(searchFor, current->val) == 0) {
                    return counter;
                }
                current = current->next;
                counter++;
            }
            break;
        }

        case DOUBLE: {

            double* searchFor = (double*) value;

            if (searchFor == NULL) {
                return -1;
            }

            while(current != NULL) {
                if (current->type == DOUBLE && *searchFor == *(double*)current->val) {
                    return counter;
                }
                current = current->next;
                counter++;
            }
            break;
        }
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

//locate element at specified index, and
//return a copy. If the index is out of
//bounds a node_t with NULL values and
//a type of INT will be returned.
node_t linkedlist_get(node_t* head, int index) {
    node_t* current = head;
    node_t returnedNode;
    int counter = 0;

    if (index >= 0) {
        
        while(current != NULL) {
            
            if (counter == index) {
                returnedNode = *current;
                return returnedNode;
            }
            
            current = current->next;
            counter++;
        }
    }

    returnedNode.val = NULL;
    returnedNode.type = INT;
    returnedNode.next = NULL;

    return returnedNode;
}

//return new linked list if
//successful. Return NULL otherwise.
node_t* linkedlist_create(void* firstVal, nodeType_t type) {
    node_t* head = NULL;
    head = malloc(sizeof(node_t));

    if (head != NULL) {
        head->val = firstVal;
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

    //set current to freshly made node.
    current = current->next;
    
    current->val = val;
    current->type = type;
    current->next = NULL;

    return 1;

}

//delete linked list using head passed.
void linkedlist_delete(node_t* head) {
    node_t* current = head;

    int i = 0;
    node_t* temp;
    
    while(current != NULL) {
        temp = current->next;
        printf("Deletion: %d.\n", i);
        free(current->val);
        free(current);
        
        current = temp;
        i++;
    }
}
