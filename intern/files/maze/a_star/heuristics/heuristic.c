#include "heuristic.h"
#include "../../common.h"
#include "../../maze_solver_helpers.h"
#include "../../../../my_math/math.h"

#ifdef KS_MAZE_SOLVER_DEBUG
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#endif

inline unsigned x(const unsigned pixel, const unsigned width)
{
	return pixel/width;
}

inline unsigned y(const unsigned pixel, const unsigned width)
{
	return pixel%width;
}

static inline
unsigned m_dist(const int x1, const int y1,
                const int x2, const int y2)
{

#ifdef KS_MAZE_SOLVER_DEBUG_MANHATTAN_DISTANCE
	printf("m_dist: x1: %d, y1: %d; x2: %d, y2: %d\n", x1, y1, x2, y2);
#endif

	return math_abs(x2-x1)+math_abs(y2-y1);
}

typedef unsigned (*heuristic_generator) (const int x1, const int x2, const int y1, const int y2);

void get_heuristic_helper(struct maze_image *maze, struct openings *gates,
                          unsigned *heuristic_values, heuristic_generator get_heuristic)
{
	const unsigned m_width = maze->width,
	               goal_x = x(gates->end_gate_pixel, m_width),
	               goal_y = y(gates->end_gate_pixel, m_width);

	// loop range is optimized based on the fact that there are no valid pixels
	// except the start and end gate in the boundary
	for (unsigned width = 1; width < m_width-1; width++)
	{
		for (unsigned height = 1; height < maze->height-1; height++)
		{
			const unsigned curr_pixel = (m_width*height) + width;
			if (is_clear_pixel(maze, curr_pixel))
			{

#ifdef KS_MAZE_SOLVER_DEBUG
				if (x(curr_pixel, m_width) > INT_MAX ||
				    y(curr_pixel, m_width) > INT_MAX ||
				    goal_x > INT_MAX ||
				    goal_y > INT_MAX)
				{
					fprintf(stderr, "get_heuristic_helper: integer overflow\n");
					fprintf(stderr, "get_heuristic_helper: co-ordinates don't fit into an 'int'\n");
					exit(EXIT_FAILURE);
				}
#endif

#ifdef KS_MAZE_SOLVER_DEBUG_MANHATTAN_DISTANCE
				printf("get_heuristic_helper: finding manhattan distance for pixel: %u\n", curr_pixel);
#endif

				*(heuristic_values + curr_pixel) = get_heuristic(x(curr_pixel, m_width),
				                                                 y(curr_pixel, m_width),
				                                                 goal_x,
				                                                 goal_y);
			}
		}
	}

}

void get_manhattan_heuristic(struct maze_image *maze, struct openings *gates,
                             unsigned *heuristic_values)
{
	get_heuristic_helper(maze, gates, heuristic_values, m_dist);
}
