/*
 * main.c
 *
 *  Created on: Mar 10, 2018
 *      Author: VoidMagic
 */

#include <stdio.h>

#include "WC_linkedlist.h"
#include "words.h"
#include "tests.h"
#include "libconfig.h"
#include "config_usr.h"
#include "string.h"
#include "config.h"

void pretty_print_guess_chars(node_t* characters) {
	if (characters == NULL) {
		return;
	}
	//assume 8 characters
	//take account for the 7 commas and null terminator
	//hence double length
	int print_length = (linkedlist_size(characters) * 2);
	char pretty_chars[print_length];

	//set i to 1 to be able to exploit modulus math.
	//(separate every word except the last with a comma)
	
	//use i-1 to account for the single shift
	int i = 1;
	for (; i < print_length; i++) {
		
		//if between a character
		if ((i % 2 == 0) && (i < print_length - 1)) {
			pretty_chars[i-1] = ',';
		
		} else {
			char* value_string = characters->val;
			
			if (value_string == NULL) {
				return;
			}

			pretty_chars[i-1] = value_string[0];
			characters = characters->next;
		}
	}
	//null terminate after building string
	pretty_chars[i-1] = '\0';
	
	printf("%s\n", pretty_chars);
}

int main(void) {
	//test program functions
	run_tests();
	printf("==========================\n");
	config_t* user_config = get_user_config();
	node_t* words = read_Words();
	
	char config_ids[5][26] = {"amount_of_words_per_set",
					 "number_of_letters_per_set",
					 "minimum_word_length",
					 "maximum_word_length",
					 "min_left_hint_reveal"};

	int config_vars[] = {10, 8, 3, 7, 0};
	
	for (int i = 0; i < 5; i++) {
		config_lookup_int(user_config, config_ids[i], &config_vars[i]);
	}
	
	char running = 1;

	while(running) {
		node_t* guess_characters = generate_Guess_Characters(config_vars[1]);
		node_t* guess_words = find_words_from_chars(guess_characters, config_vars[0],
													words, config_vars[2], config_vars[3]);
		
		pretty_print_guess_chars(guess_characters);
		linkedlist_print(guess_words);

		linkedlist_delete(guess_characters);
		linkedlist_delete(guess_words);
		running = 0;
	}

	delete_user_config(user_config);
	linkedlist_delete(words);
	return 0;
}
