#include "heuristic.h"
#include "../../common.h"
#include "../../maze_solver_helpers.h"
#include "../../../../my_math/math.h"

#ifdef KS_MAZE_SOLVER_DEBUG
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#endif

/**
 * Given the image width and the amount of padding in it gives the byte offset
 * for Nth pixel. (Indexed from 0).
 *
 * width - width of the image
 * padding - the number of bytes of padding between the rows (cannot exeeed 3)
 * N - the pixel required
 *     assumed to be in the range of width*height of the image
 */
inline static
unsigned long get_pixel_offset(const unsigned long width, const unsigned char padding, const unsigned long N)
{
	return N*bytes_per_pixel + ((N/width)*padding);
}


/**
 * Returns non-zero value if the given pixel in the maze is a clear pixel.
 * Else returns 0.
 */
inline static
int is_clear_pixel(struct maze_image *const maze, unsigned pixel)
{

#ifdef KS_MAZE_SOLVER_DEBUG
	if (pixel >= maze->pixels)
	{
		fprintf(stderr, "is_clear_pixel: Invalid pixel %u\n", pixel);
		exit(EXIT_FAILURE);
	}
#endif

	const unsigned char *const pixel_byte = maze->data + get_pixel_offset(maze->width, maze->padding, pixel);

	// It's enough to check one byte for now
	return ((*pixel_byte&CLEAR_PIXEL) == CLEAR_PIXEL) ? 1 : 0;
}

inline unsigned x(const unsigned pixel, const unsigned width)
{
	return pixel/width;
}

inline unsigned y(const unsigned pixel, const unsigned width)
{
	return pixel%width;
}

inline unsigned m_dist(const int x1, const int y1,
                       const int x2, const int y2)
{

#ifdef KS_MAZE_SOLVER_DEBUG_MANHATTAN_DISTANCE
	printf("m_dist: x1: %d, y1: %d; x2: %d, y2: %d\n", x1, y1, x2, y2);
#endif

	return math_abs(x2-x1)+math_abs(y2-y1);
}

void get_manhattan_heuristic(struct maze_image *maze, struct openings *gates,
                             unsigned *heuristic_values)
{
	const unsigned m_width = maze->width,
	               goal_x = x(gates->end_gate_pixel, m_width),
	               goal_y = y(gates->end_gate_pixel, m_width);

	// loop range is optimized based on the fact that there are no valid pixels
	// except the start and end gate in the boundary
	for (unsigned width = 1; width < maze->width-1; width++)
	{
		for (unsigned height = 1; height < maze->height-1; height++)
		{
			const unsigned curr_pixel = (maze->width*height) + width;
			if (is_clear_pixel(maze, curr_pixel))
			{

#ifdef KS_MAZE_SOLVER_DEBUG
				if (x(curr_pixel, width) > INT_MAX ||
				    y(curr_pixel, width) > INT_MAX ||
				    goal_x > INT_MAX ||
				    goal_y > INT_MAX)
				{
					fprintf(stderr, "get_manhattan_heuristic: integer overflow\n");
					fprintf(stderr, "get_manhattan_heuristic: co-ordinates don't fit into an 'int'\n");
					exit(EXIT_FAILURE);
				}
#endif

#ifdef KS_MAZE_SOLVER_DEBUG_MANHATTAN_DISTANCE
				printf("get_manhattan_heuristic: finding manhattan distance for pixel: %u\n", curr_pixel);
#endif

				*(heuristic_values + curr_pixel) =
				    m_dist(x(curr_pixel, m_width), y(curr_pixel, m_width),
				           goal_x, goal_y);
			}
		}
	}
}
