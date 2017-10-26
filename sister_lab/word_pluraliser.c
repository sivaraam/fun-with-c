#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "string/string_api.h"
#include "word_pluraliser.h"

#define MAX_WORD_SIZE (15+1)
#define MAX_REPLACEMENT_SIZE (3+1)

static void find_search_and_replacement(char *word, char *search, char *replacement) {
	size_t len = strlen(word);

	if (len<2)
		return;

	char last = *(word+len-1), penultimate = *(word+len-2);

	// replace 'y' by 'ies'
	if (last == 'y') {
		strncpy(search, "y", 1+1);
		strncpy(replacement, "ies", 3+1);
		return;
	}

	// append 'es' to nouns ending with 's', 'ch', 'sh'
	if (last == 's') {
		strncpy(search, "s", 1+1);
		strncpy(replacement, "ses", 3+1);
		return;
	} else if (penultimate == 's' && last == 'h') {
			strncpy(search, "sh", 2+1);
			strncpy(replacement, "shes", 4+1);
			return;
		} else if (penultimate == 'c' && last == 'h') {
				strncpy(search, "ch", 2+1);
				strncpy(replacement, "ches", 4+1);
				return;
			}

	// append 's' to all other nouns
	// uses append_ch instead of strncat because it's hard
	// to make a (null terminated) string out of a character.
	append_ch(search, last);
	append_ch(replacement, last);
	append_ch(replacement, 's');
}


/*
 * Using this function to initialise the char pointers
 * causes SEGFAULT for some reason (??)
static void initialise(void *str, size_t size) {
	str = malloc(size);
//	str = memset(str, 0, size);
}
*/

void pluralise_words(char *words, char *pluralised_words) {
	size_t input_length = strlen(words);
	size_t next_word_offset = 0, pluralised_marker = 0;

	while (next_word_offset<input_length) {
		char *word = malloc(MAX_WORD_SIZE);
		char *modified_word = malloc(MAX_WORD_SIZE);
		char *search = malloc(MAX_REPLACEMENT_SIZE);
		char *replacement = malloc(MAX_REPLACEMENT_SIZE);

		// It's good to initialise but have to find a better way
		// reduce this level of verbosity
		memset(word, 0, MAX_WORD_SIZE);
		memset(modified_word, 0, MAX_WORD_SIZE);
		memset(search, 0, MAX_REPLACEMENT_SIZE);
		memset(replacement, 0, MAX_REPLACEMENT_SIZE);

		// Results in SIGSEGV (see above)
		/* initialise(word, MAX_WORD_SIZE);
		initialise(modified_word, MAX_WORD_SIZE);
		initialise(search, MAX_REPLACEMENT_SIZE);
		initialise(replacement, MAX_REPLACEMENT_SIZE);*/

		sscanf(words+next_word_offset, "%s", word);
		next_word_offset += strlen(word) + 1;

		find_search_and_replacement(word, search, replacement);

		replace_end(word, modified_word, search, replacement);
		append_ch(modified_word, ' ');

		strncpy(pluralised_words+pluralised_marker, modified_word, strlen(modified_word));
		pluralised_marker += strlen(modified_word);

		free(word); free(modified_word);
		free(search); free(replacement);
	}
}
