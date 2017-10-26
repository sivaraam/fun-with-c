#include <stdio.h>
#include <stdlib.h>
#include "word_pluraliser.h"

#define MAX_INPUT_SIZE 100

int main(void) {
	char *input = malloc(MAX_INPUT_SIZE);
	char *pluralised_words = malloc(MAX_INPUT_SIZE);

	printf("Enter the words: ");
	fgets(input, MAX_INPUT_SIZE, stdin);

	pluralise_words(input, pluralised_words);
	printf("Pluralised words: %s\n", pluralised_words);
}
