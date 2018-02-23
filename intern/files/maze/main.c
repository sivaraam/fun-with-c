#include <stdio.h>
#include <stdlib.h>
#include "maze_solver.h"

#define DEBUG
#define ERRWRITE 128

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

	FILE *image_file = fopen(argv[1], "r+");
	unsigned char ret_val = 0;

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
		ret_val = 1;
		goto QUIT;
	}

	// get the image width and height
	fseek(image_file, width_offset, SEEK_SET); // go to the offset of the image width
	if (
		fread(&(maze->width), 4L, 1, image_file) == 0 ||
		fread(&(maze->height), 4L, 1, image_file) == 0
	)
	{
		fprintf(stderr, "Reading image metadata failed!\n");
		ret_val = 1;
		goto FREE_NODATA_QUIT;
	}

	maze->pixels = maze->width*maze->height;

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
		ret_val = 1;
		goto FREE_NODATA_QUIT;
	}

	// skip past the header
	fseek(image_file, 54L, SEEK_SET);

	// read the image
	if (fread(maze->data, data_size, 1, image_file) == 0)
	{
		fprintf(stderr, "Reading image failed!\n");
		ret_val = 1;
		goto FREE_QUIT;
	}

	ret_val = solve_maze(maze);
	if (ret_val == ERRMEMORY)
	{
		fprintf(stderr, "Could not solve maze due to insufficient memory!\n");
		goto FREE_QUIT;
	}
	else if (ret_val == ERROPENINGS)
	{
		fprintf(stderr, "Could not find openings (start and end pixels) for the given image!\n");
		goto FREE_QUIT;
	}
	else if (ret_val == ERRSHPATH)
	{
		fprintf(stderr, "Could not find shortest path from source to end in the given image!\n");
		goto FREE_QUIT;
	}

	// seek to the start of image data
	fseek(image_file, 54L, SEEK_SET);

	// write the solution
	if (fwrite(maze->data, data_size, 1, image_file) == 0)
	{
		fprintf(stderr, "Could not write the solved maze successfully to the file!\n");
		ret_val = ERRWRITE;
		goto FREE_QUIT;
	}

	printf("Successfully found the shortest path between from the source to the destination.\n");
	printf("The solution has been written to the file.\n");

FREE_QUIT:
	// free the memory
	free(maze->data);

FREE_NODATA_QUIT:
	free(maze);

QUIT:
	// close the file
	fclose(image_file);
	return ret_val;
}
