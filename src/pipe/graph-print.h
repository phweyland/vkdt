#pragma once
#include "pipe/graph.h"

// this is not thread safe. but who would ever debug print
// node graphs from multiple threads, right?
// you can call it up to 4 times in the same line of code. luxury.
static inline const char*
_pr(dt_token_t t)
{
  static int cur = 0;
  static char tmp[4][20];
  cur = (cur+1)&0x3;
  memcpy(tmp[cur], &t, sizeof(dt_token_t));
  tmp[cur][9] = 0;
  for(int k=0;k<8;k++)
    if(tmp[cur][k] == '-') tmp[cur][k] = '_';
  return tmp[cur];
}

static inline void
dt_graph_print_modules(
    const dt_graph_t *graph)
{
  fprintf(stdout, "digraph modules {\nnode [shape=record]\n");
  // for all modules, print all incoming edges (outgoing don't have module ids)
  for(int m=0;m<graph->num_modules;m++)
  {
    fprintf(stdout, "n%d_%s [label=\"%s|{",
        m,
        _pr(graph->module[m].name),
        _pr(graph->module[m].name));
    for(int c=0;c<graph->module[m].num_connectors;c++)
    {
      fprintf(stdout, "<%d> %s", c, _pr(graph->module[m].connector[c].name));
      if(c != graph->module[m].num_connectors - 1)
        fprintf(stdout, "|");
      else fprintf(stdout, "}\"];\n");
    }
  }
  for(int m=0;m<graph->num_modules;m++)
  {
    for(int c=0;c<graph->module[m].num_connectors;c++)
    {
      if((graph->module[m].connector[c].type == dt_token("read") ||
          graph->module[m].connector[c].type == dt_token("sink")) &&
          graph->module[m].connector[c].connected_mi >= 0)
      {
        fprintf(stdout, "n%d_%s:%d -> n%d_%s:%d",
            graph->module[m].connector[c].connected_mi,
            _pr(graph->module[
              graph->module[m].connector[c].connected_mi
            ].name),
            graph->module[m].connector[c].connected_mc,
            m,
            _pr(graph->module[m].name),
            c);
        if(graph->module[m].connector[c].flags & s_conn_feedback)
          fprintf(stdout, "[style=dashed]\n");
        else fprintf(stdout, "\n");
      }
    }
  }
  fprintf(stdout, "}\n");
}

static inline void
dt_graph_print_nodes(
    const dt_graph_t *graph)
{
  fprintf(stdout, "digraph nodes {\nnode [shape=record]\n");
  // for all nodes, print all incoming edges (outgoing don't have module ids)
  for(int m=0;m<graph->num_nodes;m++)
  {
    fprintf(stdout, "n%d_%s_%s"
        " [label=\"%s_%s|{",
        m,
        _pr(graph->node[m].name),
        _pr(graph->node[m].kernel),
        _pr(graph->node[m].name),
        _pr(graph->node[m].kernel));
    for(int c=0;c<graph->node[m].num_connectors;c++)
    {
      fprintf(stdout, "<%d> %s", c, _pr(graph->node[m].connector[c].name));
      if(c != graph->node[m].num_connectors - 1)
        fprintf(stdout, "|");
      else fprintf(stdout, "}\"];\n");
    }
  }
  for(int m=0;m<graph->num_nodes;m++)
  {
    for(int c=0;c<graph->node[m].num_connectors;c++)
    {
      if((graph->node[m].connector[c].type == dt_token("read") ||
          graph->node[m].connector[c].type == dt_token("sink")) &&
          graph->node[m].connector[c].connected_mi >= 0)
      {
        fprintf(stdout, "n%d_%s_%s:%d -> n%d_%s_%s:%d",
            graph->node[m].connector[c].connected_mi,
            _pr(graph->node[
              graph->node[m].connector[c].connected_mi
            ].name),
            _pr(graph->node[
              graph->node[m].connector[c].connected_mi
            ].kernel),
            graph->node[m].connector[c].connected_mc,
            m,
            _pr(graph->node[m].name),
            _pr(graph->node[m].kernel),
            c);
        if(graph->node[m].connector[c].array_length > 1)
          fprintf(stdout, "[style=bold]\n");
        else if(graph->node[m].connector[c].flags & s_conn_feedback)
          fprintf(stdout, "[style=dashed]\n");
        else
          fprintf(stdout, "\n");
      }
    }
  }
  fprintf(stdout, "}\n");
}
