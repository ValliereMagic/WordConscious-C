#include "main.h"

/*
* Test the functions of the list
*/

void test_list(void) {

    printf("\n\n===Testing LinkedList===\n\n");

	char* helloList = malloc(sizeof("Hello World"));
	strcpy(helloList, "Hello World");
	node_t* listHead = linkedlist_create(helloList, CHAR);

	if (listHead == NULL) {
		return;
	}

	int* test = malloc(4);
	*test = 6;
	linkedlist_add(listHead, test, INT);

	double* testDouble = malloc(8);
	*testDouble = 3.14159265;
	linkedlist_add(listHead, testDouble, DOUBLE);

	printf("Index of 6 in the list: %d\n", linkedlist_indexOf(test, listHead, INT));
	printf("Index of \"Hello World\" in the list: %d\n", linkedlist_indexOf(helloList, listHead, CHAR));
	printf("Index of PI in the list: %d\n", linkedlist_indexOf(testDouble, listHead, DOUBLE));

	node_t testNode = linkedlist_get(listHead, 1);
	printf("Value at %d: %d\n", 1, *(int*)testNode.val);

	printf("===printing list 1st time===\n");
	linkedlist_print(listHead);

	printf("remove codes: %d %d\n", linkedlist_removeAt(listHead, 0), linkedlist_removeAt(listHead, 1));

	printf("===printing list 2nd time===\n");
	linkedlist_print(listHead);

	linkedlist_removeValue(listHead, testDouble, DOUBLE);

	printf("===printing list 3rd time===\n");
	linkedlist_print(listHead);

	int* testInt = malloc(4);
	*testInt = 64;

	linkedlist_set(listHead, testInt, 0, INT);

	printf("===printing list 4th time===\n");
	linkedlist_print(listHead);

	printf("===deleting list===\n");
	linkedlist_delete(listHead);
}
