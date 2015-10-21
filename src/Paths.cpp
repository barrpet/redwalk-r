#include "Paths.h"
#include <vector>
#include <queue>
#include "AdjacencyList.h"

Eigen::MatrixXd all_pairs_shortest_paths_c(const AdjacencyList& adj)
{
  const auto nv = adj.vcount();
  Eigen::MatrixXd sp(nv, nv);
  sp.fill(0);

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

Eigen::MatrixXd all_pairs_shortest_paths_c(gclust::index_t nv,
  const Rcpp::IntegerMatrix& el)
{
  return all_pairs_shortest_paths_c(AdjacencyList(nv, el));
}

Eigen::MatrixXd subsets_shortest_paths_c(const AdjacencyList& adj,
  const Rcpp::IntegerVector& s)
{
  const auto nv = adj.vcount();
  const auto ns = s.size();
  if(ns == nv)
    return all_pairs_shortest_paths_c(adj);

  std::vector<gclust::vertex_id_t> already_counted(nv, 0);
  std::vector<gclust::vertex_id_t> indexv(nv, 0);
  std::queue<gclust::vertex_id_t> q;
  std::queue<int> d;
  gclust::index_t j = 0;
  for(auto v : s)
  {
    if(indexv[v] != 0)
      throw Rcpp::exception("Duplicate vertices in s");
    indexv[v] = ++j;
  }
  Eigen::MatrixXd sp(ns, ns);
  sp.fill(0);
  gclust::index_t i = 0;
  for(auto v : s)
  {
    Rcpp::checkUserInterrupt();
    gclust::vertex_id_t reached = 0;
    q.push(v);
    d.push(0);
    already_counted[v] = i + 1;
    while(!q.empty())
    {
      auto u = q.front(); q.pop();
      auto ud = d.front(); d.pop();
      if(indexv[u] != 0)
      {
        sp(i, indexv[u]-1) = ud;
        reached++;
        if(reached == ns)
        {
          std::queue<gclust::vertex_id_t>().swap(q);
          std::queue<int>().swap(d);
          break;
        }
      }
      for(auto n : adj[u])
      {
        if(already_counted[n] == i + 1) { continue; }
        already_counted[n] = i + 1;
        q.push(n);
        d.push(ud+1);
      }
    }
    i++;
  }
  return sp;
}

Eigen::MatrixXd subsets_shortest_paths_c(gclust::index_t nv,
  const Rcpp::IntegerMatrix& el, const Rcpp::IntegerVector& s)
{
  return subsets_shortest_paths_c(AdjacencyList(nv, el), s);
}
