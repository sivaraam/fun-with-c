#include <stdio.h>
#include <stdlib.h>
#include "bmp/bmp_helpers.h"
#include "common.h"
#include "maze_solver.h"
#include "maze_solver_helpers.h"

int solve_maze(struct maze_image *maze)
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
	print_ascii_maze(maze);
#endif

	struct openings *o = find_openings(maze);

	if (o == NULL)
	{
		fprintf(stderr, "Could not find openings for the given image!\n");
		return ERROPENINGS;
	}

#ifdef KS_MAZE_SOLVER_DEBUG
	printf("Start gate pixel: %u\t End gate pixel: %u\n",
		o->start_gate_pixel, o->end_gate_pixel);
#endif

	return 0;
}
