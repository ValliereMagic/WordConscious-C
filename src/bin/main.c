/*
 * main.c
 *
 *  Created on: Mar 10, 2018
 *      Author: VoidMagic
 */

#include <stdio.h>
#include <ctype.h>
#include "config.h"

#ifdef HAVE_STRING_H
	#include "string.h"
#else
	#include "strings.h"
#endif

#include "WC_linkedlist.h"
#include "words.h"
#include "tests.h"
#include "libconfig.h"
#include "config_usr.h"
#include "hints.h"

//print the user help screen.
void print_help(void) {
	printf("Available Commands: \n"
           "   h -> get a hint on a random un-guessed word.\n"
           "   f -> list the words found by the player.\n"
           "   g -> list the words that the player has guessed.\n"
           "   s -> shuffle the letters.\n"
           "   w -> list how many words are left.\n"
           "   q -> quit the game.\n");
}

//print out the list of things that the user has guessed;
//that aren't words in the list.
void print_user_inputs(node_t* user_inputs) {
	if (user_inputs == NULL) {
		printf("You haven't guessed any words yet!\n");
	
	} else {
		printf("Guessed Words: ");
		linkedlist_print(user_inputs);
	}
}

//print out the list of words the user has successfully guessed.
void print_guessed_words(node_t* guessed_words) {
	if (guessed_words == NULL) {
		printf("You haven't guessed any words correctly yet!\n");
	
	} else {
		printf("Guessed Words: ");
		linkedlist_print(guessed_words);
	}
}

//go through an entire string and set each character to lower case.
void str_to_lower(char* string, int input_size) {
	for (int i = 0; i < input_size; i++) {
		string[i] = tolower(string[i]);
	}
}

//check to see if the guess_words linked list is empty. If it is...
//then the game is over.
//return 1 if over.
//0 if still words left.
int get_words_left(node_t* guess_words) {
	char* first_value = guess_words->val;
	
	if (first_value == NULL) {
		fprintf(stderr, "Error. A word in the list is a NULL pointer.\n");
		return 0;
	}
	int words_left = linkedlist_size(guess_words);

	if (strcmp(first_value, "\0") == 0) {
		words_left--;
	}

	return words_left;
}

//update list of strings that the user has entered into the prompt.
void update_user_inputs_list(node_t** user_inputs, char* user_input) {
	
	if (*user_inputs == NULL) {
		*user_inputs = linkedlist_create(user_input, LINKEDLIST_CHAR);
	
	} else {
		linkedlist_add(*user_inputs, user_input, LINKEDLIST_CHAR);
	}
}

//remove a word from the list if the user guesses it correctly.
//return 1 if a word was removed.
//return 0 if a word was not removed.
int remove_if_valid(node_t* guess_words, char* user_guess, node_t** guessed_words) {
	int guess_index = linkedlist_index_of(user_guess, guess_words, LINKEDLIST_CHAR);
	
	if (guess_index != -1) {
		(guess_index == 0) ? linkedlist_set(guess_words, "\0", 0, LINKEDLIST_CHAR) :
				linkedlist_remove_at(guess_words, guess_index);

		//add the valid user guess to the guessed word list.
		if (*guessed_words == NULL) {
			*guessed_words = linkedlist_create(user_guess, LINKEDLIST_CHAR);
		
		} else {
			linkedlist_add(*guessed_words, user_guess, LINKEDLIST_CHAR);
		}

		return 1;
	}
	return 0;
}

//delete and cleanup various lists.
void cleanup_lists(node_t* guess_characters, node_t* guess_words, node_t* guessed_words,
		node_t* user_inputs) {
	linkedlist_delete(guess_characters);
	linkedlist_delete(guess_words);

	if (guessed_words != NULL) {
		linkedlist_delete(guessed_words);
	}

	if (user_inputs != NULL) {
		linkedlist_delete(user_inputs);
	}
}

int main(void) {
	//test program functions
	//run_tests();
	
	//pull in user configuration from file.
	config_t* user_config = get_user_config();
	//read in words from words file.
	node_t* words = read_words();
	
	char config_ids[5][26] = {"amount_of_words_per_set",
					 "number_of_letters_per_set",
					 "minimum_word_length",
					 "maximum_word_length",
					 "min_left_hint_reveal"};

	int config_vars[] = {10, 8, 3, 7, 0};
	
	//go through all the id keys, and pull their integer values into config_vars[].
	for (int i = 0; i < 5; i++) {
		config_lookup_int(user_config, config_ids[i], &config_vars[i]);
	}

	//Create hint object.
	hint_data_t* hint_data = create_hint_type(config_vars[4]);
	
	//specifies that the program is running. When this is set to 0,
	//the program will exit.
	char running = 1;

	//keep running main game until user quits. (makes running false)
	while (running) {
		//Create required lists.
		node_t* guess_characters = generate_guess_characters(config_vars[1]);
		node_t* guess_words = find_words_from_chars(guess_characters, config_vars[0],
													words, config_vars[2], config_vars[3]);
		//initialized to NULL, created when needed.
		node_t* guessed_words = NULL;
		node_t* user_inputs = NULL;
		
		int words_to_guess = linkedlist_size(guess_words);
		
		if (words_to_guess == 0) {
			cleanup_lists(guess_characters, guess_words, guessed_words, user_inputs);
			continue;
		}

		linkedlist_print(guess_characters);
		printf("===========================\n");
		printf("Enter a word to guess, or type ? to list commands.\n");
		
		//take user guesses until all the words are guessed.
		char action[128];
		while (running) {
			printf("> ");
			scanf("%127s", &action);
			
			int input_size = strlen(action);
			str_to_lower(action, input_size);

			if (input_size == 1) {
				
				switch (action[0]) {
					case '?': {
						print_help();
						break;
					}
					case 'q': {
						running = 0;
						break;
					}
					case 's': {
						linkedlist_shuffle(guess_characters);
						linkedlist_print(guess_characters);
						break;
					}
					case 'h': {
						get_hint(hint_data, guess_words);
						printf("%s\n", hint_data->result);
						break;
					}
					case 'w': {
						printf("There is / are: %d word(s) left.\n", get_words_left(guess_words));
						break;
					}
					case 'f': {
						print_guessed_words(guessed_words);
						break;
					}
					case 'g': {
						print_user_inputs(user_inputs);
						break;
					}
				}
			
			} else {
				int value_removed = remove_if_valid(guess_words, action, &guessed_words);
				int words_left = get_words_left(guess_words);
				
				if (words_left == 0) {
					printf("Last word found! Nice job.\n");
					break;
				
				} else if (value_removed) {
					printf("Word Found! Words left: %d\n", words_left);
				
				} else {
					update_user_inputs_list(&user_inputs, action);
				}
			}
		}

		cleanup_lists(guess_characters, guess_words, guessed_words, user_inputs);
	}

	delete_hint_type(hint_data);
	delete_user_config(user_config);
	linkedlist_delete(words);
	return 0;
}
