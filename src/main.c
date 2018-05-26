/*
 * main.c
 *
 *  Created on: Mar 10, 2018
 *      Author: VoidMagic
 */

#include "main.h"
#include "words.h"
#include "tests.h"

int main(void) {
	test_list();
	test_words_read();

	node_t* characters = generate_Guess_Characters(8);
	linkedlist_print(characters);
	printf("Amount of characters returned: %d\n", linkedlist_size(characters));
	linkedlist_delete(characters);
	return 0;
}
