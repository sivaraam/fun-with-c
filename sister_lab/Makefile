.PHONY: clean
CC = gcc
CFLAGS = -std=c99 -g -Wall -Wextra -pedantic

BINARIES = find_and_replace replace_end words_test
clean:
	rm -f $(BINARIES)
	rm -rf *.o a.out*

find_and_replace.o: string/find_and_replace.c
	$(CC) $(CFLAGS) -c $^

string_api.o: string/string_api.c
	$(CC) $(CFLAGS) -c $^

word_pluraliser.o: word_pluraliser.c
	$(CC) $(CFLAGS) -c $^

find_and_replace: find_and_replace.o find_and_replace_test.c
	$(CC) $(CFLAGS) $^ -o $@

replace_end: find_and_replace.o replace_end_test.c
	$(CC) $(CFLAGS) $^ -o $@

words_test: find_and_replace.o string_api.o word_pluraliser.o words_test.c
	$(CC) $(CFLAGS) $^ -o $@
