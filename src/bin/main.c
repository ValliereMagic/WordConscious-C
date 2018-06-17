/*
 * main.c
 *
 *  Created on: Mar 10, 2018
 *      Author: VoidMagic
 */

#include <stdio.h>

#include "linkedlist.h"
#include "words.h"
#include "tests.h"
#include "config.h"


int main(void) {
	test_list();
	test_words_read();
	test_character_gen();
	test_words_gen_from_chars();
	return 0;
}
