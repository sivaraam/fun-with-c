#include <stdlib.h>
#include "common.h"
#include "bmp/bmp_helpers.h"
#include "maze_solver.h"
#include "maze_solver_helpers.h"

#ifdef KS_MAZE_SOLVER_DEBUG
#include <stdio.h>
#include <stdbool.h>
#endif

/**
 * Finds a 'gate' in the given range. A gate is the only clear pixel in the given range.
 *
 * On success, returns the value of the pixel of the gate cast to a long.
 * If one is not found returns -1;
 */
static long find_gate(struct maze_image *maze, unsigned start_pixel, unsigned end_pixel)
{
	long gate = -1;

#ifdef KS_MAZE_SOLVER_DEBUG
	bool found_gate = false;
#endif

	// find the pixel of the start gate
	for (unsigned pixel = start_pixel; pixel<=end_pixel; pixel++)
	{
		unsigned char *pixel_ptr = maze->data+get_pixel_offset(maze->width, maze->padding, pixel);

		if ((*pixel_ptr&CLEAR_PIXEL) == CLEAR_PIXEL)
		{

#ifdef KS_MAZE_SOLVER_DEBUG
			if (found_gate)
			{
				fprintf(stderr, "Two start gates found!\n");
				exit(EXIT_FAILURE);
			}
#endif

			gate = pixel;

#ifndef KS_MAZE_SOLVER_DEBUG
			break;
#endif

#ifdef KS_MAZE_SOLVER_DEBUG
			found_gate = true;
#endif

		}

	}

	return gate;
}
struct openings *find_openings(struct maze_image *maze)
{
	struct openings *o = malloc(sizeof(struct openings));

	if (o == NULL)
	{
		return NULL;
	}

	long start_gate_pixel = find_gate(maze, 0, maze->width-1),
	     end_gate_pixel = find_gate(maze, (maze->width-1)*maze->height, maze->width*maze->height-1);

	if (start_gate_pixel == -1 || end_gate_pixel == -1)
	{
		return NULL;
	}

	o->start_gate_pixel = start_gate_pixel;
	o->end_gate_pixel = end_gate_pixel;
	return o;
}

#ifdef KS_MAZE_SOLVER_DEBUG
void print_ascii_maze(struct maze_image *maze)
{
	for (unsigned long pixel=0; pixel<maze->width*maze->height; pixel++)
	{
		if (pixel%maze->width == 0)
		{
			printf("\n");
		}
		
		unsigned char *pixel_ptr = maze->data+get_pixel_offset(maze->width, maze->padding, pixel);
		
		if ((*pixel_ptr&CLEAR_PIXEL) == CLEAR_PIXEL)
		{
			printf(" ");
		}
		else if ((*pixel_ptr&HURDLE_PIXEL) == HURDLE_PIXEL)
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
}
#endif
