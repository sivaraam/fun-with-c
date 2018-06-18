/**
 * Program to test what happens when a file is deleted
 * externally when it is opened in the program.
 *
 * In general this test tries to validate the fact that,
 *
 *   Applications continue to have access to their open
 *   files even if they are removed externally by some
 *   means.
 *
 * In GNU/Linux, this seems to be true.
 */

#include <stdio.h>
#include <unistd.h>

char read_ch(FILE *fp)
{
	char ch;
	if (fread (&ch, 1, 1, fp))
	{
		return ch;
	}

	return 0;
}

int main(void)
{
	char *read_file = "temp-file-read";
	FILE *read_fp = fopen (read_file, "r");
	if (read_fp != NULL)
	{
		printf ("First character: %c\n", read_ch(read_fp));

		/* remove the file and wait for some time */
		remove (read_file);
		sleep(120);

		fseek (read_fp, -2L, SEEK_END);
		printf ("Second character: %c\n", read_ch(read_fp));
	}
	else
	{
		printf ("File '%s' missing.\n", read_file);
	}
}
