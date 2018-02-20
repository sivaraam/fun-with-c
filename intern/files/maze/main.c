#include <stdio.h>
#include <stdlib.h>
#include "maze_solver.h"

#define DEBUG

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		fprintf(stderr, "File name is required as first argument!\n");
		return 1;
	}
	else if (argc > 2)
	{
		fprintf(stderr, "Too many arguments.\n");
		return 1;
	}

	FILE *image_file = fopen(argv[1], "r");

	if (image_file == NULL)
	{
		fprintf(stderr, "Could not open the file!\n");
		return 1;
	}

	static const unsigned long width_offset = 18L,
				   header_size = 54L;

	// create the maze object
	struct maze_image *maze = malloc(sizeof(struct maze_image));

	if (maze == NULL)
	{
		fprintf(stderr, "Not enough memory to create maze object!\n");
		return 1;
	}

	// get the image width and height
	fseek(image_file, width_offset, SEEK_SET); // go to the offset of the image width
	fread(&(maze->width), 4L, 1, image_file);
	fread(&(maze->height), 4L, 1, image_file);

#ifdef DEBUG
	printf("Image dimensions (in pixels):\n");
	printf("width: %u\t height: %u\n", maze->width, maze->height);
#endif

	// find the image data size
	fseek(image_file, 0, SEEK_END);
	const long file_size = ftell(image_file);
	unsigned long data_size = file_size-header_size;

#ifdef DEBUG
	printf("Size of image data: %lu\n", data_size);
#endif

	// allocate memory to read in the image
	maze->data = malloc(data_size);

	if (maze->data == NULL)
	{
		fprintf(stderr, "Not enough memory to read in image!\n");
		return 1;
	}

	// skip past the header
	fseek(image_file, 54L, SEEK_SET);

	// read the image
	if (fread(maze->data, data_size, 1, image_file) == 0)
	{
		fprintf(stderr, "Reading image failed!\n");
		return 1;
	}

	solve_maze(maze);

	// free the memory
	free(maze->data);
	free(maze);
}
