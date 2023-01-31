/* Graph implementation in C with adjacency list */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "stack.h"

#define GRAPH_INITIAL_ALLOC 5
#define GRAPH_GROWTH_FACTOR 2

#define VERTEX_EDGES_INITIAL_SIZE 1
#define VERTEX_EDGES_GROWTH_FACTOR 1

typedef struct Vertex Vertex;
typedef void (*Vertex_getter)(const Vertex *x, void *data);
typedef void (*Vertex_setter)(Vertex *x, const void *data);

struct Vertex {
	unsigned int edges_no, edges_alloc;
	void *data;
	bool visited;
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
void graph_print(const Graph *G);
void graph_print_gv(const Graph *G);
void graph_dfs(const Graph *G, const Vertex *v);
void graph_add_vertex(Graph *G, Vertex *v);
void graph_rm_vertex(Graph *G, Vertex *v);
void graph_destroy(Graph *G);

void vertex_new(Vertex **x, const Vertex_getter getter,
		const Vertex_setter setter);
void vertex_destroy(Vertex *x);
void vertex_add_edge(Vertex *x, Vertex *edge);
void vertex_rm_edge(Vertex *x, Vertex *edge);
void vertex_get_value(const Vertex *x, void *data);
void vertex_set_value(Vertex *x, const void *data);
void vertex_get_adjacents(Vertex *x);
bool vertex_is_adjacent(Vertex *x, Vertex *y);


/* Functions definition */
void graph_new(Graph **G) {
	(*G) = malloc(sizeof(Graph));
	(*G)->vertices_no = 0;
	(*G)->vertices = NULL;
}

void graph_add_vertex(Graph *G, Vertex *v) {
	/* add memory if needed */
	if (G->vertices == NULL) {
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
	for(size_t i = 0; i < G->vertices_no; i++)
		vertex_destroy(G->vertices[i]);

	free(G->vertices);
	free(G);
}

/* TODO: make print functions generic (data type independent) */
void graph_print(const Graph *G) {
	int x;
	size_t i, j;
	Vertex *v;

	puts("strict graph G {");

	for(i = 0; i < G->vertices_no; i++) {
		v = G->vertices[i];
		vertex_get_value(v, &x);
		printf("    %d%s", x, v->edges_no ? " -- " : "\n");

		for(j = 0, printf("{"); j < v->edges_no; j++) {
			vertex_get_value(v->edges[j], &x);
			printf("%d%s", x, (j < (v->edges_no - 1)) ? ", " : "}");
		}
		printf("\n");
	}
	puts("}");
}

void graph_print_gv(const Graph *G) {
	int x;
	size_t i, j;
	Vertex *v;

	puts("strict graph G {\n"
		"    rankdir=RL;\n"
		"    node [ shape=circle,\n"
			"            penwidth=2.0, \n"
			"            fontname=\"DejaVu Sans Mono\", \n"
			"            fontsize=20.0 ]\n"
		"    edge [ penwidth=2.0 ]\n");

	for(i = 0; i < G->vertices_no; i++) {
		v = G->vertices[i];
		vertex_get_value(v, &x);
		printf("    %d%s", x, v->edges_no ? " -- " : "\n");

		for(j = 0, printf("{"); j < v->edges_no; j++) {
			vertex_get_value(v->edges[j], &x);
			printf("%d%s", x, (j < (v->edges_no - 1)) ? ", " : "}");
		}
		printf("\n");
	}
	puts("}");
}

/* non-recursive Depth-First Search */
void graph_dfs(const Graph *G, const Vertex *v) {
	Stack *S;
	Vertex *aux;
	int x;

    stack_new(&S, sizeof(Vertex *));
	stack_push(S, &v);

    while(S->len) {
		stack_pop(S, &aux);

        if (!aux->visited) {
			aux->visited = true;
			vertex_get_value(aux, &x);
			printf("%d -> ", x);

            for(size_t j = 0; j < aux->edges_no; j++) {
				stack_push(S, &(aux->edges[j]));
				vertex_get_value(aux->edges[j], &x);
				printf("%d%s", x, (j < (aux->edges_no - 1)) ? " -> " : "");
			}
			printf("\n");
		}
	}
}

void vertex_new(Vertex **x, const Vertex_getter getter,
		const Vertex_setter setter) {
	(*x) = malloc(sizeof(Vertex));
	(*x)->edges_no = 0;
	(*x)->edges_alloc = 0;
	(*x)->visited = false;
	(*x)->vertex_getter = getter;
	(*x)->vertex_setter = setter;
	(*x)->data = NULL;
	(*x)->edges = NULL;
}

/* does not destroy adjacent vertices nor edges */
void vertex_destroy(Vertex *x) {
	free(x->data);
	free(x->edges);
	free(x);
}

void vertex_add_edge(Vertex *x, Vertex *edge) {
	if (x->edges == NULL)
		x->edges = malloc(sizeof(Vertex) * VERTEX_EDGES_INITIAL_SIZE);

	else if (x->edges_no == x->edges_alloc)
		x->edges = realloc(x->edges,
				x->edges_alloc + (1 * VERTEX_EDGES_GROWTH_FACTOR));

	x->edges[x->edges_no++] = edge;
}

void vertex_get_value(const Vertex *x, void *data) {
	x->vertex_getter(x, data);
}

void vertex_set_value(Vertex *x, const void *data) {
	x->vertex_setter(x, data);
}

/* type specific functions */
void vertex_getter(const Vertex *x, void *data) {
	*(int *)data = *(int *)x->data;
}

void vertex_setter(Vertex *x, const void *data) {
	int *i = malloc(sizeof(int));
	*i = *(int *)data;
	x->data = i;
}


int main (void) {
	Graph *graph;
	Vertex *v;

	graph_new(&graph);

	int x;
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

	graph_dfs(graph, graph->vertices[0]);
	graph_destroy(graph);
	return 0;
}
