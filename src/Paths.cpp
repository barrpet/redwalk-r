#include "Paths.h"
#include <vector>
#include <queue>
#include "AdjacencyList.h"

Rcpp::NumericMatrix all_shortest_paths_c(const AdjacencyList& adj)
{
  const auto nv = adj.vcount();
  Rcpp::NumericMatrix sp(nv, nv);
  std::vector<gclust::vertex_id_t> already_counted(nv, 0);
  std::queue<gclust::vertex_id_t> q;
  for(gclust::vertex_id_t v = 0; v < nv; v++)
  {
    Rcpp::checkUserInterrupt();
    already_counted[v] = v + 1;
    sp(v,v) = 0;
    q.push(v);
    while(!q.empty())
    {
      auto u = q.front(); q.pop();
      for(auto n : adj[u])
      {
        if(already_counted[n] == v + 1) { continue; }
        already_counted[n] = v + 1;
        sp(v,n) = sp(v,u) + 1;
        q.push(n);
      }
    }
  }
    return sp;
}

Rcpp::NumericMatrix all_shortest_paths_c(gclust::index_t nv,
  const Rcpp::IntegerMatrix& el)
{
  AdjacencyList adj(nv, el);
  return all_shortest_paths_c(adj);
}
