#include "Cluster.h"
#include <vector>
#include <queue>
#include "AdjacencyList.h"

Rcpp::NumericMatrix d2_berenhaut_sp_c(const AdjacencyList& adj,
  const Rcpp::NumericMatrix& sp)
{
  //Get number of vertices
  long nv = adj.vcount();

  //Allocate D2
  Rcpp::NumericMatrix D2(nv, nv);

  //Do avg shortest paths of neighbors
  for(long i = 0; i < nv; i++)
  {
    for(long j = 0; j < nv; j++)
    {
      if(i != j)
        for(long n : adj[i])
          D2(i,j) += sp(n,j);
    }
    Rcpp::checkUserInterrupt();
    //TODO: column or row major storage for faster division
    gclust::float_t deg = adj.degree(i);
    D2.row(i) = D2.row(i) / deg;
  }

  //Minimize (make symmetric with min)
  //Set D2_ij to min(D2_ij, D2_ji)
  for(long i = 0; i < nv; i++)
  {
    for(long j = i+1; j < nv; j++)
    {
      double dist1 = D2(i,j);
      double dist2 = D2(j,i);
      if(dist1 <= dist2)
        D2(j,i) = dist1;
      else
        D2(i,j) = dist2;
    }
    Rcpp::checkUserInterrupt();
  }

  //Return the symmetric D2
  return D2;
}

Rcpp::NumericMatrix d2_berenhaut_sp_c(long nv, const Rcpp::IntegerMatrix& el,
  const Rcpp::NumericMatrix& sp)
{
  //set up adjacency list
  AdjacencyList adj(nv, el);

  //Call the main function
  return d2_berenhaut_sp_c(adj, sp);
}

Rcpp::NumericMatrix d2_berenhaut_c(long nv, const Rcpp::IntegerMatrix& el)
{
  //Set up adjacency list
  AdjacencyList adj(nv, el);

  //Set up shortest paths matrix
  Rcpp::NumericMatrix sp(nv, nv);

  //Find all pairs shortest paths
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

  //Call main function
  return d2_berenhaut_sp_c(adj, sp);
}