#include <stdio.h>
#include <stdlib.h>
#include "maze_solver.h"
#include "bmp/bmp_helpers.h"

#define DEBUG
// #define DEBUG_EXPAND_DATA

#define ERRWRITE 128

/**
 * Keeps only the first byte of every pixel and throws away the remaining bytes.
 * This shrinks the image size and simplifies the access to the pixel data.
 */
void shrink_image(struct maze_image *maze)
{
	unsigned padding_adjustment = 0;
	for (unsigned curr_row = 0; curr_row<maze->height; curr_row++)
	{
		for (unsigned curr_col = 0; curr_col<maze->width; curr_col++)
		{
			const unsigned curr_pixel = (curr_row*maze->width + curr_col);
			*(maze->data + curr_pixel) = *(maze->data + bytes_per_pixel*curr_pixel + padding_adjustment);
		}
		padding_adjustment += maze->padding;
	}
}

void expand_data(struct maze_image *maze)
{
	unsigned expanded_offset = maze->pixels*bytes_per_pixel + maze->padding*maze->height - 1;

	unsigned curr_row = maze->height;
	do
	{
		curr_row--;

		const unsigned char padding_byte = 0x00;

		// fill in the padding
		for (unsigned curr_padding_byte = 0; curr_padding_byte<maze->padding; curr_padding_byte++)
		{

#ifdef DEBUG
			if (expanded_offset > maze->pixels*bytes_per_pixel + maze->padding*maze->height)
			{
				fprintf(stderr, "expand_data: Assumption that 'expanded_offset' always has the correct offset failed!\n");
				exit(EXIT_FAILURE);
			}
#endif

			*(maze->data + expanded_offset--) = padding_byte;
		}

		unsigned curr_col = maze->width;
		do
		{
			curr_col--;

			const unsigned shrunk_pixel_offset = (curr_row*maze->width + curr_col);

#ifdef DEBUG_EXPAND_DATA
			printf("expand_data: expanding data for pixel: %u; expanded_offset: %u;\n",
			       shrunk_pixel_offset, expanded_offset);
#endif

			// expand the pixel bytes
			for (unsigned pixel_byte = 0; pixel_byte<bytes_per_pixel; pixel_byte++)
			{

#ifdef DEBUG
				if (expanded_offset > maze->pixels*bytes_per_pixel + maze->padding*maze->height)
				{
					fprintf(stderr, "expand_data: Assumption that 'expanded_offset' always has the correct offset failed!\n");
					exit(EXIT_FAILURE);
				}
#endif

				*(maze->data + expanded_offset--) = *(maze->data + shrunk_pixel_offset);
			}

		} while (curr_col != 0);

	} while (curr_row != 0);
}

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
	if (fseek(image_file, width_offset, SEEK_SET)) // go to the offset of the image width
	{
		fprintf(stderr, "File format not as expected!\n");
		ret_val = 1;
		goto FREE_NODATA_QUIT;
	}

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

	// find the padding
	maze->padding = find_padding(maze->width);

#ifdef DEBUG
	printf("Image dimensions (in pixels):\n");
	printf("width: %u\t height: %u\n", maze->width, maze->height);
#endif

	// find the image data size
	fseek(image_file, 0, SEEK_END);
	const long file_size = ftell(image_file);
	long data_size = file_size-header_size;

	if (data_size < 0)
	{
		fprintf(stderr, "File format not as expected!\n");
		ret_val = 1;
		goto FREE_NODATA_QUIT;
	}

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
	if (fseek(image_file, 54L, SEEK_SET))
	{
		fprintf(stderr, "File format not as expected!\n");
		ret_val = 1;
		goto FREE_NODATA_QUIT;
	}

	// read the image
	if (fread(maze->data, data_size, 1, image_file) == 0)
	{
		fprintf(stderr, "Reading image failed!\n");
		ret_val = 1;
		goto FREE_QUIT;
	}

	// throw away the unnecessary parts of the image
	shrink_image(maze);

	// reduce the image size after shrinking
	unsigned char *shrunk_data = realloc(maze->data, maze->pixels*sizeof(unsigned char));
	if (shrunk_data == NULL)
	{
		// Fail in case we couldn't shrink the memro (!?) to avoid unnecessarry overhead of function
		// calls to identify the offset of a pixel in the data.
		//
		// If that overhead is not an issue, we could pass a function pointer to 'solve_maze' indicating
		// how to find the pixel offset and thus handling this failure graciously.
		fprintf(stderr, "Shrinking image data failed!\n");
		ret_val = 1;
		goto FREE_QUIT;
	}
	else
	{
		maze->data = shrunk_data;
		shrunk_data = NULL;
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
	// This should succeed if the previous one did!
	fseek(image_file, 54L, SEEK_SET);

	// expand the memory to expand the data to BMP format
	unsigned char *expanded_data = realloc(maze->data, data_size);
	if (expanded_data == NULL)
	{
		// we have to fail because we are allowed to use fwrite only once
		// So, the fallback technique of directly expanding in file is not possible.
		fprintf(stderr, "Expanding image failed!\n");
		ret_val = 1;
		goto FREE_QUIT;
	}
	else
	{
		maze->data = expanded_data;
		expanded_data = NULL;
	}

	expand_data(maze);

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
