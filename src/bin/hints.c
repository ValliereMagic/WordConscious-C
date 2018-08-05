
#include <stdio.h>
#include <string.h>
#include "linkedlist.h"
#include "hints.h"
#include <limits.h>
#include "sodium.h"

hint_data_t* create_hint_type(void) {
    hint_data_t* type = malloc(sizeof(hint_data_t));
    if (type == NULL) {
        fprintf(stderr, "Error. System out of memory while allocating new hint_data.\n");
        return NULL;
    }
    type->current_hint_index = -1;
    type->chars_revealed = 0;
    type->min_chars_unrevealed = 0;
    return type;
}

void delete_hint_type(hint_data_t* type) {
    if (type != NULL && type->current_guess_word != NULL
        && type->result != NULL) {
        
        free(type->result);
        free(type->current_guess_word);
        free(type);
    }
} 

//returns null on failure
char* get_char_from_node(node_t* char_to_test) {
    //null checks
        if (char_to_test != NULL) {
            char* internal_char = char_to_test->val;
            
            if (internal_char != NULL) {
                return internal_char;
            }
        }
}

//result is stored in hint_data->result
void get_hint(hint_data_t* hint_data, node_t* guessable_words) {
    if (hint_data == NULL || guessable_words == NULL) {
        fprintf(stderr, "Error. Either null hint data or guessable word list "
                        "was passed to get_hint\n");
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
        current_guess_word = hint_data->current_guess_word = malloc(sizeof(char));
        
        if (current_guess_word == NULL) {
            fprintf(stderr, "Error. System out of memory initializing current_guess_word "
                    "in hint_data.\n");
            return;
        }
        current_guess_word[0] = '\0';
    }
    char* word_equality_test = NULL;
    
    //make sure retrieving the word at the current index is not out of bounds.
    if (current_hint_index < (max_value - 1)) {
        
        //retrieve word at current_hint_index
        char* internal_check = get_char_from_node(linkedlist_get(guessable_words, current_hint_index));
        word_equality_test = internal_check;
    }

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
        realloc(current_guess_word, (sizeof(char) * strlen(new_word) + 1));
        if (current_guess_word == NULL) {
            fprintf(stderr, "Error. system out of memory reallocating current_guess_word.\n");
        }
        strcpy(current_guess_word, new_word);
        
    }
    int current_hint_word_length = strlen(current_guess_word);
    char hint_string[current_hint_word_length + 1];
    int min_chars_unrevealed = hint_data->min_chars_unrevealed;
    
    //engages if min_chars_unrevealed is disabled.
    if (min_chars_unrevealed == 0) {
        min_chars_unrevealed = INT_MAX;
    }

    //reveal one more char of the word than chars_revealed, unless that goes over the
    //minimum amount of characters allowed to be revealed. then null terminate.
    int i = 0;
    for(; i < current_hint_word_length; i++) {
        
        if (i <= chars_revealed && i < min_chars_unrevealed) {
            hint_string[i] = current_guess_word[i];
        
        } else {
            hint_string[i] = '*';
        }
    }
    hint_string[i + 1] = '\0';

    char* result = hint_data->result;
    
    char error_string[] = "Error. System out of memory trying to allocate hint result.\n";
    int size_of_result = sizeof(char) * (current_hint_word_length) + 1;
    
    //if result is already allocated, reallocate to the right size. otherwise allocate.
    if (result == NULL) {
        result = malloc(size_of_result);
        if (result == NULL) {
            fprintf(stderr, error_string);
            return;
        }
    } else {
        realloc(result, size_of_result);
        if (result == NULL) {
            fprintf(stderr, error_string);
            return;
        }
    }

    strcpy(result, hint_string);
}
