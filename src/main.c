/*
 * main.c
 *
 *  Created on: Mar 10, 2018
 *      Author: VoidMagic
 */

#include "main.h"
#include "words.h"
#include "list_test.h"

int main(void) {
	test_list();
	
	node_t* words = read_Words();
	if (words != NULL) {
		printf("%d\n", linkedlist_size(words));
		printf("Index of fish: %d\n", linkedlist_indexOf("fish", words, CHAR));
		printf("\n");
		
		printf("Enter a 2 digit integer => ");
		int x;
		scanf("%2d", &x);
		
		node_t retrievedWord = linkedlist_get(words, x);
		printf("%s\n", (char*)retrievedWord.val);
		
		linkedlist_delete(words);
	}
	return 0;
}
