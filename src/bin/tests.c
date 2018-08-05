#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "linkedlist.h"
#include "words.h"
#include "libconfig.h"
#include "config_usr.h"
#include "hints.h"

/*
* Test program functions.
*/

void test_list(void) {

    printf("\n\n===Testing LinkedList===\n\n");

	char helloList[sizeof("Hello World") + 1] = "Hello World";
	node_t* listHead = linkedlist_create(helloList, LINKEDLIST_CHAR);

	int test = 6;
	linkedlist_add(listHead, &test, LINKEDLIST_INT);

	double testDouble = 3.14159265;
	linkedlist_add(listHead, &testDouble, LINKEDLIST_DOUBLE);

	printf("Index of 6 in the list: %d\n", linkedlist_indexOf(&test, listHead, LINKEDLIST_INT));
	printf("Index of \"Hello World\" in the list: %d\n", linkedlist_indexOf(helloList, listHead, LINKEDLIST_CHAR));
	printf("Index of PI in the list: %d\n", linkedlist_indexOf(&testDouble, listHead, LINKEDLIST_DOUBLE));

	node_t* testNode = linkedlist_get(listHead, 1);
	printf("Value at %d: %d\n", 1, *(int*)testNode->val);

	printf("===printing list 1st time===\n");
	linkedlist_print(listHead);

	printf("===Shuffling the list===\n");
	linkedlist_shuffle(listHead);

	printf("===Printing shuffled list===\n");
	linkedlist_print(listHead);

	printf("===Cloning list===\n");
	node_t* clone_node = linkedlist_clone(listHead);

	printf("===Printing cloned list===\n");
	linkedlist_print(clone_node);

	linkedlist_delete(clone_node);

	printf("remove codes: %d %d\n", linkedlist_removeAt(listHead, 0), linkedlist_removeAt(listHead, 1));

	printf("===printing list 2nd time===\n");
	linkedlist_print(listHead);

	printf("Remove using value code: %d\n", linkedlist_removeValue(listHead, &testDouble, LINKEDLIST_DOUBLE));

	printf("===printing list 3rd time===\n");
	linkedlist_print(listHead);

	int testInt = 64;

	linkedlist_set(listHead, &testInt, 0, LINKEDLIST_INT);

	printf("===printing list 4th time===\n");
	linkedlist_print(listHead);

	printf("===deleting list===\n");
	linkedlist_delete(listHead);
}

void test_words_read(void) {
	node_t* words = read_Words();
	
	if (words != NULL) {
		printf("%d\n", linkedlist_size(words));
		printf("Index of fish: %d\n", linkedlist_indexOf("fish", words, LINKEDLIST_CHAR));
		printf("\n");
		
		printf("Enter a 2 digit integer => ");
		int x;
		scanf("%2d", &x);
		
		node_t* retrievedWord = linkedlist_get(words, x);
		printf("%s\n", (char*)retrievedWord->val);
		
		linkedlist_delete(words);
	}
}

void test_character_gen(void) {
	node_t* characters = generate_Guess_Characters(8);
	linkedlist_print(characters);
	printf("Amount of characters returned: %d\n", linkedlist_size(characters));
	linkedlist_delete(characters);
}

void test_words_gen_from_chars(void) {
	node_t* words = read_Words();
	node_t* characters = generate_Guess_Characters(8);
	node_t* resultWords = find_words_from_chars(characters, 5, words);

	printf("Characters:\n");
	linkedlist_print(characters);
	printf("Found Words:\n");
	linkedlist_print(resultWords);

	linkedlist_delete(words);
	linkedlist_delete(characters);
	linkedlist_delete(resultWords);
}

void test_config_file(void) {
	config_t* user_config = get_user_config();
	
	int words_per_set;
	config_lookup_int(user_config, "amount_of_words_per_set", &words_per_set);
	printf("Words per set: %d\n", words_per_set);

	int number_of_letters_set;
	config_lookup_int(user_config, "number_of_letters_per_set", &number_of_letters_set);
	printf("Letters per set: %d\n", number_of_letters_set);

	int minimum_word_length;
	config_lookup_int(user_config, "minimum_word_length", &minimum_word_length);
	printf("Minimum word length: %d\n", minimum_word_length);

	int maximum_word_length;
	config_lookup_int(user_config, "maximum_word_length", &maximum_word_length);
	printf("Maximum word length: %d\n", maximum_word_length);

	int min_left_hint_reveal;
	config_lookup_int(user_config, "min_left_hint_reveal", &min_left_hint_reveal);
	printf("minimum left of word using hints: %d\n", min_left_hint_reveal);

	delete_user_config(user_config);
}

void test_hints(void) {
	hint_data_t* hints = create_hint_type();
	hints->min_chars_unrevealed = 2;
	node_t* words = read_Words();
	
	for (int i = 0; i < 12; i++) {
		get_hint(hints, words);

		if (i == 6) {
			linkedlist_shuffle(words);
		}

		printf("word: %s\n", hints->result);
	}
	delete_hint_type(hints);
	linkedlist_delete(words);
}
