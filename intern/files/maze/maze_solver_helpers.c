#include <stdlib.h>
#include <stdbool.h>
#include "common.h"
#include "bmp/bmp_helpers.h"
#include "graph/maze_graph.h"
#include "bfs_frontier/queue.h"
#include "maze_solver.h"
#include "maze_solver_helpers.h"
#include "maze_graph_bridge.h"

#ifdef KS_MAZE_SOLVER_DEBUG
#include <stdio.h>
#endif

#define MAX_NEIGHBOURS 4

/**
 * Returns non-zero value if the given pixel in the maze is a hurdle pixel.
 * Else returns 0.
 */
int is_hurdle_pixel(struct maze_image *maze, unsigned pixel)
{

#ifdef KS_MAZE_SOLVER_DEBUG
	if (pixel >= maze->width*maze->height)
	{
		fprintf(stderr, "is_hurdle_pixel: Invalid pixel %u\n", pixel);
		exit(EXIT_FAILURE);
	}
#endif

	unsigned char *pixel_byte = maze->data + get_pixel_offset(maze->width, maze->padding, pixel);

	if ((*pixel_byte&HURDLE_PIXEL) == HURDLE_PIXEL) // It's enough to check one byte for now
	{
		return 1;
	}

	return 0;
}

/**
 * Returns non-zero value if the given pixel in the maze is a clear pixel.
 * Else returns 0.
 */
int is_clear_pixel(struct maze_image *maze, unsigned pixel)
{

#ifdef KS_MAZE_SOLVER_DEBUG
	if (pixel >= maze->width*maze->height)
	{
		fprintf(stderr, "is_clear_pixel: Invalid pixel %u\n", pixel);
		exit(EXIT_FAILURE);
	}
#endif

	unsigned char *pixel_byte = maze->data + get_pixel_offset(maze->width, maze->padding, pixel);

	if ((*pixel_byte&CLEAR_PIXEL) == CLEAR_PIXEL) // It's enough to check one byte for now
	{
		return 1;
	}

	return 0;
}

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
	for (unsigned pixel=0; pixel<maze->width*maze->height; pixel++)
	{
		if (pixel%maze->width == 0)
		{
			printf("\n");
		}
		
		if (is_clear_pixel(maze, pixel))
		{
			printf(" ");
		}
		else if (is_hurdle_pixel(maze, pixel))
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

int create_graph(struct maze_image *maze)
{
	unsigned clear_pixels=0;

	if (maze == NULL)
	{
		return 1;
	}

	for (unsigned pixel=0; pixel<maze->width*maze->height; pixel++)
	{
		if (is_clear_pixel(maze, pixel))
		{
			// create the node
			struct node *n = create_node(pixel);

			if (n == NULL)
			{

#ifdef KS_MAZE_SOLVER_DEBUG_CREATE_GRAPH
				fprintf(stderr, "create_graph: 'create_node' failed for pixel: %u\n", pixel);
#endif

				return 1;
			}

			// insert the node into the 'np_list'
			if (insert_node(pixel, n))
			{

#ifdef KS_MAZE_SOLVER_DEBUG_CREATE_GRAPH
				fprintf(stderr, "create_graph: 'insert_node' failed for pixel: %u\n", pixel);
#endif

				return 1;
			}

#ifdef KS_MAZE_SOLVER_DEBUG_CREATE_GRAPH
			printf("create_graph: node value: %u\n", n->pixel);
			fflush(stdout);
#endif

			clear_pixels++;
		}
	}

	maze->clear_pixels = clear_pixels;
	return 0;
}

/**
 * The data structure used to hold the four possible neighbouring clear pixels
 * for a given pixel.
 *
 * The value is non-negative for a valid neighbour.
 */
struct pixel_neighbours
{
	long neighbour[4];

};

/**
 * Identifies valid clear pixel neighbours for the given "start gate" pixel and returns a valid
 * 'struct pixel_neighbours' object.
 */
struct pixel_neighbours find_start_gate_neighbours(struct maze_image *maze, unsigned sg_pixel)
{

#ifdef KS_MAZE_SOLVER_DEBUG
	long
#else
	unsigned
#endif
	p = sg_pixel;

#ifdef KS_MAZE_SOLVER_DEBUG
	long
#else
	unsigned
#endif
	bottom = p+maze->width;

	struct pixel_neighbours pn = { {-1L, -1L, -1L, -1L} };

#ifdef KS_MAZE_SOLVER_DEBUG
	if (
		p > maze->width*maze->height ||
		bottom > maze->width*maze->height
	)
	{
		fprintf(stderr, "find_start_gate_neighbours: Invalid pixel value\n");
		exit(EXIT_FAILURE);
	}
#endif

	if (is_clear_pixel(maze, bottom))
	{
		pn.neighbour[0] = bottom;
	}

	return pn;
}
/**
 * Identifies valid clear pixel neighbours for the given "end gate" pixel and returns a valid
 * 'struct pixel_neighbours' object.
 */
struct pixel_neighbours find_end_gate_neighbours(struct maze_image *maze, unsigned eg_pixel)
{

#ifdef KS_MAZE_SOLVER_DEBUG
	long
#else
	unsigned
#endif
	p = eg_pixel;

#ifdef KS_MAZE_SOLVER_DEBUG
	long
#else
	unsigned
#endif
	top = p - maze->width;

	struct pixel_neighbours pn = { {-1L, -1L, -1L, -1L} };

#ifdef KS_MAZE_SOLVER_DEBUG
	if (
		p > maze->width*maze->height ||
		top < 0
	)
	{
		fprintf(stderr, "find_end_gate_neighbours: Invalid pixel value\n");
		exit(EXIT_FAILURE);
	}
#endif

	if (is_clear_pixel(maze, top))
	{
		pn.neighbour[0] = top;
	}

	return pn;
}

/**
 * Identifiies neighbouring clear pixels for the given pixel and returns a valid
 * 'struct pixel_neighbours' object accordingly. Pixels are not expected to be
 * border pixels (those that have fewer than 4 valid neighbours).
 */
struct pixel_neighbours find_neighbours(struct maze_image *maze, unsigned pixel)
{

#ifdef KS_MAZE_SOLVER_DEBUG
	long
#else
	unsigned
#endif
	p = pixel;

#ifdef KS_MAZE_SOLVER_DEBUG
	long
#else
	unsigned
#endif
	top = p - maze->width, bottom = p + maze->width,
	left = p - 1, right = p + 1;

	struct pixel_neighbours pn = { {-1L, -1L, -1L, -1L} };


#ifdef KS_MAZE_SOLVER_DEBUG
	if (
		p > maze->width*maze->height ||
		left < 0 || right > maze->width*maze->height ||
		top < 0 || bottom > maze->width*maze->height
	)
	{
		fprintf(stderr, "find_end_gate_neighbours: Invalid pixel value\n");
		exit(EXIT_FAILURE);
	}
#endif

	if (is_clear_pixel(maze, left))
	{
		pn.neighbour[0] = left;
	}

	if (is_clear_pixel(maze, right))
	{
		pn.neighbour[1] = right;
	}

	if (is_clear_pixel(maze, top))
	{
		pn.neighbour[2] = top;
	}

	if (is_clear_pixel(maze, bottom))
	{
		pn.neighbour[3] = bottom;
	}

	return pn;
}

int initialize_adjacencies(struct maze_image *maze, struct openings *o)
{
	for (unsigned clear_pixel=0; clear_pixel<np_list_vals; clear_pixel++)
	{
		const unsigned curr_pixel = (*(np_list+clear_pixel))->pixel;

		struct pixel_neighbours n;

		// special case gates for simplicity
		if (
			curr_pixel != o->start_gate_pixel &&
			curr_pixel != o->end_gate_pixel
		)
		{
			// normal pixel
			n = find_neighbours(maze, curr_pixel);
		}
		else if (curr_pixel == o->start_gate_pixel)
		{
			// start gate_pixel
			n = find_start_gate_neighbours(maze, curr_pixel);
		}
		else
		{
			// end gate pixel
			n = find_end_gate_neighbours(maze, curr_pixel);
		}

		for (unsigned curr=0; curr<MAX_NEIGHBOURS; curr++)
		{
			if (n.neighbour[curr] < 0)
			{
				continue;
			}

#ifdef KS_MAZE_SOLVER_DEBUG_INITIALIZE_ADJACENCIES
			printf("%ld\t", n.neighbour[curr]);
#endif

			if (
				add_adjacency((*(np_list+clear_pixel))->pixel_node,
					      get_node(n.neighbour[curr]))
			)
			{
				return 1;
			}
		}

#ifdef KS_MAZE_SOLVER_DEBUG_INITIALIZE_ADJACENCIES
		printf("\nTotally %u adjacencies for pixel: %u\n",
			(*(np_list+clear_pixel))->pixel_node->adjlist.num,
			curr_pixel);
#endif

	}

	return 0;
}

static struct sp_queue_head *construct_shortest_path(struct openings *o)
{
	struct node *path_node = get_node(o->end_gate_pixel);

	// initialize the shortest path queue
	struct sp_queue_head *sp = malloc(sizeof(struct sp_queue_head));
	initialise_sp_queue(sp);

	while (path_node->pi != NULL)
	{
		// insert the current path node
		struct sp_queue_elem *path_elem = malloc(sizeof(struct sp_queue_elem));
		path_elem->elem = path_node->pixel;

		sp_insert_elem(sp, path_elem);

		path_node = path_node->pi;
	}

	// insert the source node
	struct sp_queue_elem *source_elem = malloc(sizeof(struct sp_queue_elem));
	source_elem->elem = o->start_gate_pixel;
	sp_insert_elem(sp, source_elem);

	return sp;
}

struct sp_queue_head *find_shortest_path(struct openings *o)
{
	struct node *start_node = get_node(o->start_gate_pixel);
	bool found_dest = false;

	// initial setup
	start_node->colour = GREY;

	// create the frotier queue head
	struct bfsfront_queue_head *frontier = malloc(sizeof(struct bfsfront_queue_head));
	initialise_bfsfront_queue(frontier);

	// insert the start node into the frontier
	struct bfsfront_queue_elem *first = malloc(sizeof(struct bfsfront_queue_elem));
	first->elem = start_node;
	bfsfront_insert_elem(frontier, first);

	while (!bfsfront_queue_empty(frontier))
	{
		struct bfsfront_queue_elem *curr_elem = bfsfront_remove_elem(frontier);
		struct node *curr = curr_elem->elem;

		for (unsigned adj=0; adj<curr->adjlist.num; adj++)
		{
			struct node *curr_adj = *(curr->adjlist.adjs + adj);

			if (curr_adj->colour == WHITE)
			{
				// set the attributes
				curr_adj->colour = GREY;
				curr_adj->src_dist = curr->src_dist+1;
				curr_adj->pi = curr;

				// insert the element into the frontier
				struct bfsfront_queue_elem *adj_elem = malloc(sizeof(struct bfsfront_queue_elem));
				adj_elem->elem = curr_adj;
				bfsfront_insert_elem(frontier, adj_elem);

				if (curr_adj->pixel == o->end_gate_pixel)
				{
					found_dest = true;
					break;
				}
			}
		}

		curr->colour = BLACK;
		free(curr_elem);

		if (found_dest)
		{
			break;
		}
	}

	// construct the shortest path from the values of the predecessors
	return construct_shortest_path(o);
}

void delete_graph(void)
{
	// initially free the individual nodes
	for (unsigned clear_pixel=0; clear_pixel<np_list_vals; clear_pixel++)
	{
		struct node **curr_pixel_node = &(*(np_list+clear_pixel))->pixel_node;
		free(*curr_pixel_node);
	}

	// now delete the np_list itself
	delete_np_list();
}

/**
 * Colour the given pixel in the maze.
 */
inline static
void colour_pixel(struct maze_image *maze, unsigned pixel)
{
	unsigned char *pixel_bytes = maze->data + get_pixel_offset(maze->width, maze->padding, pixel);

	// ordering of RGB for little-endian architecture
	static const unsigned char colour_bytes[3] = {
	   0x00, // blue,
	   0x00, // green
	   0xFF //red
	};

	for (unsigned byte=0; byte<PIXEL_BYTES; byte++)
	{
		*(pixel_bytes + byte) = colour_bytes[byte];
	}
}

void colour_path(struct maze_image *maze, struct sp_queue_head *sp)
{
	while (!sp_queue_empty(sp))
	{
		struct sp_queue_elem *curr_elem = sp_remove_elem(sp);
		colour_pixel(maze, curr_elem->elem);
		free(curr_elem);
	}
}
