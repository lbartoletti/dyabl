extern "C" {
#include "Graph.h"
#include <string.h>

static void del(void *data) { printf("rm %ld\n", (intptr_t)data); }

static int q[] = {65, 78, 98, 12, 78, 312, 6};

static char str[100];

static void affInt(const void *data) {
  char tmp[100];
  int d = (int)(intptr_t)((Graph_elem *)data)->data;
  snprintf(tmp, 100, "%d", d);
  if (strlen(str) > 0)
    strncat(str, " ", 1);
  size_t size;
  size = 100 - strlen(str) - 1;
  strncat(str, tmp, size);
}

static char *printSL(SList *l) {
  memset(str, 0, 100);
  SList_foreach_safe(l, affInt);
  return str;
}
}

#include <gtest/gtest.h>

TEST(GRAPH, init) {
  Graph *graph = NULL;
  graph = Graph_new();
  ASSERT_NE(graph, nullptr);
  ASSERT_EQ(graph->destroy, nullptr);
  ASSERT_EQ(graph->compar, nullptr);
  ASSERT_TRUE(graph->isDirected == false);
  ASSERT_TRUE(Graph_edges_count(graph) == 0);
  ASSERT_TRUE(Graph_vertices_count(graph) == 0);
  Graph_free(graph);

  graph = Graph_new_init(NULL, NULL, false);
  ASSERT_NE(graph, nullptr);
  ASSERT_EQ(graph->destroy, nullptr);
  ASSERT_EQ(graph->compar, nullptr);
  ASSERT_TRUE(graph->isDirected == false);
  ASSERT_TRUE(Graph_edges_count(graph) == 0);
  ASSERT_TRUE(Graph_vertices_count(graph) == 0);
  Graph_free(graph);
}

TEST(GRAPH, setters) {
  Graph *graph = NULL;
  ASSERT_TRUE(GRAPH_ERR_NOGRAPH == Graph_set_compar(graph, comparIntptr));
  ASSERT_TRUE(GRAPH_ERR_NOGRAPH == Graph_set_destroy(graph, del));
  ASSERT_TRUE(GRAPH_ERR_NOGRAPH == Graph_set_directed(graph, true));
  graph = Graph_new();
  ASSERT_NE(graph, nullptr);
  Graph_set_compar(graph, comparIntptr);
  ASSERT_TRUE(graph->compar == comparIntptr);
  Graph_set_destroy(graph, del);
  ASSERT_TRUE(graph->destroy == del);
  Graph_set_directed(graph, true);
  ASSERT_TRUE(graph->isDirected == true);
  Graph_free(graph);
}

TEST(GRAPH, add_vertex) {
  Graph *graph = NULL;
  ASSERT_TRUE(GRAPH_ERR_NOGRAPH ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[1]));
  graph = Graph_new();
  ASSERT_NE(graph, nullptr);
  ASSERT_TRUE(GRAPH_ERR_NOCOMPAR ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[1]));
  Graph_set_compar(graph, comparIntptr);
  ASSERT_EQ(Graph_vertices_count(graph), 0);
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[1]));
  ASSERT_EQ((const void *)(intptr_t)q[1],
            ((Graph_elem *)SList_head(graph->adjList)->data)->data);
  ASSERT_EQ(Graph_vertices_count(graph), 1);
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[2]));
  ASSERT_EQ((const void *)(intptr_t)q[2],
            ((Graph_elem *)SList_tail(graph->adjList)->data)->data);
  ASSERT_EQ(Graph_vertices_count(graph), 2);
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[3]));
  ASSERT_EQ((const void *)(intptr_t)q[3],
            ((Graph_elem *)SList_tail(graph->adjList)->data)->data);
  ASSERT_EQ(Graph_vertices_count(graph), 3);
  ASSERT_TRUE(0 == Graph_add_vertex(graph, (const void *)(intptr_t)q[3]));
  ASSERT_EQ(Graph_vertices_count(graph), 3);
  ASSERT_TRUE(0 == Graph_add_vertex(graph, (const void *)(intptr_t)q[4]));
  ASSERT_EQ(Graph_vertices_count(graph), 3);
  Graph_free(graph);
}

TEST(GRAPH, has_vertex) {
  Graph *graph = NULL;
  ASSERT_TRUE(GRAPH_ERR_NOGRAPH ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[1]));
  graph = Graph_new();
  ASSERT_NE(graph, nullptr);
  ASSERT_TRUE(GRAPH_ERR_NOCOMPAR ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[1]));
  Graph_set_compar(graph, comparIntptr);
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[1]));
  ASSERT_EQ((const void *)(intptr_t)q[1],
            ((Graph_elem *)SList_head(graph->adjList)->data)->data);
  ASSERT_EQ(Graph_vertices_count(graph), 1);
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[2]));
  ASSERT_EQ((const void *)(intptr_t)q[2],
            ((Graph_elem *)SList_tail(graph->adjList)->data)->data);
  ASSERT_EQ(Graph_vertices_count(graph), 2);
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[3]));
  ASSERT_EQ((const void *)(intptr_t)q[3],
            ((Graph_elem *)SList_tail(graph->adjList)->data)->data);

  ASSERT_NE(Graph_find_vertext(graph, (const void *)(intptr_t)q[1]), nullptr);
  ASSERT_EQ(Graph_find_vertext(graph, (const void *)(intptr_t)q[5]), nullptr);
  Graph_free(graph);
}

TEST(GRAPH, add_edge) {
  /* undirected */
  Graph *graph = NULL;
  ASSERT_TRUE(GRAPH_ERR_NOGRAPH ==
              Graph_add_edge(graph, (const void *)(intptr_t)q[1],
                             (const void *)(intptr_t)q[2]));
  graph = Graph_new();
  ASSERT_NE(graph, nullptr);
  ASSERT_TRUE(GRAPH_ERR_NOCOMPAR ==
              Graph_add_edge(graph, (const void *)(intptr_t)q[1],
                             (const void *)(intptr_t)q[2]));
  Graph_set_compar(graph, comparIntptr);
  ASSERT_EQ(Graph_vertices_count(graph), 0);
  ASSERT_EQ(Graph_edges_count(graph), 0);
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[1]));
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[2]));
  ASSERT_TRUE(GRAPH_SUCCESS == Graph_add_edge(graph,
                                              (const void *)(intptr_t)q[1],
                                              (const void *)(intptr_t)q[2]));
  ASSERT_EQ(Graph_vertices_count(graph), 2);
  ASSERT_EQ(Graph_edges_count(graph), 1);
  ASSERT_EQ(graph->nb_edges, 2);
  ASSERT_TRUE(GRAPH_SUCCESS == Graph_add_edge(graph,
                                              (const void *)(intptr_t)q[1],
                                              (const void *)(intptr_t)q[3]));
  ASSERT_EQ(Graph_vertices_count(graph), 3);
  ASSERT_EQ(Graph_edges_count(graph), 2);
  ASSERT_EQ(graph->nb_edges, 4);
  ASSERT_TRUE(GRAPH_SUCCESS == Graph_add_edge(graph,
                                              (const void *)(intptr_t)q[0],
                                              (const void *)(intptr_t)q[5]));
  ASSERT_EQ(Graph_vertices_count(graph), 5);
  ASSERT_EQ(Graph_edges_count(graph), 3);
  ASSERT_EQ(graph->nb_edges, 6);
  ASSERT_TRUE(GRAPH_ERR_ADJLIST ==
              Graph_add_edge(graph, (const void *)(intptr_t)q[5],
                             (const void *)(intptr_t)q[0]));
  ASSERT_EQ(Graph_vertices_count(graph), 5);
  ASSERT_EQ(Graph_edges_count(graph), 3);
  ASSERT_EQ(graph->nb_edges, 6);

  SList_elem *elm = Graph_find_vertext(graph, (const void *)(intptr_t)q[1]);
  ASSERT_NE(elm, nullptr);
  SList *list = ((Graph_elem *)elm->data)->adjacent;
  ASSERT_EQ(SList_length(list), 2);
  ASSERT_EQ(((Graph_elem *)SList_head(list)->data)->data,
            (const void *)(intptr_t)q[2]);
  ASSERT_EQ(((Graph_elem *)SList_tail(list)->data)->data,
            (const void *)(intptr_t)q[3]);
  Graph_free(graph);
  graph = NULL;

  /* directed */
  ASSERT_TRUE(GRAPH_ERR_NOGRAPH ==
              Graph_add_edge(graph, (const void *)(intptr_t)q[1],
                             (const void *)(intptr_t)q[2]));
  graph = Graph_new();
  ASSERT_NE(graph, nullptr);
  ASSERT_TRUE(GRAPH_ERR_NOCOMPAR ==
              Graph_add_edge(graph, (const void *)(intptr_t)q[1],
                             (const void *)(intptr_t)q[2]));
  Graph_set_compar(graph, comparIntptr);
  Graph_set_directed(graph, true);
  ASSERT_EQ(Graph_vertices_count(graph), 0);
  ASSERT_EQ(Graph_edges_count(graph), 0);
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[1]));
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[2]));
  ASSERT_TRUE(GRAPH_SUCCESS == Graph_add_edge(graph,
                                              (const void *)(intptr_t)q[1],
                                              (const void *)(intptr_t)q[2]));
  ASSERT_EQ(Graph_vertices_count(graph), 2);
  ASSERT_EQ(Graph_edges_count(graph), 1);
  ASSERT_TRUE(GRAPH_SUCCESS == Graph_add_edge(graph,
                                              (const void *)(intptr_t)q[1],
                                              (const void *)(intptr_t)q[3]));
  ASSERT_EQ(Graph_vertices_count(graph), 3);
  ASSERT_EQ(Graph_edges_count(graph), 2);
  ASSERT_TRUE(GRAPH_SUCCESS == Graph_add_edge(graph,
                                              (const void *)(intptr_t)q[0],
                                              (const void *)(intptr_t)q[5]));
  ASSERT_EQ(Graph_vertices_count(graph), 5);
  ASSERT_EQ(Graph_edges_count(graph), 3);
  ASSERT_TRUE(GRAPH_SUCCESS == Graph_add_edge(graph,
                                              (const void *)(intptr_t)q[5],
                                              (const void *)(intptr_t)q[0]));
  ASSERT_EQ(Graph_vertices_count(graph), 5);
  ASSERT_EQ(Graph_edges_count(graph), 4);
  Graph_free(graph);
}

TEST(GRAPH, remove_vertex) {
  /* Undirected */
  Graph *graph = NULL;
  graph = Graph_new();
  ASSERT_NE(graph, nullptr);
  Graph_set_compar(graph, comparIntptr);
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[1]));
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[2]));
  ASSERT_TRUE(GRAPH_SUCCESS == Graph_add_edge(graph,
                                              (const void *)(intptr_t)q[1],
                                              (const void *)(intptr_t)q[2]));
  ASSERT_TRUE(GRAPH_SUCCESS == Graph_add_edge(graph,
                                              (const void *)(intptr_t)q[1],
                                              (const void *)(intptr_t)q[3]));
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[0]));
  ASSERT_EQ(Graph_vertices_count(graph), 4);
  ASSERT_EQ(Graph_edges_count(graph), 2);

  ASSERT_EQ(GRAPH_SUCCESS,
            Graph_remove_vertex(graph, (const void *)(intptr_t)q[3]));
  ASSERT_EQ(Graph_vertices_count(graph), 3);
  ASSERT_EQ(Graph_edges_count(graph), 1);

  ASSERT_EQ(GRAPH_ERR,
            Graph_remove_vertex(graph, (const void *)(intptr_t)q[3]));
  ASSERT_EQ(Graph_vertices_count(graph), 3);
  ASSERT_EQ(Graph_edges_count(graph), 1);

  ASSERT_EQ(GRAPH_SUCCESS,
            Graph_remove_vertex(graph, (const void *)(intptr_t)q[0]));
  ASSERT_EQ(Graph_vertices_count(graph), 2);
  ASSERT_EQ(Graph_edges_count(graph), 1);

  ASSERT_EQ(GRAPH_SUCCESS,
            Graph_remove_vertex(graph, (const void *)(intptr_t)q[2]));
  ASSERT_EQ(Graph_vertices_count(graph), 1);
  ASSERT_EQ(Graph_edges_count(graph), 0);

  ASSERT_EQ(GRAPH_ERR,
            Graph_remove_vertex(graph, (const void *)(intptr_t)q[2]));
  ASSERT_EQ(Graph_vertices_count(graph), 1);
  ASSERT_EQ(Graph_edges_count(graph), 0);
  Graph_free(graph);

  /* directed */
  graph = Graph_new();
  ASSERT_NE(graph, nullptr);
  Graph_set_compar(graph, comparIntptr);
  Graph_set_directed(graph, true);
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[1]));
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[2]));
  ASSERT_TRUE(GRAPH_SUCCESS == Graph_add_edge(graph,
                                              (const void *)(intptr_t)q[1],
                                              (const void *)(intptr_t)q[2]));
  ASSERT_TRUE(GRAPH_SUCCESS == Graph_add_edge(graph,
                                              (const void *)(intptr_t)q[1],
                                              (const void *)(intptr_t)q[3]));
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[0]));
  ASSERT_EQ(Graph_vertices_count(graph), 4);
  ASSERT_EQ(Graph_edges_count(graph), 2);

  ASSERT_EQ(GRAPH_SUCCESS,
            Graph_remove_vertex(graph, (const void *)(intptr_t)q[3]));
  ASSERT_EQ(Graph_vertices_count(graph), 3);
  ASSERT_EQ(Graph_edges_count(graph), 1);

  ASSERT_EQ(GRAPH_ERR,
            Graph_remove_vertex(graph, (const void *)(intptr_t)q[3]));
  ASSERT_EQ(Graph_vertices_count(graph), 3);
  ASSERT_EQ(Graph_edges_count(graph), 1);

  ASSERT_EQ(GRAPH_SUCCESS,
            Graph_remove_vertex(graph, (const void *)(intptr_t)q[0]));
  ASSERT_EQ(Graph_vertices_count(graph), 2);
  ASSERT_EQ(Graph_edges_count(graph), 1);

  ASSERT_EQ(GRAPH_SUCCESS,
            Graph_remove_vertex(graph, (const void *)(intptr_t)q[2]));
  ASSERT_EQ(Graph_vertices_count(graph), 1);
  ASSERT_EQ(Graph_edges_count(graph), 0);

  ASSERT_EQ(GRAPH_ERR,
            Graph_remove_vertex(graph, (const void *)(intptr_t)q[2]));
  ASSERT_EQ(Graph_vertices_count(graph), 1);
  ASSERT_EQ(Graph_edges_count(graph), 0);
  Graph_free(graph);
}

TEST(GRAPH, isIsolated) {
  /* Undirected */
  Graph *graph = NULL;
  ASSERT_FALSE(Graph_isIsolated(graph, (const void *)(intptr_t)q[5]));
  graph = Graph_new();
  ASSERT_NE(graph, nullptr);
  Graph_set_compar(graph, comparIntptr);
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[1]));
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[2]));
  ASSERT_TRUE(GRAPH_SUCCESS == Graph_add_edge(graph,
                                              (const void *)(intptr_t)q[1],
                                              (const void *)(intptr_t)q[2]));
  ASSERT_TRUE(GRAPH_SUCCESS == Graph_add_edge(graph,
                                              (const void *)(intptr_t)q[1],
                                              (const void *)(intptr_t)q[3]));
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[0]));
  ASSERT_FALSE(Graph_isIsolated(graph, (const void *)(intptr_t)q[5]));
  ASSERT_FALSE(Graph_isIsolated(graph, (const void *)(intptr_t)q[3]));
  ASSERT_FALSE(Graph_isIsolated(graph, (const void *)(intptr_t)q[2]));
  ASSERT_FALSE(Graph_isIsolated(graph, (const void *)(intptr_t)q[1]));
  ASSERT_TRUE(Graph_isIsolated(graph, (const void *)(intptr_t)q[0]));
  Graph_free(graph);

  /* directed */
  graph = Graph_new();
  ASSERT_NE(graph, nullptr);
  Graph_set_compar(graph, comparIntptr);
  Graph_set_directed(graph, true);
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[1]));
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[2]));
  ASSERT_TRUE(GRAPH_SUCCESS == Graph_add_edge(graph,
                                              (const void *)(intptr_t)q[1],
                                              (const void *)(intptr_t)q[2]));
  ASSERT_TRUE(GRAPH_SUCCESS == Graph_add_edge(graph,
                                              (const void *)(intptr_t)q[1],
                                              (const void *)(intptr_t)q[3]));
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[0]));
  ASSERT_FALSE(Graph_isIsolated(graph, (const void *)(intptr_t)q[5]));
  ASSERT_FALSE(Graph_isIsolated(graph, (const void *)(intptr_t)q[3]));
  ASSERT_FALSE(Graph_isIsolated(graph, (const void *)(intptr_t)q[2]));
  ASSERT_FALSE(Graph_isIsolated(graph, (const void *)(intptr_t)q[1]));
  ASSERT_TRUE(Graph_isIsolated(graph, (const void *)(intptr_t)q[0]));
  Graph_free(graph);
}

TEST(GRAPH, isAdjacent) {
  /* Undirected */
  Graph *graph = NULL;
  ASSERT_FALSE(Graph_isIsolated(graph, (const void *)(intptr_t)q[5]));
  graph = Graph_new();
  ASSERT_NE(graph, nullptr);
  Graph_set_compar(graph, comparIntptr);
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[1]));
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[2]));
  ASSERT_TRUE(GRAPH_SUCCESS == Graph_add_edge(graph,
                                              (const void *)(intptr_t)q[1],
                                              (const void *)(intptr_t)q[2]));
  ASSERT_TRUE(GRAPH_SUCCESS == Graph_add_edge(graph,
                                              (const void *)(intptr_t)q[1],
                                              (const void *)(intptr_t)q[3]));
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[0]));
  ASSERT_FALSE(Graph_isAdjacent(graph, (const void *)(intptr_t)q[5],
                                (const void *)(intptr_t)q[0]));
  ASSERT_FALSE(Graph_isAdjacent(graph, (const void *)(intptr_t)q[3],
                                (const void *)(intptr_t)q[0]));
  ASSERT_FALSE(Graph_isAdjacent(graph, (const void *)(intptr_t)q[2],
                                (const void *)(intptr_t)q[0]));
  ASSERT_FALSE(Graph_isAdjacent(graph, (const void *)(intptr_t)q[1],
                                (const void *)(intptr_t)q[0]));
  ASSERT_FALSE(Graph_isAdjacent(graph, (const void *)(intptr_t)q[0],
                                (const void *)(intptr_t)q[0]));
  ASSERT_TRUE(Graph_isAdjacent(graph, (const void *)(intptr_t)q[1],
                               (const void *)(intptr_t)q[2]));
  ASSERT_TRUE(Graph_isAdjacent(graph, (const void *)(intptr_t)q[2],
                               (const void *)(intptr_t)q[1]));
  ASSERT_TRUE(Graph_isAdjacent(graph, (const void *)(intptr_t)q[1],
                               (const void *)(intptr_t)q[3]));
  ASSERT_TRUE(Graph_isAdjacent(graph, (const void *)(intptr_t)q[3],
                               (const void *)(intptr_t)q[1]));
  Graph_free(graph);

  /* directed */
  graph = Graph_new();
  ASSERT_NE(graph, nullptr);
  Graph_set_compar(graph, comparIntptr);
  Graph_set_directed(graph, true);
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[1]));
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[2]));
  ASSERT_TRUE(GRAPH_SUCCESS == Graph_add_edge(graph,
                                              (const void *)(intptr_t)q[1],
                                              (const void *)(intptr_t)q[2]));
  ASSERT_TRUE(GRAPH_SUCCESS == Graph_add_edge(graph,
                                              (const void *)(intptr_t)q[1],
                                              (const void *)(intptr_t)q[3]));
  ASSERT_TRUE(GRAPH_SUCCESS ==
              Graph_add_vertex(graph, (const void *)(intptr_t)q[0]));
  ASSERT_FALSE(Graph_isAdjacent(graph, (const void *)(intptr_t)q[5],
                                (const void *)(intptr_t)q[0]));
  ASSERT_FALSE(Graph_isAdjacent(graph, (const void *)(intptr_t)q[3],
                                (const void *)(intptr_t)q[0]));
  ASSERT_FALSE(Graph_isAdjacent(graph, (const void *)(intptr_t)q[2],
                                (const void *)(intptr_t)q[0]));
  ASSERT_FALSE(Graph_isAdjacent(graph, (const void *)(intptr_t)q[1],
                                (const void *)(intptr_t)q[0]));
  ASSERT_FALSE(Graph_isAdjacent(graph, (const void *)(intptr_t)q[0],
                                (const void *)(intptr_t)q[0]));
  ASSERT_TRUE(Graph_isAdjacent(graph, (const void *)(intptr_t)q[1],
                               (const void *)(intptr_t)q[2]));
  ASSERT_FALSE(Graph_isAdjacent(graph, (const void *)(intptr_t)q[2],
                                (const void *)(intptr_t)q[1]));
  ASSERT_TRUE(Graph_isAdjacent(graph, (const void *)(intptr_t)q[1],
                               (const void *)(intptr_t)q[3]));
  ASSERT_FALSE(Graph_isAdjacent(graph, (const void *)(intptr_t)q[3],
                                (const void *)(intptr_t)q[1]));
  Graph_free(graph);
}
TEST(GRAPH, undirect) { Graph_undirect(NULL); }

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
