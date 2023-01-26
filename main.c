/* Graph implementation in C with adjacency list */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define GRAPH_INITIAL_ALLOC 5
#define GRAPH_GROWTH_FACTOR 2

#define VERTEX_EDGES_INITIAL_SIZE 1
#define VERTEX_EDGES_GROWTH_FACTOR 1

typedef struct Vertex Vertex;
typedef void (*Vertex_getter)(Vertex *x, void *data);
typedef void (*Vertex_setter)(Vertex *x, void *data);

struct Vertex {
	unsigned int edges_no, edges_alloc;
	void *data;
	Vertex_getter vertex_getter;
	Vertex_setter vertex_setter;
	Vertex **edges;
};

typedef struct {
	Vertex **vertices;
	unsigned int vertices_no, vertices_alloc;
} Graph;


/* Functions declaration */
void graph_new(Graph **G);
void graph_print(Graph *G);
void graph_add_vertex(Graph *G, Vertex *v);
void graph_rm_vertex(Graph *G, Vertex *v);
void graph_destroy(Graph *G);

void vertex_new(Vertex **x, Vertex_getter getter, Vertex_setter setter);
void vertex_destroy(Vertex *x);
void vertex_add_edge(Vertex *x, Vertex *edge);
void vertex_rm_edge(Vertex *x, Vertex *edge);
void vertex_get_value(Vertex *x, void *data);
void vertex_set_value(Vertex *x, void *data);
void vertex_get_adjacents(Vertex *x);
bool vertex_is_adjacent(Vertex *x, Vertex *y);


/* Functions definition */
void graph_new(Graph **G) {
	(*G) = malloc(sizeof(G));
	(*G)->vertices_no = 0;
	(*G)->vertices = NULL;
}

void graph_add_vertex(Graph *G, Vertex *v) {
	/* add memory if needed */
	if(G->vertices == NULL) {
		G->vertices_alloc = GRAPH_INITIAL_ALLOC;
		G->vertices = malloc(sizeof(Vertex *) * GRAPH_INITIAL_ALLOC);
	}

	else if (G->vertices_alloc == G->vertices_no) {
		G->vertices_alloc = G->vertices_alloc * GRAPH_GROWTH_FACTOR;
		G->vertices = realloc(G->vertices,
				sizeof(Vertex *) * G->vertices_alloc);
	}

	G->vertices[G->vertices_no++] = v;
}

void graph_destroy(Graph *G) {
	if(G->vertices_no)
		for(size_t i = 0; i < G->vertices_no; i++)
			vertex_destroy(G->vertices[i]);

	free(G);
}

/* TODO: make this function generic (data type independent) */
void graph_print(Graph *G) {
	int *x;
	size_t i, j;
	Vertex *v;

	for(i = 0; i < G->vertices_no; i++) {
		v = G->vertices[i];
		vertex_get_value(v, &x);
		printf("%d%s", x, v->edges_no ? " -> " : "\n");

		for(j = 0; j < v->edges_no; j++) {
			vertex_get_value(v->edges[j], &x);
			printf("%d%s", x, (j < (v->edges_no - 1)) ? " -> " : "");
		}
		printf("\n");
	}
}

void vertex_new(Vertex **x, Vertex_getter getter, Vertex_setter setter) {
	(*x) = malloc(sizeof(Vertex));
	(*x)->edges_no = 0;
	(*x)->vertex_getter = getter;
	(*x)->vertex_setter = setter;
	(*x)->data = NULL;
	(*x)->edges = NULL;
}

/* does not destroy adjacent vertices nor edges */
void vertex_destroy(Vertex *x) {
	free(x->data);
	free(x);
}

void vertex_add_edge(Vertex *x, Vertex *edge) {
	if(x->edges == NULL)
		x->edges = malloc(sizeof(Vertex) * VERTEX_EDGES_INITIAL_SIZE);

	else if(x->edges_no == x->edges_alloc)
		x->edges = realloc(x->edges,
				x->edges_alloc + (1 * VERTEX_EDGES_GROWTH_FACTOR));

	x->edges[x->edges_no++] = edge;
}

void vertex_rm_edge(Vertex *x, Vertex *edge);

void vertex_get_value(Vertex *x, void *data) {
	x->vertex_getter(x, data);
}

void vertex_set_value(Vertex *x, void *data) {
	x->vertex_setter(x, data);
}

void vertex_list_adjacents(Vertex *x);
bool vertex_is_adjacent(Vertex *x, Vertex *y);

/* type specific functions */
void vertex_getter(Vertex *x, void *data) {
	*(int *)data = *(int *)x->data;
}

void vertex_setter(Vertex *x, void *data) {
	int *i = malloc(sizeof(int));
	*i = *(int *)data;
	x->data = i;
}


int main (void) {
	Graph *graph;
	Vertex *v;

	graph_new(&graph);

	int x, res;
	size_t i;

	for(i = 0, x = 1; i < 6; i++, x++) {
		vertex_new(&v, vertex_getter, vertex_setter);
		graph_add_vertex(graph, v);
		vertex_set_value(graph->vertices[i], &x);
	}

	vertex_add_edge(graph->vertices[0], graph->vertices[1]);
	vertex_add_edge(graph->vertices[0], graph->vertices[4]);

	vertex_add_edge(graph->vertices[1], graph->vertices[0]);
	vertex_add_edge(graph->vertices[1], graph->vertices[2]);
	vertex_add_edge(graph->vertices[1], graph->vertices[4]);

	vertex_add_edge(graph->vertices[4], graph->vertices[0]);
	vertex_add_edge(graph->vertices[4], graph->vertices[1]);
	vertex_add_edge(graph->vertices[4], graph->vertices[3]);

	vertex_add_edge(graph->vertices[2], graph->vertices[1]);
	vertex_add_edge(graph->vertices[2], graph->vertices[3]);

	vertex_add_edge(graph->vertices[3], graph->vertices[2]);
	vertex_add_edge(graph->vertices[3], graph->vertices[4]);
	vertex_add_edge(graph->vertices[3], graph->vertices[5]);

	vertex_add_edge(graph->vertices[5], graph->vertices[3]);

	graph_print(graph);

	graph_destroy(graph);
	return 0;
}