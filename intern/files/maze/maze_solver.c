#include <stdio.h>
#include <stdlib.h>
#include "bmp/bmp_helpers.h"

#define KS_MAZE_SOLVER_DEBUG

void solve_maze(unsigned char *maze_image, unsigned width, unsigned height)
{
	// find the padding
	unsigned long padding = find_padding(width);

#ifdef KS_MAZE_SOLVER_DEBUG
	printf("padding: %lu bytes\n", padding);
#endif

	// find the image data size
	unsigned long image_data_size = (height*width*bytes_per_pixel)+(padding*height); // a way to find data size without file size

#ifdef KS_MAZE_SOLVER_DEBUG
	printf("Size of image data: %lu\n", image_data_size);

	printf("ASCII art image of maze:\n");
	for (unsigned long pixel=0; pixel<width*height; pixel++)
	{
		if (pixel%width == 0)
		{
			printf("\n");
		}
		
		unsigned char *pixel_ptr = get_pixel(maze_image, width, padding, pixel);
		
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
