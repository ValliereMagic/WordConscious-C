#ifndef WORDS_H
#define WORDS_H

//read all words from words.txt in the execution path
node_t* read_Words(void);

//generate a set of random characters the length speficied
//containing at least one vowel.
node_t* generate_Guess_Characters(int number_Of_Chars);

//find words from the passed list made up only of the list
// of characters passed.
node_t* find_words_from_chars(node_t* characters, int numberOfWords,
		node_t* wordList);

#endif
