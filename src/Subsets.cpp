#include "Subsets.h"
#include <vector>
#include "AdjacencyList.h"

Rcpp::NumericVector dissimilarity_subsets_sp_c(const AdjacencyList& adj, const
  Rcpp::IntegerVector& s, const Eigen::MatrixXd& sp)
{
  const int nv = adj.vcount();
  const int ns = s.size();

  //to check if a vertex is a target
  std::vector<bool> is_targ(nv, false);
  for(int i = 0; i < ns; i++)
    is_targ[s[i]] = true;

  //Set up triplet list for sparse matrix
  typedef Eigen::Triplet<double> T;
  std::vector<T> tripletList;
  tripletList.reserve(nv + 2 * adj.ecount()); //TODO: reserve exact or close to

  //Build the tripletlist
  for(int i = 0; i < nv; i++)
  {
    tripletList.push_back(T(i, i, 1.0));
    if(!is_targ[i])
    {
      double val = -1.0 / (double)adj.degree(i);
      for(auto n : adj[i])
        tripletList.push_back(T(i,n,val));
    }
  }

  //set up the random walk normalized laplacian
  Eigen::SparseMatrix<double> L(nv, nv);
  L.setFromTriplets(tripletList.begin(), tripletList.end());
  L.makeCompressed();

  //free the triplets
  std::vector<T>().swap(tripletList);

  //factor the RWNL
  Eigen::SparseLU<Eigen::SparseMatrix<double, Eigen::ColMajor>,
                  Eigen::COLAMDOrdering<int> > solver;
  solver.analyzePattern(L);
  solver.factorize(L);

  //setup b
  Eigen::MatrixXd b(ns, nv);
  b.fill(0);  //TODO: may not be necessary
  for(int i = 0; i < ns; i++)
    b.col(s[i]) = sp.col(i);
  b.transposeInPlace();

  //Solve
  const Eigen::MatrixXd x = solver.solve(b);

  //Free b
  b.resize(0,0);

  //Allocate D - what will become a "dist"
  //number of entries in contiguous symmetric matrix with no diagonal
  Rcpp::NumericVector D((ns * ns - ns) >> 1);

  int idx = 0;
  double msp1, msp2;
  for(int i = 0; i < (ns-1); i++)
  {
    int ti = s[i];
    for(int j = i+1; j < ns; j++)
    {
      msp1 = msp2 = 0;
      int tj = s[j];
      for(long n : adj[ti])
        msp1 += x(n,j);
      msp1 /= adj.degree(ti);
      for(long n : adj[tj])
        msp2 += x(n,i);
      msp2 /= adj.degree(tj);
      D(idx) = (msp1 < msp2) ? msp1 : msp2;
      idx++;
    }
    Rcpp::checkUserInterrupt();
  }

  //Create dist object
  D.attr("Diag") = false;
  D.attr("Upper") = false;
  D.attr("Size") = ns;
  D.attr("class") = "dist";

  return D;
}

Rcpp::NumericVector dissimilarity_subsets_sp_c(int nv, const
  Rcpp::IntegerMatrix& el, const Rcpp::IntegerVector& s, const Eigen::MatrixXd&
  sp)
{
  AdjacencyList adj(nv, el);
  return dissimilarity_subsets_sp_c(adj, s, sp);
}