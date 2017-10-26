#include <stddef.h>
#include <string.h>

// find the occurence of 'search_string' in 'str' and return the offset at which
// it is present; else return -1
// So, return is value is either -1 (or) [0, str_len)
static ptrdiff_t find(char *str, size_t str_len, char *search_string, size_t search_len) {
	size_t str_pos = 0;

	while(str_pos < str_len) { // can optimize str_pos+search_len < str_len
		if(!strncmp(str+str_pos, search_string, search_len))
			return str_pos;
		str_pos++;
	}

	return -1;
}

void replace_all(char *input_str, char *result, char *search, char *replace) {
	size_t input_len = 0, search_len = 0, replace_len = 0;
	size_t to_copy_start = 0, result_marker = 0;

	// get the sizes
	input_len = strlen(input_str);
	search_len = strlen(search);
	replace_len = strlen(replace);

	// start searching and replacing!
	while (1) {

		// Get the offset at which search term is present in unanalyzed portion of input_str.
		// i.e., offset of search term in string starting at 'input_str+to_copy_start'.
		ptrdiff_t offset = find(input_str+to_copy_start, input_len-to_copy_start, search, search_len);

		if (offset != -1) {

			// copy the string between the last copied position and currently found position
			strncpy(result+result_marker, input_str+to_copy_start, offset);
			result_marker += offset;

			// copy the found instance with the replace word
			strncpy(result+result_marker, replace, replace_len);
			result_marker += replace_len;
			to_copy_start += offset+search_len;

		} else {
			strncpy(result+result_marker, input_str+to_copy_start, input_len-to_copy_start);
			break;
		}

	}
}

void replace_end(char *input_str, char *result, char *search, char *replace) {
	size_t input_len = 0, search_len = 0, replace_len = 0;
	size_t to_copy_start = 0, result_marker = 0;

	// get the sizes
	input_len = strlen(input_str);
	search_len = strlen(search);
	replace_len = strlen(replace);

	// start searching and replacing!
	while (1) {

		// Get the offset at which search term is present in unanalyzed portion of input_str.
		// i.e., offset of search term in string starting at 'input_str+to_copy_start'.
		ptrdiff_t offset = find(input_str+to_copy_start, input_len-to_copy_start, search, search_len);
		_Bool is_last = (input_len-to_copy_start-offset) == search_len;

		if (offset != -1) {

			// copy the string between the last copied position and currently found position
			strncpy(result+result_marker, input_str+to_copy_start, offset);
			result_marker += offset;

			if(is_last) {
				// copy the "last" found instance with the replace word
				strncpy(result+result_marker, replace, replace_len);
				result_marker += replace_len;
			} else {
				//copy the search term itself
				strncpy(result+result_marker, search, search_len);
				result_marker += search_len;
			}
			to_copy_start += offset+search_len;

		} else {
			strncpy(result+result_marker, input_str+to_copy_start, input_len-to_copy_start);
			break;
		}

	}
}
