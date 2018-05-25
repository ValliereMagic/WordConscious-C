#include "linkedlist.h"
#include <stdio.h>

#define MAX_WORD_LENGTH 128

//read all the words from the lines of the words file
//into a linked list.
node_t* read_Words(void) {
	FILE* file_ptr = fopen("words.txt", "r");
	char file_line[MAX_WORD_LENGTH];
	node_t* word_list = NULL;

	//make sure the file exists.
	if (!file_ptr) {
		return NULL;
	}

	//for every line in the file, copy the line into a new container
	//and add that containter onto the linked list.
	int counter = 0;
	char* temp;
	while(fgets(file_line, MAX_WORD_LENGTH, file_ptr) != NULL) {
		temp = malloc(MAX_WORD_LENGTH);
		char* strtok_rest = temp;

		//make sure the system is not out of memory.
		if (temp == NULL) {
			return NULL;
		}
		
		//copy the line into the new container in linked list.
		strcpy(temp, file_line);
		//remove trailing \n from fgets.
		strtok_r(temp, "\n", &strtok_rest);

		//make the list for the first entry. Otherwise
		//add onto the list.
		if (counter == 0) {
			word_list = linkedlist_create(temp, CHAR);
			counter++;
		} else {
			linkedlist_add(word_list, temp, CHAR);
		}
	}
	fclose(file_ptr);
	return word_list;
}