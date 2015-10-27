#include "Paths.h"
#include <vector>
#include <queue>
#include "AdjacencyArray.h"

gclust::MatrixUS shortest_path_lengths_c(const AdjacencyArray& adj)
{
  const int nv = adj.vcount();
  gclust::MatrixUS sp(nv, nv);
  sp.setConstant(gclust::INF_US);

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
      int u = q.front(); q.pop();
      NeighborhoodList nbrs = adj[u];
      NeighborhoodList::iterator itr;
      for(itr = nbrs.cbegin(); itr != nbrs.cend(); itr++)
      {
        int n = *itr;
        if(already_counted[n] == v + 1) { continue; }
        already_counted[n] = v + 1;
        sp(n,v) = sp(u,v) + 1;
        q.push(n);
      }
    }
  }
  return sp;
}

gclust::MatrixUS shortest_path_lengths_c(gclust::index_t nv,
  const Rcpp::IntegerMatrix& el)
{
  return shortest_path_lengths_c(AdjacencyArray(nv, el));
}

gclust::MatrixUS shortest_path_lengths_subsets_c(const AdjacencyArray& adj,
  const Rcpp::IntegerVector& s)
{
  const int nv = adj.vcount();
  const int ns = s.size();
  if(ns == nv)
    return shortest_path_lengths_c(adj);

  std::vector<gclust::vertex_id_t> already_counted(nv, 0);
  std::vector<gclust::vertex_id_t> indexv(nv, 0);
  std::queue<int> q;
  std::queue<int> d;
  gclust::index_t j = 0;
  for(int v = 0; v < ns; v++)
  {
    if(indexv[v] != 0)
      throw Rcpp::exception("Duplicate vertices in s");
    indexv[v] = ++j;
  }
  gclust::MatrixUS sp(ns, ns);
  sp.setConstant(gclust::INF_US);
  for(int vi = 0; vi < ns; vi++)
  {
    Rcpp::checkUserInterrupt();
    int v = s[vi];
    sp(v,v) = 0;
    gclust::vertex_id_t reached = 0;
    q.push(v);
    d.push(0);
    already_counted[v] = vi + 1;
    while(!q.empty())
    {
      int u = q.front(); q.pop();
      int ud = d.front(); d.pop();
      if(indexv[u] != 0)
      {
        sp(vi, indexv[u]-1) = ud;
        reached++;
        if(reached == ns)
        {
          while(!q.empty()) { q.pop(); }
          while(!d.empty()) { d.pop(); }
          break;
        }
      }
      NeighborhoodList nbrs = adj[u];
      NeighborhoodList::iterator nitr;
      for(nitr = nbrs.begin(); nitr != nbrs.end(); nitr++)
      {
        int n = *nitr;
        if(already_counted[n] == vi + 1) { continue; }
        already_counted[n] = vi + 1;
        q.push(n);
        d.push(ud+1);
      }
    }
  }
  return sp;
}

gclust::MatrixUS shortest_path_lengths_subsets_c(gclust::index_t nv,
  const Rcpp::IntegerMatrix& el, const Rcpp::IntegerVector& s)
{
  return shortest_path_lengths_subsets_c(AdjacencyArray(nv, el), s);
}
