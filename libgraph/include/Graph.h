/**
 *
 * \file Math.h
 *
 * \brief Math functions utility
 *
 * \version 0.1
 * \date  17.07.2015 14:26:27
 * \author Loïc BARTOLETTI, l.bartoletti@free.fr
 *
 */

#ifndef DYABL_GRAPH_H__
#define DYABL_GRAPH_H__

/*
 * Matrice d'adjacence statique / set (di)graph
 *
 * - création avec un nombre de noeuds
 * - possibilité d'ajouter des boucles sur le noeud
 * -
 *
 * Liste d'adjacence / set (di)graph
 * - Ajout noeud ; si pas existant on l'insere
 * - Ajout arcs ; si existent pas, possibilité de l'insérer
 * - suppression noeud ; option pour ne pas supprimer si pas vide
 * - suppression arcs ;
 * - membre : nb_arcs, nb_noeuds
 * - Vérifie si arc
 * - Vérifie si noeud
 * - Retourne liste adjacent d'un noeud
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "SList.h"
#include "Utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef int (*compar)(const void *, const void *);

typedef enum graph_config_error {
  GRAPH_ERR = -1,
  GRAPH_ERR_NOGRAPH = -2,
  GRAPH_ERR_NOCOMPAR = -3,
  GRAPH_ERR_NODESTROY = -4,
  GRAPH_ERR_MEMORY = -5,
  GRAPH_ERR_ADJLIST = -6,
  GRAPH_SUCCESS = 1
} error_graph;

static errordesc graph_errors[] = {
    {GRAPH_ERR, "Error"},
    {GRAPH_ERR_NOGRAPH, "No graph"},
    {GRAPH_ERR_NOCOMPAR, "No compar function setted"},
    {GRAPH_ERR_NODESTROY, "No destroy function setted"},
    {GRAPH_ERR_MEMORY, "Cannot allocate memory"},
    {GRAPH_ERR_ADJLIST, "Error with the adjacent list"},
    {GRAPH_SUCCESS, "No error"}};

/***************************************************************************************/
/* Graph structure */
/**
 * graph_elem (aka Vertex/Node)
 * @brief Element into a single list.
 *
 * graph_elem contain node and data for an element of single list.
 */
typedef struct graph_elem {
  void *data;      /* !< data of the element. */
  SList *adjacent; /* !< node of the element. */
} Graph_elem;      /* ----------  end of struct
                    * graph_elem  ---------- */

/**
 * graph
 * @brief The graph.
 *
 * graph is the a single list structure.
 */
typedef struct graph {
  SList *adjList;                            /* !< The graph. */
  void (*destroy)(void *data);               /* !< function to delete
                                              * data element. */
  int (*compar)(const void *, const void *); /* !< function to compar
                                              * elements. */
  long nb_edges;                             /* !< number of edges into the
                                              * graph. */
  long nb_vertices;                          /* !< number of vertices into the
                                              * graph. */
  bool isDirected; /* !< True if the graph is directed, else is undirected */

} Graph; /* ----------  end of struct graph
          * ---------- */

/***************************************************************************************/
/** Graph initialization */
/**
 * @brief Create a graph.
 *
 * @return A new empty graph, else NULL.
 */
Graph *Graph_new(void);

/**
 * @brief Create a graph and initialize functions (delete and compar).
 *
 * @return A new empty graph, else NULL.
 */
Graph *Graph_new_init(void (*destroy)(void *data),
                      int (*compar)(const void *, const void *),
                      bool isDirected);

/**
 * @brief Set the comparison function.
 *
 * @param[in] graph The graph
 * @param[in] compar The function which performes the destruction
 *
 * @return GRAPH_ERR_NOGRAPH or GRAPH_SUCCESS.
 */
int Graph_set_compar(Graph *graph, int (*compar)(const void *, const void *));

/**
 * @brief Set the destroy function.
 *
 * @param[in] graph The graph
 * @param[in] destroy The function which performes the comparison
 *
 * @return GRAPH_ERR_NOGRAPH or GRAPH_SUCCESS.
 */
int Graph_set_destroy(Graph *graph, void (*destroy)(void *data));

/**
 * @brief Set if the graph is directed or not
 *
 * @param[in] graph The graph
 * @param isDirected Directed or not
 *
 * @return GRAPH_ERR_NOGRAPH or GRAPH_SUCCESS.
 */
int Graph_set_directed(Graph *graph, bool isDirected);

/**
 * @brief Undirect a graph
 *
 * @param graph The graph to undirect
 */
void Graph_undirect(Graph *graph);

/***************************************************************************************/
/* Graph add elements */
/**
 * @brief Add a vertex in the graph.
 *
 * @param[in] graph The graph to add an element with data.
 * @param[in] data Data to insert into the graph.
 *
 * @return GRAPH_ERR_NOGRAPH, GRAPH_ERR_MEMORY or GRAPH_SUCCESS.
 */
int Graph_add_vertex(Graph *graph, const void *data);

/**
 * @brief Add an edge in the graph.
 *
 * @param[in] graph The graph to append an element with data.
 * @param[in] data1 First data of the edge to insert into the graph.
 * @param[in] data2 Second data of the edge to insert into the graph.
 *
 * @return GRAPH_ERR_NOGRAPH, GRAPH_ERR_MEMORY or GRAPH_SUCCESS.
 */
int Graph_add_edge(Graph *graph, const void *data1, const void *data2);

/***************************************************************************************/
/* Graph remove elements */
/**
 * @brief Remove a vertex, get data for working with it and/or free
 * it.
 *
 * @param[in] graph The graph to remove an element.
 * @param[out] data The deleted data.
 *
 * @return GRAPH_ERR_NOGRAPH, GRAPH_ERR or GRAPH_SUCCESS.
 */
int Graph_remove_vertex(Graph *graph, const void *data);

/**
 * @brief Remove an edge, get data for working with it and/or free
 * it.
 *
 * @param[in] graph The graph to remove an element.
 * @param[out] data The deleted data.
 *
 * @return GRAPH_ERR_NOGRAPH, GRAPH_ERR or GRAPH_SUCCESS.
 */
int Graph_remove_edge(Graph *graph, void *data, void **data2);

/***************************************************************************************/
/* Graph free and delete the graph */
/**
 * @brief Free all of the memory used by a graph and delete it
 *
 * @param graph The graph to free'd
 *
 * @return GRAPH_ERR_NOGRAPH or GRAPH_SUCCESS.
 */
int Graph_free(Graph *graph);

/**
 * @brief Empty and free all of the memory used by a graph
 *
 * @param graph The graph to empty
 *
 * @return GRAPH_ERR_NOGRAPH or GRAPH_SUCCESS.
 */
int Graph_empty(Graph *graph);

/***************************************************************************************/
/* Graph macros */
/** Return the number of vertices in a graph */
#define Graph_vertices_count(graph) ((graph)->nb_vertices)
/** Return the number of edges in a graph */
#define Graph_edges_count(graph)                                               \
  ((graph)->isDirected ? (graph)->nb_edges : (graph)->nb_edges / 2)

/***************************************************************************************/
/* Graph utility */

/* --- Traversal functions --- */
/**
 * Print all vertex and edge data of the graph - on screen
 *
 * @param[in] graph - a reference to current graph.
 * @param[in] vtxcallback -  reference to user-defined callback function,
 * that gets read access to vertex data via its parameter @a vtxcallback -
 * to do whatever is relevant. In this case it is a matter of formatting vertex
 * data for printing on screen. The printed data should be kept to a
 * minimum (the key value, for example) in order not to clutter the screen.
 * This function is primarily for small graphs and debugging purposes
 * @param[in] edgecallback - reference to user-defined callback function,
 * that gets read access to edge data via its parameter @a edgecallback -
 * to do whatever is relevant. In this case it is a matter of formatting edge
 * data for printing on screen. The printed data should be kept to a
 * minimum (the key value, for example) in order not to clutter the screen.
 * This function is primarily for small graphs and debugging purposes
 * @return Nothing.
 *
 **/
int Graph_Print(Graph *graph, void (*vtxcallback)(const void *data),
                void (*edgecallback)(const void *data));

/**
 * Traverse all vertices/edges of the graph
 *
 *
 * @param[in] graph - a reference to current graph.
 * @param[in] vtxcallback -  reference to user-defined callback function,
 * that gets read access to vertex data via its parameter @a vtxcallback -
 * to do whatever is relevant.
 * @param[in] edgecallback - reference to user-defined callback function,
 * that gets read access to edge data via its parameter @a edgecallback -
 * to do whatever is relevant.
 * @return Nothing.
 **/
int Graph_Traverse(Graph *graph, void (*vtxcallback)(const void *data),
                   void (*edgecallback)(const void *data));

/* --- Miscellaneous functions --- */
/**
 * Determine if 2 vertices are adjacent - i.e. if there is an edge connecting
 *them
 *
 * More in detail, this function determines if there is an edge - <i>incident
 *from</i> vertex matching search key data referenced by parameter @a vtxdata -
 *which is <i>incident to</i> another vertex, matching search key data
 *referenced by parameter
 * @a edgedata. Or - equivalently put - if vertex matching search key
 * data referenced by @a edgedata is @a present in the <i>adjacency list</i> of
 *vertex specified by parameter @a vtxdata.
 *
 * @param[in] graph - a reference to current graph.
 * @param[in] data1 - reference to search key data for vertex, which the edge is
 *<i>incident from</i>.
 * @param[in] data2 - reference to search key data for vertex, which the edge is
 *<i>incident to</i>.
 * @return  Value 1 - if there is an edge incident from vertex specified by @a
 *vtxdata - (incident) to a vertex specified by @a edgedata - or 0 otherwise.
 **/
bool Graph_isAdjacent(const Graph *graph, const void *data1, const void *data2);

/**
 * Determine if a vertex is isolated - i.e. has no (in- or outgoing) edges
 *
 * If a vertex is to be considered isolated - the following must be true:
 * - The vertex specified with parameter @a vtxdata must <i>not have any edges
 * incident to it</i> - i.e. it must @a not be part of any adjacency list in the
 * graph, whatsoever.
 * - Moreover, this vertex must <i>not have any edges incident from it</i> -
 *i.e. its own <i>adjacency list</i> must be <i>empty</i>.
 *
 * @param[in] graph - a reference to current graph.
 * @param[in] data - reference to search key data for target vertex.
 * @return true - if target vertex is isolated - false otherwise (including if
 *graph doesn't exits).
 *
 **/
bool Graph_isIsolated(const Graph *graph, const void *data);

/** Trouve un element et retourne le noeud de la liste
 * NULL si pas trouvé */
SList_elem *Graph_find_vertext(const Graph *graph, const void *data);

#ifdef __cplusplus
}

#endif

#endif
