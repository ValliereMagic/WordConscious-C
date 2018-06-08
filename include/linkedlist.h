#ifndef LINKEDLIST_H
#define LINKEDLIST_H

typedef enum {
    INT, CHAR, DOUBLE
} nodeType_t;

typedef struct node {
    void* val;
    nodeType_t type;
    struct node* next;
} node_t;

//returns the head of the new linked list, with a pointer in it
//holding the first value.
node_t* linkedlist_create(void* firstVal, nodeType_t type);

//print out each element in the linked list.
void linkedlist_print(node_t* head);

//add a new element to the linked list
//a return of 1 means successful, -1 means failure
int linkedlist_add(node_t* head, void* val, nodeType_t type);

//remove a value at a specified index
//a return of 1 means successful, -1 means failure
int linkedlist_removeAt(node_t* head, int index);

//remove a passed value from the list
//a return of 1 means successful, -1 means failure
int linkedlist_removeValue(node_t* head, void* value, nodeType_t type);

//set a node already in the list to a new value
//a return of 1 means successful, -1 means failure
int linkedlist_set(node_t* head, void* value, int index, nodeType_t type);

//return a copy of the node at the index passed.
//If the index is out of bounds, a node_t with
//NULL values and a type of INT will be
//returned.
node_t linkedlist_get(node_t* head, int index);

//find the index of the node passed in the linkedlist
//if it is present. If the element is not found,
//return -1.
int linkedlist_indexOf(void* value, node_t* head, nodeType_t type);

//return the amount of elements in the linked list.
int linkedlist_size(node_t* head);

//free all of the elements, and values in the list.
void linkedlist_delete(node_t* head);

#endif
