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
	printf("solve_maze: padding: %u bytes\n", maze->padding);

	// find the image data size
	const unsigned long image_data_size = (maze->height*maze->width*bytes_per_pixel) +
					      (maze->padding*maze->height); // a way to find data size without file size

	printf("solve_maze: Size of image data: %lu\n", image_data_size);
#endif

	struct openings *o = find_openings(maze);

	if (o == NULL)
	{
		fprintf(stderr, "solve_maze: Could not find openings for the given image!\n");
		return ERROPENINGS;
	}

#ifdef KS_MAZE_SOLVER_DEBUG
	printf("solve_maze: Start gate pixel: %u\t End gate pixel: %u\n",
		o->start_gate_pixel, o->end_gate_pixel);
#endif

	if (create_graph(maze))
	{
		return ERRMEMORY;
	}

#ifdef KS_MAZE_SOLVER_DEBUG
	printf("solve_maze: Found %u clear pixels\n", maze->clear_pixels);
#endif

	// initialize the adjacency for each node in the graph
	initialize_adjacencies(maze, o);

	// find the shortest path to the end node from the source node
	// for the constructed graph
	struct sp_queue_head *sp = find_shortest_path(o);

#ifdef KS_MAZE_SOLVER_DEBUG_PRINT_SHORTEST_PATH
	// Warning: This removes the items from the queue!
	printf("Shortest path from %u to %u:\n", o->start_gate_pixel, o->end_gate_pixel);

	while (!sp_queue_empty(sp))
	{
		struct sp_queue_elem *curr_elem = sp_remove_elem(sp);
		printf("%u\t", curr_elem->elem);
		free(curr_elem);
	}

	printf("\n");
#else
	colour_path(maze, sp);
#endif

	delete_graph();
	return 0;
}
