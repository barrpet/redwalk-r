#include "Dissimilarity.h"
#include "Paths.h"
#include "AdjacencyList.h"

Rcpp::NumericVector dissimilarity_sp_c(const AdjacencyList& adj,
  const Eigen::MatrixXd& sp)
{
  //Get number of vertices
  const long nv = adj.vcount();

  //Allocate D2 - what will become a "dist"
  //number of entries in contiguous symmetric matrix with no diagonal
  Rcpp::NumericVector D((nv * nv - nv) >> 1);
  D.fill(0);

  //Do avg shortest paths of neighbors
  //TODO: optimize
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
      D(idx) = (msp1 < msp2) ? msp1 : msp2;
      idx++;
    }
    //let user bail if taking too long
    Rcpp::checkUserInterrupt();
  }

  //Create dist object
  D.attr("Diag") = false;
  D.attr("Upper") = false;
  D.attr("Size") = nv;
  D.attr("class") = "dist";

  return D;
}

Rcpp::NumericVector dissimilarity_sp_c(long nv, const Rcpp::IntegerMatrix& el,
  const Eigen::MatrixXd& sp)
{
  //Call the main function
  return dissimilarity_sp_c(AdjacencyList(nv, el), sp);
}

Rcpp::NumericVector dissimilarity_c(long nv, const Rcpp::IntegerMatrix& el)
{
  //Set up adjacency list
  AdjacencyList adj(nv, el);

  //Call main function
  return dissimilarity_sp_c(adj, all_pairs_shortest_paths_c(adj));
}