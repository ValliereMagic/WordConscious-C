#include "linkedlist.h"
#include "sodium.h"
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif


#define MAX_WORD_LENGTH 128

typedef struct {	
	node_t* wordsList;
	node_t** resultWords;
	node_t* charsToContain;
	int wordsToCheck;
	int* numberOfWords;
	pthread_mutex_t* wordsMutex;
} thread_data_t;

//read all the words from the lines of the words file
//into a linked list.
node_t* read_Words(void) {
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
node_t* generate_Guess_Characters(int number_Of_Chars) {

	if (sodium_init() < 0) {
		fprintf(stderr, "Error. Unable to initiate libsodium\n");
		return NULL;
	}

	node_t* guessable_Characters = NULL;
	
	char possibleChars[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g',
		'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
		's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

	char mustContainOne[] = {'a', 'e', 'i', 'o', 'u'};

	for (int i = 0; i < number_Of_Chars; i++) {
		char charToAdd[2];

		//check to see if there is a vowel in the list of
		//generated characters right before the last
		//character is appended to the list
		//if there is no vowel in the list
		//append one as the last character
		if (i == number_Of_Chars - 1) {
			int oneExists = 0;

			for (int i = 0; i < 5; i++) {
				char searchChar[2];
				
				searchChar[0] = mustContainOne[i];
				searchChar[1] = '\0';
				
				if (linkedlist_indexOf(searchChar, guessable_Characters, LINKEDLIST_CHAR) != -1) {
					oneExists = 1;
					break;
				}
			}

			if (!oneExists) {
				int randVowelIndex = randombytes_uniform(5);
				
				charToAdd[0] = mustContainOne[randVowelIndex];
				charToAdd[1] = '\0';
				
				if (guessable_Characters == NULL) {
					return NULL;
				}
				linkedlist_add(guessable_Characters, charToAdd, LINKEDLIST_CHAR);
				break;
			}
		}

		uint32_t randInt = randombytes_uniform(26);
		
		charToAdd[0] = possibleChars[randInt];
		charToAdd[1] = '\0';

		if (guessable_Characters == NULL) {
			guessable_Characters = linkedlist_create(charToAdd, LINKEDLIST_CHAR);
		} else {
			linkedlist_add(guessable_Characters, charToAdd, LINKEDLIST_CHAR);
		}
	}
	return guessable_Characters;
}

//return number of online processors
//returns negative number on failure.
int get_num_cpu_cores(void) {
	long numProcessors = -1;
	
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

//get the closest number of threads to the number of
//processors that divides evenly into the number of
//words in the words file.
//returns -1 on failure.
int get_thread_number() {
	int possibleThreadValues[] = {1, 2, 3, 4, 5, 6, 10, 12, 15, 20, 30};
	int numProcessors = get_num_cpu_cores();
	
	if (numProcessors <= 0) {
		return -1;
	}
	
	int distance = abs(possibleThreadValues[0] - numProcessors);
	int idx = 0;

	for (int c = 1; c < 11; c++) {
		int cDistance = abs(possibleThreadValues[c] - numProcessors);

		if (cDistance < distance) {
			idx = c;
			distance = cDistance;
		}
	}
	return possibleThreadValues[idx];
}

//only allow a word to be added to the word list
//while it has less words than the number of words
//we were asked to find.
int update_word_list(node_t** list, int* numberOfWords, char* word) {
	if (numberOfWords == NULL) {
		fprintf(stderr, "Error, number of words passed to searcher thread is NULL\n");
		return 0;
	}
	
	if (*list == NULL) {
		*list = linkedlist_create(word, LINKEDLIST_CHAR);
		return 1;
	
	} else if (linkedlist_size(*list) < *numberOfWords) {
		linkedlist_add(*list, word, LINKEDLIST_CHAR);
		return 1;
	}

	return 0;
}

//keep removing characters from characterRegex as those characters
//are found in word. When the regex runs out of characters, or the
//characterRegex doesn't contain a character that the word has,
//the word is determined to be invalid.
int char_list_contains_word(node_t* charsToContain, char* word,
		int currentWordLength) {
	
	int valid = 1;
	node_t* characterRegex = linkedlist_clone(charsToContain);
	
	for (int i = 0; i < currentWordLength; i++) {

		char currentChar[2];
		currentChar[0] = word[i];
		currentChar[1] = '\0';
		
		int index = linkedlist_indexOf(currentChar, characterRegex, LINKEDLIST_CHAR);

		if ((index != -1) && (valid)) {
			
			if (index != 0) {
				linkedlist_removeAt(characterRegex, index);
			
			} else {
				linkedlist_set(characterRegex, "\0", index, LINKEDLIST_CHAR);
			}
		
		} else {
			valid = 0;
			break;
		}
	}
	
	linkedlist_delete(characterRegex);
	
	return valid;
}

//Search through from the node passed in wordsList until
//the number of words to check has been reached.
//make sure that each of the words are made up of the
//characters passed in charsToContain using
//char_list_contains_word()
//
//if a word is determined to be valid, lock the mutex, 
//and try to add it to the list. If enough words have
//already been found, this will fail and return 0.
//then the loop will exit, and the thread will stop.
void* word_searcher_thread(void* arg) {
	thread_data_t* working_data = arg;
	node_t** resultWords = working_data->resultWords;
	node_t* wordsList = working_data->wordsList;
	node_t* charsToContain = working_data->charsToContain;
	pthread_mutex_t* wordsMutex = working_data->wordsMutex;
	int* numberOfWords = working_data->numberOfWords;
	
	//loop through each of the words assigned in working_data
	//and check their validity considering the characters passed
	//that the words need to contain.
	//if it is valid add the word to the result word list.
	for (int i = 0; i < working_data->wordsToCheck; i++) {
		if (wordsList == NULL) {
			fprintf(stderr, "Error. word searcher thread passed NULL word list.\n");
			break;
		}

		char* currentWordToCheck = wordsList->val;
		int currentWordLength = strlen(currentWordToCheck);
		
		if ((currentWordLength <= linkedlist_size(charsToContain)) &&
				(currentWordLength >= 3)) {

			//check if the word is valid considering the current charsToContain
			//if valid, add to the result word list.
			if (char_list_contains_word(charsToContain, currentWordToCheck, currentWordLength)) {
				pthread_mutex_lock(wordsMutex);
				
				int success = update_word_list(resultWords, numberOfWords,
						currentWordToCheck);
				
				pthread_mutex_unlock(wordsMutex);
				
				if (!success) {
					break;
				}
			}
		}
			
		wordsList = wordsList->next;
	}
	return NULL;
}

//find words from the passed list made up only of the list
// of characters passed.
node_t* find_words_from_chars(node_t* characters, int numberOfWords,
		node_t* wordList) {
	
	//get the number of threads to generate based on the number of
	//online processors on the system.
	int numThreads = get_thread_number();

	//fail if can't find number of processors.
	if (numThreads < 0) {
		fprintf(stderr, "Unable to find the number of online processors.\n");
		return NULL;
	}

	pthread_mutex_t wordsMutex = PTHREAD_MUTEX_INITIALIZER;
	node_t* resultWords = NULL;

	int startValue = 0;
	int incrementValue = linkedlist_size(wordList) / numThreads;

	//threads holds each generated thread that needs to be joined back
	//threadData holds allocated work packages for each thread to work on.
	pthread_t threads[numThreads];
	thread_data_t* threadData[numThreads];

	for (int i = 0; i < numThreads; i++) {
		thread_data_t* currentData = malloc(sizeof(thread_data_t));
		threadData[i] = currentData;
		int threadCreationSuccess;

		if (currentData == NULL) {
			fprintf(stderr, "The system is out of memory!");
			return NULL;
		}

		//assign values to the currentData package that is destined
		//for a thread.
		currentData->wordsList = linkedlist_get(wordList, startValue);
		currentData->resultWords = &resultWords;
		currentData->charsToContain = characters;
		currentData->wordsToCheck = incrementValue - 1;
		currentData->numberOfWords = &numberOfWords;
		currentData->wordsMutex = &wordsMutex;

		//create the thread to handle the currentData
		if ((threadCreationSuccess = pthread_create(&threads[i], NULL,
				&word_searcher_thread, currentData))) {
			
			fprintf(stderr, "Thread Creation Failed: %d", threadCreationSuccess);
		}

		startValue = startValue + incrementValue;
	}

	//join back all the threads,
	//and free the the threadData objects allocated.
	for (int i = 0; i < numThreads; i++) {
		pthread_join(threads[i], NULL);
		free(threadData[i]);
	}

	return resultWords;
}
