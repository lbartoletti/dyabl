/**
 *
 * \file Graph.c
 *
 * \brief Graph functions utility
 *
 * \version 0.1
 * \date  17.07.2015 14:26:27
 * \author Loïc BARTOLETTI, l.bartoletti@free.fr
 *
 */

#include "Graph.h"
#include "Utils.h"

static int Graph_new_empty(Graph *graph) {
  if (!graph) {
    return GRAPH_ERR_NOGRAPH;
  }

  graph->compar = NULL;
  graph->destroy = NULL;
  graph->isDirected = false;
  graph->nb_edges = 0;
  graph->nb_vertices = 0;

  return GRAPH_SUCCESS;
}

Graph *Graph_new() {
  Graph *graph = NULL;
  graph = malloc(sizeof(struct graph));

  if (!graph) {
    return NULL;
  }

  Graph_new_empty(graph);

  graph->adjList = SList_new();
  if (!graph->adjList) {
    free(graph);
    graph = NULL;
  }

  return graph;
}

Graph *Graph_new_init(void (*destroy)(void *),
                      int (*compar)(const void *, const void *),
                      bool isDirected) {
  Graph *graph = NULL;
  graph = Graph_new();

  if (!graph) {
    return NULL;
  }

  int ret = Graph_set_compar(graph, compar);
  if (ret != GRAPH_SUCCESS) {
    return graph;
  }
  ret = Graph_set_destroy(graph, destroy);
  if (ret != GRAPH_SUCCESS) {
    return graph;
  }
  ret = Graph_set_directed(graph, isDirected);
  if (ret != GRAPH_SUCCESS) {
    return graph;
  }

  return graph;
}

int Graph_set_compar(Graph *graph, int (*compar)(const void *, const void *)) {
  if (!graph) {
    return GRAPH_ERR_NOGRAPH;
  }

  if (SList_set_compar(graph->adjList, compar) != SLIST_SUCCESS) {
    return GRAPH_ERR_ADJLIST;
  }

  graph->compar = compar;

  return GRAPH_SUCCESS;
}

int Graph_set_destroy(Graph *graph, void (*destroy)(void *)) {
  if (!graph) {
    return GRAPH_ERR_NOGRAPH;
  }

  if (SList_set_destroy(graph->adjList, destroy) != SLIST_SUCCESS) {
    return GRAPH_ERR_ADJLIST;
  }

  graph->destroy = destroy;

  return GRAPH_SUCCESS;
}

int Graph_set_directed(Graph *graph, bool isDirected) {
  if (!graph) {
    return GRAPH_ERR_NOGRAPH;
  }

  graph->isDirected = isDirected;

  return GRAPH_SUCCESS;
}

int Graph_free(Graph *graph) {
  int ret = Graph_empty(graph);
  if (ret == GRAPH_SUCCESS) {
    free(graph);
  }

  return ret;
}

int Graph_empty(Graph *graph) {
  if (SList_empty(graph->adjList) == SLIST_SUCCESS) {
    return Graph_new_empty(graph);
  }

  return GRAPH_ERR_ADJLIST;
}

int Graph_add_vertex(Graph *graph, const void *data) {
  if (!graph) {
    return GRAPH_ERR_NOGRAPH;
  }
  if (!graph->compar) {
    return GRAPH_ERR_NOCOMPAR;
  }

  SList_elem *elm = NULL;
  elm = Graph_find_vertext(graph, data);
  if (elm != NULL) {
    return 0;
  }

  Graph_elem *vertex = NULL;
  if ((vertex = malloc(sizeof(struct graph_elem))) == NULL) {
    return GRAPH_ERR_MEMORY;
  }

  if ((vertex->adjacent = SList_new_init(graph->destroy, graph->compar)) ==
      NULL) {
    free(vertex);
    return GRAPH_ERR_ADJLIST;
  }

  vertex->data = DECONST(void *, data);

  int ret = SList_append(graph->adjList, vertex);
  if (ret == SLIST_SUCCESS) {
    graph->nb_vertices++;
  }

  return ret;
}

SList_elem *Graph_find_vertext(const Graph *graph, const void *data) {
  if (!graph) {
    return NULL;
  }
  if (!graph->compar) {
    return NULL;
  }

  SList *list = graph->adjList;
  SList_elem *elm = NULL;
  for (elm = SList_head(list); elm != NULL; elm = elm->next) {
    if (graph->compar(data, ((Graph_elem *)(elm->data))->data) == 0) {
      return elm;
    }
  }

  return NULL;
}

int Graph_add_edge(Graph *graph, const void *data1, const void *data2) {
  if (!graph) {
    return GRAPH_ERR_NOGRAPH;
  }

  SList_elem *elm1 = Graph_find_vertext(graph, data1);
  if (elm1 == NULL) {
    int ret = Graph_add_vertex(graph, data1);
    if (ret != GRAPH_SUCCESS) {
      return ret;
    }
    elm1 = SList_tail(graph->adjList);
  }
  SList_elem *elm2 = Graph_find_vertext(graph, data2);
  if (elm2 == NULL) {
    int ret = Graph_add_vertex(graph, data2);
    if (ret != GRAPH_SUCCESS) {
      return ret;
    }
    elm2 = SList_tail(graph->adjList);
  }
  if (SList_unique_insert(((Graph_elem *)(elm1->data))->adjacent, elm2->data) ==
      SLIST_SUCCESS) {
    graph->nb_edges++;
  } else {
    return GRAPH_ERR_ADJLIST;
  }

  if (!graph->isDirected) {
    if (SList_unique_insert(((Graph_elem *)(elm2->data))->adjacent,
                            elm1->data)) {
      graph->nb_edges++;
    } else {
      return GRAPH_ERR_ADJLIST;
    }
  }

  return GRAPH_SUCCESS;
}

int Graph_remove_vertex(Graph *graph, const void *data) {
  if (!graph) {
    return GRAPH_ERR_NOGRAPH;
  }

  SList_elem *elm = Graph_find_vertext(graph, data);
  if (elm == NULL) {
    return GRAPH_ERR;
  }

  SList_elem *listElm = NULL;
  if (graph->isDirected) {
    listElm = SList_head(graph->adjList);
  } else {
    listElm = SList_head(((Graph_elem *)(elm->data))->adjacent);
  }
  for (; listElm != NULL; listElm = listElm->next) {
    SList_elem *adjElem = NULL;
    adjElem = Graph_find_vertext(graph, ((Graph_elem *)(listElm->data))->data);

    if ((!graph->isDirected) && (adjElem == NULL)) {
      return GRAPH_ERR;
    }
    if (adjElem) {
      long before = ((Graph_elem *)(adjElem->data))->adjacent->nb_elem;
      int ret =
          SList_remove(((Graph_elem *)(adjElem->data))->adjacent, elm->data);
      if (ret == SLIST_ERR) {
        return GRAPH_ERR_ADJLIST;
      }

      if (before != ((Graph_elem *)(adjElem->data))->adjacent->nb_elem) {
        graph->nb_edges--;
        if (!graph->isDirected) {
          graph->nb_edges--;
        }
      }
    }
  }

  int ret = SList_remove(graph->adjList, elm->data);
  if (ret != SLIST_SUCCESS) {
    return ret;
  }

  graph->nb_vertices--;

  return GRAPH_SUCCESS;
}

int Graph_remove_edge(Graph *graph, void *data, void **data2) {
  if (!graph) {
    return GRAPH_ERR_NOGRAPH;
  }

  (void)graph;
  (void)data;
  (void)data2;

  return 0;
}

int Graph_Print(Graph *graph, void (*vtxcallback)(const void *),
                void (*edgecallback)(const void *)) {
  if (!graph) {
    return GRAPH_ERR_NOGRAPH;
  }

  graph;
  vtxcallback(NULL);
  edgecallback(NULL);

  return 0;
}

int Graph_Traverse(Graph *graph, void (*vtxcallback)(const void *),
                   void (*edgecallback)(const void *)) {
  if (!graph) {
    return GRAPH_ERR_NOGRAPH;
  }

  graph;
  vtxcallback(NULL);
  edgecallback(NULL);

  return 0;
}

bool Graph_isAdjacent(const Graph *graph, const void *data1,
                      const void *data2) {
  if (!graph) {
    return false;
  }

  SList_elem *elm1 = Graph_find_vertext(graph, data1);
  if (elm1 == NULL) {
    return false;
  }
  SList_elem *elm2 = Graph_find_vertext(graph, data2);
  if (elm2 == NULL) {
    return false;
  }

  SList_elem *adjElem = NULL;
  adjElem = SList_find(((Graph_elem *)(elm1->data))->adjacent, elm2->data);
  if (adjElem) {
    return true;
  }

  return false;
}

bool Graph_isIsolated(const Graph *graph, const void *data) {
  if (!graph) {
    return false;
  }

  SList_elem *elm = Graph_find_vertext(graph, data);
  if (elm == NULL) {
    return false;
  }

  if (graph->isDirected) {
    for (SList_elem *listElm = SList_head(graph->adjList); listElm != NULL;
         listElm = listElm->next) {
      SList_elem *adjElem = NULL;
      adjElem =
          SList_find(((Graph_elem *)(listElm->data))->adjacent, elm->data);
      if (adjElem) {
        return false;
      }
    }
  }

  if (((Graph_elem *)(elm->data))->adjacent->nb_elem > 0) {
    return false;
  }

  return true;
}

void Graph_undirect(Graph *graph) { (void)graph; }
