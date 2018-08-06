
#include <stdio.h>
#include <string.h>
#include "WC_linkedlist.h"
#include "hints.h"
#include "sodium.h"

//create a new hint type, initialize the default values
//return the new created type
hint_data_t* create_hint_type(void) {
    hint_data_t* type = malloc(sizeof(hint_data_t));
    
    if (type == NULL) {
        fprintf(stderr, "Error. System out of memory while allocating new hint_data.\n");
        return NULL;
    }

    //set default values to hint_type.
    type->min_chars_unrevealed = 0;
    type->current_hint_index = -1;
    type->current_guess_word = NULL;
    type->chars_revealed = 0;
    type->result = NULL;
    
    return type;
}
//free the 2 dynamically allocated variables in the hint type,
//then free the hint data type itself.
void delete_hint_type(hint_data_t* type) {
    if (type != NULL && type->current_guess_word != NULL
        && type->result != NULL) {
        
        free(type->result);
        free(type->current_guess_word);
        free(type);
    }
} 

//returns null on failure
//retrieve a specific char* from a node_t in a linked list type.
char* get_char_from_node(node_t* char_to_test) {
    //null checks
    if (char_to_test != NULL) {
        char* internal_char = char_to_test->val;
            
        if (internal_char != NULL) {
            return internal_char;
        }
    }
    return NULL;
}

//allocate or reallocate result to store the new hint string
//current_hint_word_length must not be NULL.
//will return NULL on failure.
char* allocate_result(char* result, int* current_hint_word_length) {   
    char error_string[] = "Error. System out of memory trying to allocate hint result.\n";
    int size_of_result = sizeof(char) * (*current_hint_word_length) + 1;
    
    //if result is already allocated, reallocate to the right size. otherwise allocate.
    if (result == NULL) {
        result = malloc(size_of_result);
        
        if (result == NULL) {
            fprintf(stderr, "%s", error_string);
            return NULL;
        }
        
    } else {
        result = realloc(result, size_of_result);
        
        if (result == NULL) {
            fprintf(stderr, "%s", error_string);
            return NULL;
        }
    }
    return result;
}

//build the hint string
//none of the arguments can be null.
void get_hint_string(char* hint_string, char* current_guess_word,
                     int* min_chars_unrevealed, int* chars_revealed,
                     int* current_hint_word_length) {

    //reveal one more char of the word than chars_revealed, unless that goes over the
    //minimum amount of characters allowed to be revealed. then null terminate.
    int i = 0;
    for(; i < *current_hint_word_length; i++) {
        
        if (i <= *chars_revealed && (i < *current_hint_word_length - *min_chars_unrevealed)) {
            hint_string[i] = current_guess_word[i];
        
        } else {
            hint_string[i] = '*';
        }
    }

    //null terminate the hint string.
    hint_string[i] = '\0';

    //set chars_revealed to record that we have revealed another character of the word
    //to the user.
    *chars_revealed += 1;
}

//validate parameters passed to get_hint()
int validate_hint_data(hint_data_t* hint_data, node_t* guessable_words) {
    if (hint_data == NULL || guessable_words == NULL) {
        fprintf(stderr, "Error. Either null hint data or guessable word list "
                        "was passed to get_hint\n");
        return 1;
    }
    return 0;
}

//initialize the current guess word if uninitialized.
char* initialize_current_guess_word(void) {
        char* current_guess_word = malloc(sizeof(char));
        
        if (current_guess_word == NULL) {
            fprintf(stderr, "Error. System out of memory initializing current_guess_word "
                    "in hint_data.\n");
            return NULL;
        }
        current_guess_word[0] = '\0';
        return current_guess_word;
}

//retrieve word at index making sure it isn't out of bounds.
char* get_word_equality_test(int* current_hint_index, int* max_value, node_t* guess_words) {
    //make sure retrieving the word at the current index is not out of bounds.
    if (*current_hint_index < (*max_value - 1)) {
        
        //retrieve word at current_hint_index
        char* internal_check = get_char_from_node(linkedlist_get(guess_words, *current_hint_index));
        return internal_check;
    }
    return NULL;
}

//result is stored in hint_data->result
void get_hint(hint_data_t* hint_data, node_t* guessable_words) {
    if (validate_hint_data(hint_data, guessable_words)) {
        return;
    }

    //used to make sure the current index is not longer than the guessable
    //words list.
    int max_value = linkedlist_size(guessable_words);
    
    //the current index of the word that we are revealing hints for.
    int current_hint_index = hint_data->current_hint_index;

    //the number of characters we have revealed
    int chars_revealed = hint_data->chars_revealed;

    //the current word we are revealing
    char* current_guess_word = hint_data->current_guess_word;

    if (current_guess_word == NULL) {
        hint_data->current_guess_word = current_guess_word = initialize_current_guess_word();
        
        if (current_guess_word == NULL) {
            return;
        }
    }

    char* word_equality_test = get_word_equality_test(&current_hint_index, &max_value, guessable_words);

    //if the hint_data is new, or the word is missing from the list,
    //or a word has been found pick a new word to reveal characters from
    if (current_hint_index < 0 || word_equality_test == NULL || 
        strcmp(word_equality_test, current_guess_word) != 0) {
        
        hint_data->current_hint_index = current_hint_index = randombytes_uniform(max_value);
        hint_data->chars_revealed = chars_revealed = 0;

        char* new_word = get_char_from_node(linkedlist_get(guessable_words, current_hint_index));
        
        if (new_word == NULL) {
            fprintf(stderr, "Error. new_word after finding a new word to reveal hints for is NULL.\n");
            return;
        }

        //reallocate current_guess_word to the right size, copy in the new guess word.   
        current_guess_word = realloc(current_guess_word, (sizeof(char) * (strlen(new_word) + 1)));
        if (current_guess_word == NULL) {
            fprintf(stderr, "Error. system out of memory reallocating current_guess_word.\n");
        }
        
        strcpy(current_guess_word, new_word);
        
        //set hint_data guess word to new guess word.
        hint_data->current_guess_word = current_guess_word;
        
    }

    //find length of current word a hint is being made for,
    //and create a character array to hold the new hint string
    int current_hint_word_length = strlen(current_guess_word);
    char hint_string[current_hint_word_length + 1];

    //create new obfuscated hint for the user to re-attempt a guess at the word.
    get_hint_string(hint_string, current_guess_word, &hint_data->min_chars_unrevealed,
                    &hint_data->chars_revealed, &current_hint_word_length);

    //allocate memory for the new obfuscated hint string.
    char* result = allocate_result(hint_data->result, &current_hint_word_length);
    
    //copy the newly made hint string into the result,
    //and set the result in hint_data.
    strcpy(result, hint_string);
    hint_data->result = result;
}
