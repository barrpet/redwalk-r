#include "Dissimilarity.h"
#include "Paths.h"
#include "AdjacencyList.h"

#ifdef _OPENMP
#include <omp.h>
#else
typedef int omp_int_t;
inline omp_int_t omp_get_thread_num()  { return 0; }
inline omp_int_t omp_get_num_threads() { return 1; }
inline omp_int_t omp_get_max_threads() { return 1; }
inline omp_int_t omp_in_parallel()     { return false; }
#endif


Rcpp::NumericVector dissimilarity_sp_c(const AdjacencyList& adj,
  const Rcpp::NumericMatrix& sp)
{
  //Get number of vertices
  long nv = adj.vcount();

  //Allocate D2 - what will become a "dist"
  //number of entries in contiguous symmetric matrix with no diagonal
  Rcpp::NumericVector D((nv * nv - nv) >> 1);

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
  return D;
}

Rcpp::NumericVector dissimilarity_sp_c(long nv, const Rcpp::IntegerMatrix& el,
  const Rcpp::NumericMatrix& sp)
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