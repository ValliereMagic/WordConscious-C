#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef enum {
    LINKEDLIST_INT, LINKEDLIST_CHAR, LINKEDLIST_DOUBLE
} node_type_t;

typedef struct node {
    void* val;
    node_type_t type;
    struct node* next;
} node_t;

//returns the head of the new linked list, with a pointer in it
//holding the first value.
node_t* linkedlist_create(void* firstVal, node_type_t type);

//print out each element in the linked list.
void linkedlist_print(node_t* head);

//add a new element to the linked list
//a return of 1 means successful, -1 means failure
int linkedlist_add(node_t* head, void* val, node_type_t type);

//return a clone of the passed linked list.
//return NULL on failure
node_t* linkedlist_clone(node_t* head);

//return the same linked list shuffled
//in a random order.
void linkedlist_shuffle(node_t* head);

//remove a value at a specified index
//a return of 1 means successful, -1 means failure
int linkedlist_remove_at(node_t* head, int index);

//remove a passed value from the list
//a return of 1 means successful, -1 means failure
int linkedlist_remove_value(node_t* head, void* value, node_type_t type);

//set a node already in the list to a new value
//a return of 1 means successful, -1 means failure
int linkedlist_set(node_t* head, void* value, int index, node_type_t type);

//Return the node_t* at the index passed.
//If the index is out of bounds, NULL will
//be returned.
node_t* linkedlist_get(node_t* head, int index);

//find the index of the node passed in the linkedlist
//if it is present. If the element is not found,
//return -1.
int linkedlist_index_of(void* value, node_t* head, node_type_t type);

//return the amount of elements in the linked list.
int linkedlist_size(node_t* head);

//free all of the elements, and values in the list.
void linkedlist_delete(node_t* head);

#endif
