#ifndef WORDS_H
#define WORDS_H

//read all words from words.txt in the execution path
node_t* read_words(void);

//generate a set of random characters the length speficied
//containing at least one vowel.
node_t* generate_guess_characters(int number_of_chars);

//find words from the passed list made up only of the list
// of characters passed.
node_t* find_words_from_chars(node_t* characters, int number_of_words,
		node_t* word_list, int minimum_word_length, int maximum_word_length);

#endif
