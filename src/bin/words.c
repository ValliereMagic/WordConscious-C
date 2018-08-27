#include "WC_linkedlist.h"
#include "sodium.h"
#include "config.h"

#ifdef HAVE_STRING_H
	#include <string.h>
#else
	#include <strings.h>
#endif

#include <stdlib.h>
#include <pthread.h>

#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif


#define MAX_WORD_LENGTH 128
#define MAX_FILE_LCFS 11

typedef struct {	
	node_t* words_list;
	node_t** result_words;
	node_t* chars_to_contain;
	int words_to_check;
	int* number_of_words;
	int* minimum_word_length;
	int* maximum_word_length;
	pthread_mutex_t* words_mutex;
} thread_data_t;

//read all the words from the lines of the words file
//into a linked list.
node_t* read_words(void) {
	FILE* file_ptr = fopen(DATA_DIR "/words.txt", "r");
	char file_line[MAX_WORD_LENGTH];
	node_t* word_list = NULL;

	//make sure the file exists.
	if (!file_ptr) {
		fprintf(stderr, "Error, cannot find words file.\n");
		return NULL;
	}

	//Add each line in the file to the linked list.
	int counter = 0;
	while(fgets(file_line, MAX_WORD_LENGTH, file_ptr) != NULL) {
		char* strtok_rest = file_line;
		
		//remove trailing \n from fgets
		strtok_r(file_line, "\n", &strtok_rest);

		//make the list for the first entry. Otherwise
		//add onto the list.
		if (counter == 0) {
			word_list = linkedlist_create(file_line, LINKEDLIST_CHAR);
			counter++;
		} else {
			linkedlist_add(word_list, file_line, LINKEDLIST_CHAR);
		}
	}
	fclose(file_ptr);
	return word_list;
}

//generate a set of random characters the length speficied
//containing at least one vowel.
node_t* generate_guess_characters(int number_of_chars) {

	if (sodium_init() < 0) {
		fprintf(stderr, "Error. Unable to initiate libsodium\n");
		return NULL;
	}

	node_t* guessable_characters = NULL;
	
	char possible_chars[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g',
		'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
		's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

	char must_contain_one[] = {'a', 'e', 'i', 'o', 'u'};

	for (int i = 0; i < number_of_chars; i++) {
		char char_to_add[2];

		//check to see if there is a vowel in the list of
		//generated characters right before the last
		//character is appended to the list
		//if there is no vowel in the list
		//append one as the last character
		if (i == number_of_chars - 1) {
			int one_exists = 0;

			for (int i = 0; i < 5; i++) {
				char search_char[2];
				
				search_char[0] = must_contain_one[i];
				search_char[1] = '\0';
				
				if (linkedlist_index_of(search_char, guessable_characters, LINKEDLIST_CHAR) != -1) {
					one_exists = 1;
					break;
				}
			}

			if (!one_exists) {
				int rand_vowel_index = randombytes_uniform(5);
				
				char_to_add[0] = must_contain_one[rand_vowel_index];
				char_to_add[1] = '\0';
				
				if (guessable_characters == NULL) {
					return NULL;
				}
				linkedlist_add(guessable_characters, char_to_add, LINKEDLIST_CHAR);
				break;
			}
		}

		uint32_t randInt = randombytes_uniform(26);
		
		char_to_add[0] = possible_chars[randInt];
		char_to_add[1] = '\0';

		if (guessable_characters == NULL) {
			guessable_characters = linkedlist_create(char_to_add, LINKEDLIST_CHAR);
		} else {
			linkedlist_add(guessable_characters, char_to_add, LINKEDLIST_CHAR);
		}
	}
	return guessable_characters;
}

//return number of online processors
//returns negative number on failure.
int get_num_cpu_cores(void) {
	
	//check if on windows, if so change sysconf to work.
	#ifdef _WIN32
	
		#ifndef _SC_NPROCESSORS_ONLN
			SYSTEM_INFO info;
			GetSystemInfo(&info);

			#define sysconf(a) info.dwNumberOfProcessors

			#define _SC_NPROCESSORS_ONLN
		#endif

	#endif

	#ifdef _SC_NPROCESSORS_ONLN

		return(sysconf(_SC_NPROCESSORS_ONLN));

	#else
		return -1;
	#endif

}

//Using the amount of words in the word file, find the first thread_values_size
//lowest factors of the number of words in the file.
//function returns 0 on failure, and 1 on success.
int get_possible_thread_values(int word_list_size, int* thread_values,
			int thread_values_size) {

	if (thread_values == NULL) {
		fprintf(stderr, "Error. NULL thread_values pointer "
						"passed to LCF function.\n");
		thread_values[0] = 1;
		return 0;
	}

	int value_counter = 0;
	//check each value of i to see if it is a valid factor of word_list_size
	//only keep checking until thread_values_size has been reached.
	for (int i = 1; i <= word_list_size; i++) {

		if (value_counter == thread_values_size) {
			break;
		
		} else if ((word_list_size % i == 0) &&
			(value_counter < thread_values_size)) {

			thread_values[value_counter] = i;
			value_counter++;
		
		}
	}
	return 1;
}

//get the closest number of threads to the number of
//processors that divides evenly into the number of
//words in the words file.
//returns -1 on failure.
int get_thread_number(int word_list_size) {
	int possible_thread_values[MAX_FILE_LCFS];
	int valid_values = get_possible_thread_values(word_list_size, possible_thread_values, MAX_FILE_LCFS);
	
	int num_processors = get_num_cpu_cores();
	
	if ((num_processors <= 0) || (valid_values == 0)) {
		return -1;
	}
	
	int distance = abs(possible_thread_values[0] - num_processors);
	int idx = 0;

	for (int c = 1; c < MAX_FILE_LCFS; c++) {
		int c_distance = abs(possible_thread_values[c] - num_processors);

		if (c_distance < distance) {
			idx = c;
			distance = c_distance;
		}
	}
	return possible_thread_values[idx];
}

//only allow a word to be added to the word list
//while it has less words than the number of words
//we were asked to find.
int update_word_list(node_t** list, int* number_of_words, char* word) {
	if (number_of_words == NULL) {
		fprintf(stderr, "Error, number of words passed to searcher thread is NULL\n");
		return 0;
	}
	
	if (*list == NULL) {
		*list = linkedlist_create(word, LINKEDLIST_CHAR);
		return 1;
	
	} else if (linkedlist_size(*list) < *number_of_words) {
		linkedlist_add(*list, word, LINKEDLIST_CHAR);
		return 1;
	}

	return 0;
}

//keep removing characters from character_regex as those characters
//are found in word. When the regex runs out of characters, or the
//character_regex doesn't contain a character that the word has,
//the word is determined to be invalid.
int char_list_contains_word(node_t* chars_to_contain, char* word,
		int current_word_length) {
	
	int valid = 1;
	node_t* character_regex = linkedlist_clone(chars_to_contain);
	
	for (int i = 0; i < current_word_length; i++) {

		char current_char[2];
		current_char[0] = word[i];
		current_char[1] = '\0';
		
		int index = linkedlist_index_of(current_char, character_regex, LINKEDLIST_CHAR);

		if (index != -1) {
			
			if (index != 0) {
				linkedlist_remove_at(character_regex, index);
			
			} else {
				linkedlist_set(character_regex, "\0", index, LINKEDLIST_CHAR);
			}
		
		} else {
			valid = 0;
			break;
		}
	}
	
	linkedlist_delete(character_regex);
	
	return valid;
}

//Search through from the node passed in words_list until
//the number of words to check has been reached.
//make sure that each of the words are made up of the
//characters passed in chars_to_contain using
//char_list_contains_word()
//
//if a word is determined to be valid, lock the mutex, 
//and try to add it to the list. If enough words have
//already been found, this will fail and return 0.
//then the loop will exit, and the thread will stop.
void* word_searcher_thread(void* arg) {
	thread_data_t* working_data = arg;
	node_t** result_words = working_data->result_words;
	node_t* words_list = working_data->words_list;
	node_t* chars_to_contain = working_data->chars_to_contain;
	pthread_mutex_t* words_mutex = working_data->words_mutex;
	int* number_of_words = working_data->number_of_words;

	int* minimum_word_length = working_data->minimum_word_length;
	int* maximum_word_length = working_data->maximum_word_length;
	
	//loop through each of the words assigned in working_data
	//and check their validity considering the characters passed
	//that the words need to contain.
	//if it is valid add the word to the result word list.
	for (int i = 0; i < working_data->words_to_check; i++) {
		if (words_list == NULL) {
			fprintf(stderr, "Error. word searcher thread passed NULL word list.\n");
			break;
		}

		char* current_word_to_check = words_list->val;
		int current_word_length = strlen(current_word_to_check);
		
		if ((current_word_length <= linkedlist_size(chars_to_contain)) &&
				(current_word_length >= *minimum_word_length) &&
				(current_word_length <= *maximum_word_length)) {

			//check if the word is valid considering the current chars_to_contain
			//if valid, add to the result word list.
			if (char_list_contains_word(chars_to_contain, current_word_to_check, current_word_length)) {
				pthread_mutex_lock(words_mutex);
				
				int success = update_word_list(result_words, number_of_words,
						current_word_to_check);
				
				pthread_mutex_unlock(words_mutex);
				
				if (!success) {
					break;
				}
			}
		}
			
		words_list = words_list->next;
	}
	return NULL;
}

//find words from the passed list made up only of the list
// of characters passed.
node_t* find_words_from_chars(node_t* characters, int number_of_words,
		node_t* word_list, int minimum_word_length, int maximum_word_length) {
	
	int word_list_size = linkedlist_size(word_list);

	//get the number of threads to generate based on the number of
	//online processors on the system.
	int num_threads = get_thread_number(word_list_size);

	//fail if can't find number of processors.
	if (num_threads < 0) {
		fprintf(stderr, "Unable to find the number of online processors.\n");
		return NULL;
	}

	pthread_mutex_t words_mutex = PTHREAD_MUTEX_INITIALIZER;
	node_t* result_words = NULL;

	int start_value = 0;
	int increment_value = word_list_size / num_threads;

	//threads holds each generated thread that needs to be joined back
	//thread_data holds allocated work packages for each thread to work on.
	pthread_t threads[num_threads];
	thread_data_t* thread_data[num_threads];

	for (int i = 0; i < num_threads; i++) {
		thread_data_t* current_data = malloc(sizeof(thread_data_t));
		thread_data[i] = current_data;
		int thread_creation_success;

		if (current_data == NULL) {
			fprintf(stderr, "The system is out of memory!");
			return NULL;
		}

		//assign values to the current_data package that is destined
		//for a thread.
		current_data->words_list = linkedlist_get(word_list, start_value);
		current_data->result_words = &result_words;
		current_data->chars_to_contain = characters;
		current_data->words_to_check = increment_value - 1;
		current_data->number_of_words = &number_of_words;
		current_data->minimum_word_length = &minimum_word_length;
		current_data->maximum_word_length = &maximum_word_length;
		current_data->words_mutex = &words_mutex;

		//create the thread to handle the current_data
		if ((thread_creation_success = pthread_create(&threads[i], NULL,
				&word_searcher_thread, current_data))) {
			
			fprintf(stderr, "Thread Creation Failed: %d", thread_creation_success);
		}

		start_value = start_value + increment_value;
	}

	//join back all the threads,
	//and free the the thread_data objects allocated.
	for (int i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);
		free(thread_data[i]);
	}

	return result_words;
}
