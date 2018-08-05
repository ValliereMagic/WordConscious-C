#ifndef HINTS_H
#define HINTS_H

typedef struct {
    int min_chars_unrevealed;
    int current_hint_index;
    char* current_guess_word;
    int chars_revealed;
    char* result;
} hint_data_t;

//create a new hint type
hint_data_t* create_hint_type(void);

//delete a hint type
void delete_hint_type(hint_data_t* type);

//update hint with guessable words list.
void get_hint(hint_data_t* hint_data, node_t* guessable_words);


#endif