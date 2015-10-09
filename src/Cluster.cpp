#include "Cluster.h"
#include <vector>
#include <queue>
#include "AdjacencyList.h"

Rcpp::NumericVector d2_berenhaut_sp_c(const AdjacencyList& adj,
  const Rcpp::NumericMatrix& sp)
{
  //Get number of vertices
  long nv = adj.vcount();

  //Allocate D2 - what will become a "dist"
  //number of entries in contiguous symmetric matrix with no diagonal
  long ns = (nv * nv - nv) / 2;
  Rcpp::NumericVector D2(ns);

  //Do avg shortest paths of neighbors
  //this can be optimized
  long idx = 0;
  double msp1, msp2;
  for(long i = 0; i < (nv-1); i++)
  {
    for(long j = (i+1); j < nv; j++)
    {
      msp1 = msp2 = 0;
      for(long n : adj[i])
        msp1 += sp(n,j);
      msp1 /= adj.degree(i);
      for(long n : adj[j])
        msp2 += sp(n,i);
      msp2 /= adj.degree(j);
      D2(idx) = (msp1 < msp2) ? msp1 : msp2;
      idx++;
    }
    //let user bail if taking too long
    Rcpp::checkUserInterrupt();
  }

  /*
  D2.attr("Size") = nv;
  D2.attr("call") = Rcpp::NA;
  D2.attr("Diag") = false;
  D2.attr("Upper") = false;
  D2.attr("class") = "dist";
  */

  //Return the numeric vector
  return D2;
}

Rcpp::NumericVector d2_berenhaut_sp_c(long nv, const Rcpp::IntegerMatrix& el,
  const Rcpp::NumericMatrix& sp)
{
  //set up adjacency list
  AdjacencyList adj(nv, el);

  //Call the main function
  return d2_berenhaut_sp_c(adj, sp);
}

Rcpp::NumericVector d2_berenhaut_c(long nv, const Rcpp::IntegerMatrix& el)
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