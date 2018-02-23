#include <stdio.h>
#include <stdlib.h>
#include "bmp/bmp_helpers.h"
#include "common.h"
#include "maze_solver.h"
#include "maze_solver_helpers.h"

/**
 * Free the nodes (if any) in the shortest path queue when the shortest
 * path could not be successfully found.
 */
static void free_sp_queue(struct sp_queue_head *sp)
{
	while(!sp_queue_empty(sp))
	{
		free(sp_remove_elem(sp));
	}
}

int solve_maze(struct maze_image *const maze)
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

	struct openings *const o = find_openings(maze);

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
	if (initialize_adjacencies(maze, o))
	{
		return ERRMEMORY;
	}

	// find the shortest path to the end node from the source node
	// for the constructed graph
	struct sp_queue_head *const sp = malloc(sizeof(struct sp_queue_head));

	if (sp == NULL)
	{
		return ERRMEMORY;
	}

	unsigned dest_distance = find_shortest_path(o, sp);

	if (dest_distance != 0)
	{
#ifdef KS_MAZE_SOLVER_DEBUG_PRINT_SHORTEST_PATH
		// Warning: This removes the items from the queue!
		printf("Shortest path from %u to %u:\n", o->start_gate_pixel, o->end_gate_pixel);

		while (!sp_queue_empty(sp))
		{
			struct sp_queue_elem *const curr_elem = sp_remove_elem(sp);
			printf("%u\t", curr_elem->elem);
			free(curr_elem);
		}

		printf("\n");
#else
		colour_path(maze, sp);
#endif

	}
	else
	{
		free_sp_queue(sp);
		return ERRSHPATH;
	}

	// free the queue head
	free(sp);

	delete_graph();
	return 0;
}
