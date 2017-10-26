#ifndef FIND_REPLACE
#define FIND_REPLACE

// Replace all occurences of 'search' in 'str' by 'replace' and return
// resulting string in 'result'
extern void replace_all(char *input, char *result, char *search, char *replace);


// Replace occurence of 'search' that appears at the end of 'str'
// by 'replace' and return resulting string in 'result'
extern void replace_end(char *input, char *result, char *search, char *replace);

#endif
