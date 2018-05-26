#include "linkedlist.h"
#include "sodium.h"

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

node_t* generate_Guess_Characters(int number_Of_Chars) {

	if (sodium_init() < 0) {
		return NULL;
	}

	node_t* guessable_Characters = NULL;
	
	char possibleChars[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g',
		'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
		's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

	char mustContainOne[] = {'a', 'e', 'i', 'o', 'u'};

	for (int i = 0; i < number_Of_Chars; i++) {
		char* charToAdd;

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
				
				if (linkedlist_indexOf(searchChar, guessable_Characters, CHAR) != -1) {
					oneExists = 1;
					break;
				}
			}

			if (!oneExists) {
				int randVowelIndex = randombytes_uniform(5);
				charToAdd = malloc(2);
				
				if (charToAdd == NULL) {
					return NULL;
				}
				
				charToAdd[0] = mustContainOne[randVowelIndex];
				charToAdd[1] = '\0';
				
				if (guessable_Characters == NULL) {
					return NULL;
				}
				linkedlist_add(guessable_Characters, charToAdd, CHAR);
				break;
			}
		}

		uint32_t randInt = randombytes_uniform(26);
		charToAdd = malloc(2);

		if (charToAdd == NULL) {
			return NULL;
		}
		
		charToAdd[0] = possibleChars[randInt];
		charToAdd[1] = '\0';

		if (guessable_Characters == NULL) {
			guessable_Characters = linkedlist_create(charToAdd, CHAR);
		} else {
			linkedlist_add(guessable_Characters, charToAdd, CHAR);
		}
	}
	return guessable_Characters;
}
