/**
 * A simple program to test if buffering is involved in
 * file I/O.
 *
 * This program writes some data to a file and sleeps for
 * 10s. If the data is successfully written to the file
 * even when the program is terminated while sleeping,
 * buffering *might* not have been involved. Else,
 * buffering is involved.
 */
#include <stdio.h>
#include <unistd.h>

int main(void)
{
	FILE *fp = fopen ("test-file-bufferring", "w");
	static const char *const data = "Hello";
	fwrite(data, 5, 1, fp);

	sleep(10);

	fclose(fp);
}
