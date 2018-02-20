#include <stdio.h>
#include <stdlib.h>
#include "maze_solver.h"

#define DEBUG

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("File name is required as first argument!\n");
		return 1;
	}
	else if (argc > 2)
	{
		printf("Too many arguments.\n");
		return 1;
	}

	FILE *image_file = fopen(argv[1], "r");

	if (image_file == NULL)
	{
		printf("Could not open the file!");
		return 1;
	}

	static const unsigned long width_offset = 18L,
				   header_size = 54L;
	unsigned width = 0, height = 0;
	unsigned char *image = NULL;

	// get the image width and height
	fseek(image_file, width_offset, SEEK_SET); // go to the offset of the image width
	fread(&width, 4L, 1, image_file);
	fread(&height, 4L, 1, image_file);

#ifdef DEBUG
	printf("Image dimensions (in pixels):\n");
	printf("width: %u\t height: %u\n", width, height);
#endif

	// find the image data size
	fseek(image_file, 0, SEEK_END);
	const long file_size = ftell(image_file);
	unsigned long data_size = file_size-header_size;

#ifdef DEBUG
	printf("Size of image data: %lu\n", data_size);
#endif

	// allocate memory to read in the image
	image = malloc(data_size);

	if (image == NULL)
	{
		fprintf(stderr, "Not enough memory to read in image!\n");
		exit(EXIT_FAILURE);
	}

	// skip past the header
	fseek(image_file, 54L, SEEK_SET);

	// read the image
	if (fread(image, data_size, 1, image_file) == 0)
	{
		fprintf(stderr, "Reading image failed!\n");
		exit(EXIT_FAILURE);
	}

	solve_maze(image, width, height);

	free(image);
}
