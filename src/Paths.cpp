#include "Paths.h"
#include <vector>
#include <queue>
#include "AdjacencyArray.h"

redwalk::MatrixUS shortest_path_lengths_c(const AdjacencyArray& adj)
{
  const redwalk::idx_t nv = adj.vcount();
  redwalk::MatrixUS sp(nv, nv);
  sp.setConstant(redwalk::INF_US);

  std::vector<redwalk::vid_t> already_counted(nv, 0);
  std::queue<redwalk::vid_t> q;
  for(redwalk::vid_t v = 0; v < nv; v++)
  {
    Rcpp::checkUserInterrupt();
    already_counted[v] = v + 1;
    sp(v,v) = 0;
    q.push(v);
    while(!q.empty())
    {
      redwalk::vid_t u = q.front(); q.pop();
      NeighborhoodList nbrs = adj[u];
      NeighborhoodList::iterator itr;
      for(itr = nbrs.cbegin(); itr != nbrs.cend(); itr++)
      {
        redwalk::vid_t n = *itr;
        if(already_counted[n] == v + 1) { continue; }
        already_counted[n] = v + 1;
        sp(n,v) = sp(u,v) + 1;
        q.push(n);
      }
    }
  }
  return sp;
}

redwalk::MatrixUS shortest_path_lengths_c(redwalk::idx_t nv,
  const Rcpp::IntegerMatrix& el)
{
  return shortest_path_lengths_c(AdjacencyArray(nv, el));
}

redwalk::MatrixUS shortest_path_lengths_subsets_c(const AdjacencyArray& adj,
  const Rcpp::IntegerVector& s)
{
  const redwalk::idx_t nv = adj.vcount();
  const redwalk::idx_t ns = s.size();
  if(ns == nv)
    return shortest_path_lengths_c(adj);

  std::vector<redwalk::vid_t> already_counted(nv, 0);
  std::vector<redwalk::vid_t> indexv(nv, 0);
  std::queue<redwalk::vid_t> q;
  std::queue<redwalk::ushort_t> d;
  redwalk::idx_t j = 0;
  for(redwalk::idx_t vi = 0; vi < ns; vi++)
  {
    redwalk::vid_t v = s[vi];
    if(indexv[v] != 0)
      throw Rcpp::exception("Duplicate vertices in s");
    indexv[v] = ++j;
  }
  redwalk::MatrixUS sp(ns, ns);
  sp.setConstant(redwalk::INF_US);
  for(int vi = 0; vi < ns; vi++)
  {
    Rcpp::checkUserInterrupt();
    int v = s[vi];
    sp(vi,vi) = 0;
    redwalk::vertex_id_t reached = 0;
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
          #ifdef _GC_CXX11
            std::queue<redwalk::vid_t>().swap(q);
            std::queue<redwalk::ushort_t>().swap(d);
          #else
            while(!q.empty()) { q.pop(); }
            while(!d.empty()) { d.pop(); }
          #endif
          break;
        }
      }
      NeighborhoodList nbrs = adj[u];
      NeighborhoodList::iterator nitr;
      for(nitr = nbrs.begin(); nitr != nbrs.end(); nitr++)
      {
        redwalk::vid_t n = *nitr;
        if(already_counted[n] == vi + 1) { continue; }
        already_counted[n] = vi + 1;
        q.push(n);
        d.push(ud+1);
      }
    }
  }
  return sp;
}

redwalk::MatrixUS shortest_path_lengths_subsets_c(redwalk::index_t nv,
  const Rcpp::IntegerMatrix& el, const Rcpp::IntegerVector& s)
{
  return shortest_path_lengths_subsets_c(AdjacencyArray(nv, el), s);
}
