#include <stdio.h>
#include <stdlib.h>
#include "maze_solver.h"
#include "bmp/bmp_helpers.h"

#define KS_MAZE_SOLVER_DEBUG

void solve_maze(struct maze_image *maze)
{
	// find the padding
	maze->padding = find_padding(maze->width);

#ifdef KS_MAZE_SOLVER_DEBUG
	printf("padding: %u bytes\n", maze->padding);
#endif

	// find the image data size
	const unsigned long image_data_size = (maze->height*maze->width*bytes_per_pixel) +
					      (maze->padding*maze->height); // a way to find data size without file size

#ifdef KS_MAZE_SOLVER_DEBUG
	printf("Size of image data: %lu\n", image_data_size);

	printf("ASCII art image of maze:\n");
	for (unsigned long pixel=0; pixel<maze->width*maze->height; pixel++)
	{
		if (pixel%maze->width == 0)
		{
			printf("\n");
		}
		
		unsigned char *pixel_ptr = get_pixel(maze->data, maze->width, maze->padding, pixel);
		
		if ((*pixel_ptr&0xFF) == 0xFF)
		{
			printf(" ");
		}
		else if ((*pixel_ptr&0x00) == 0x00)
		{
			printf("\u2588"); // unicode block character
		}
		else
		{
			fprintf(stderr, "Unexpected Hex!!\n");
			exit(EXIT_FAILURE);
		}
	}

	printf("\n");
#endif
}
