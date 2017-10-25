#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#define MAX_INPUT_SIZE (100+1)
//#define MAX_SEARCH_TERM_SIZE (15+1)
//#define MAX_REPLACE_TERM_SIZE (15+1)

/* Returns the position in which 'search_string' is
 * found in 'str'. Returns -1 if not found.
 */
ptrdiff_t find(char *str, size_t str_len, char *search_string, size_t search_len) {
	size_t str_pos = 0;

	while(str_pos < str_len) { // can optimize str_pos+search_len < str_len
		if(!strncmp(str+str_pos, search_string, search_len))
			return str_pos;
		str_pos++;
	}

	return -1;
}

int main(void) {
	char *input_str = malloc(MAX_INPUT_SIZE), *replace_str = malloc(MAX_INPUT_SIZE);
	char *search;// = malloc(MAX_SEARCH_TERM_SIZE);
	char *replace;// = malloc(MAX_REPLACE_TERM_SIZE);
	size_t input_len = 0, search_len = 0, replace_len  = 0;
	size_t to_copy_start = 0, replace_str_marker=0;


	// get the inputs
	printf("Enter the string: ");
	fgets(input_str, MAX_INPUT_SIZE, stdin);

	printf("Enter search string: ");
	scanf("%ms", &search);

	printf("Enter replace string: ");
	scanf("%ms", &replace);	
//	fgets(replace, MAX_REPLACE_TERM_SIZE, stdin);

//	printf("%s %zu %s", input_str, strlen(search), replace);

	// get the sizes
	input_len = strlen(input_str);
	search_len = strlen(search);
	replace_len = strlen(replace);

	// start searching and replacing!
	while (1) {

		// Get the offset at which search term is present in unanalyzed portion of input.
		// i.e., offset of search term in string starting at 'input_str+to_copy_start'.
		ptrdiff_t offset = find(input_str+to_copy_start, input_len-to_copy_start, search, search_len);

		if (offset != -1) {

			// copy the string between the last copied position and currently found position
			strncpy(replace_str+replace_str_marker, input_str+to_copy_start, offset);
			replace_str_marker += offset;

			// copy the found instance with the replace word
			strncpy(replace_str+replace_str_marker, replace, replace_len);
			replace_str_marker += replace_len;
			to_copy_start += offset+search_len;

		} else {
			strncpy(replace_str+replace_str_marker, input_str+to_copy_start, input_len-to_copy_start);
			break;
		}

	}

	printf("Replaced string: %s", replace_str);
}
