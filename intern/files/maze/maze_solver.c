#include <stdio.h>
#include <stdlib.h>
#include "bmp/bmp_helpers.h"
#include "common.h"
#include "maze_solver.h"
#include "maze_solver_helpers.h"
#include "a_star/heuristics/heuristic.h"

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
	int ret_val = 0;

	// find the padding
	maze->padding = find_padding(maze->width);

#ifdef KS_MAZE_SOLVER_DEBUG
	printf("solve_maze: padding: %u bytes\n", maze->padding);

	// find the image data size
	const unsigned long image_data_size = (maze->height*maze->width*bytes_per_pixel) +
					      (maze->padding*maze->height); // a way to find data size without file size

	printf("solve_maze: Size of image data: %lu\n", image_data_size);
#endif

	struct openings *const gates = find_openings(maze);

	if (gates == NULL)
	{
		return ERROPENINGS;
	}

#ifdef KS_MAZE_SOLVER_DEBUG
	printf("solve_maze: Start gate pixel: %u\t End gate pixel: %u\n",
		gates->start_gate_pixel, gates->end_gate_pixel);
#endif

#ifdef KS_MAZE_SOLVER_DEBUG_PROGRESS
	printf("solve_maze: Progress: Graph creation for the maze ...\n");
#endif

	if (create_graph(maze))
	{
		ret_val = ERRMEMORY;
		goto CLEANUP_GRAPH;
	}

#ifdef KS_MAZE_SOLVER_DEBUG_PROGRESS
	printf("solve_maze: Progress: Graph generated successfully for the maze.\n");
#endif

#ifdef KS_MAZE_SOLVER_DEBUG_PROGRESS
	printf("solve_maze: Progress: Adjacency initialization for the graph ..\n");
#endif

	// initialize the adjacency for each node in the graph
	if (initialize_adjacencies(maze, gates))
	{
		ret_val = ERRMEMORY;
		goto CLEANUP_GRAPH;
	}

#ifdef KS_MAZE_SOLVER_DEBUG_PROGRESS
	printf("solve_maze: Progress: Adjacencies initialized successfully for the graph.\n");
#endif

	// find the shortest path to the end node from the source node
	// for the constructed graph
	struct sp_queue_head *const sp = malloc(sizeof(struct sp_queue_head));

	if (sp == NULL)
	{
		ret_val = ERRMEMORY;
		goto CLEANUP_GRAPH;
	}

	initialise_sp_queue(sp);

	// allocate memory for the heuristic vector
	double *const heuristic_vector = malloc(maze->pixels*sizeof(double));

	if (heuristic_vector == NULL)
	{
		ret_val = ERRMEMORY;
		goto CLEANUP_SP;
	}

#ifdef KS_MAZE_SOLVER_DEBUG_PROGRESS
	printf("solve_maze: Progress: Heuristic value generation for the pixels ..\n");
#endif

	get_manhattan_heuristic(maze, gates, heuristic_vector);

#ifdef KS_MAZE_SOLVER_DEBUG_PROGRESS
	printf("solve_maze: Progress: Heuristic value generation completed.\n");
#endif

#ifdef KS_MAZE_SOLVER_DEBUG_PROGRESS
	printf("solve_maze: Progress: Shortest path to destination using the graph ..\n");
#endif

	unsigned dest_distance = find_shortest_path(gates, sp, heuristic_vector);

	if (dest_distance != 0)
	{

#ifdef KS_MAZE_SOLVER_DEBUG_PROGRESS
	printf("solve_maze: Progress: Shortest path generated successfully.\n");
#endif

#ifdef KS_MAZE_SOLVER_DEBUG_PRINT_SHORTEST_PATH
		// Warning: This removes the items from the queue!
		printf("Shortest path from %u to %u:\n", gates->start_gate_pixel, gates->end_gate_pixel);

		while (!sp_queue_empty(sp))
		{
			struct sp_queue_elem *const curr_elem = sp_remove_elem(sp);

			if (curr_elem == NULL)
			{
				fprintf(stderr, "solve_maze: Removing element from queue failed!\n");
				exit(EXIT_FAILURE);
			}

			printf("%u\t", curr_elem->elem);
			fflush(stdout);
			free(curr_elem);
		}

		printf("\n");
#else

#ifdef KS_MAZE_SOLVER_DEBUG_PROGRESS
	printf("solve_maze: Progress: Colour the shortest path ..\n");
#endif

		colour_path(maze, sp);

#ifdef KS_MAZE_SOLVER_DEBUG_PROGRESS
	printf("solve_maze: Progress: Colouring of shortest path completed.\n");
#endif

#endif

	}
	else
	{
		free_sp_queue(sp);
		ret_val = ERRSHPATH;
		goto CLEANUP;
	}

CLEANUP:
	free(heuristic_vector);

CLEANUP_SP:
	// free the queue head
	free(sp);

CLEANUP_GRAPH:
	delete_graph(maze);
	free(gates);
	return ret_val;
}
