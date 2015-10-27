#include "Dissimilarity.h"
#include "Paths.h"
#include "AdjacencyArray.h"

Rcpp::NumericVector dissimilarity_sp_c(const AdjacencyArray& adj,
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
  double mspi, mspj;
  int degi, degj;
  for(long i = 0; i < (nv-1); i++)
  {
    NeighborhoodList nbrsi = adj[i];
    degi = nbrsi.size();
    for(long j = (i+1); j < nv; j++)
    {
      mspi = 0; mspj = 0;
      NeighborhoodList nbrsj = adj[j];
      degj = nbrsj.size();
      for(int n = 0; n < degi; n++)
        mspi += sp(nbrsi[n],j);
      mspi /= degi;
      for(int n = 0; n < degj; n++)
        mspj += sp(nbrsj[n],i);
      mspj /= degj;
      D(idx) = (mspi < mspj) ? mspi : mspj;
      idx++;
    }
    //let user quit if taking too long
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
  return dissimilarity_sp_c(AdjacencyArray(nv, el), sp);
}

Rcpp::NumericVector dissimilarity_c(long nv, const Rcpp::IntegerMatrix& el)
{
  //Set up adjacency array
  AdjacencyArray adj(nv, el);

  //Call main function
  return dissimilarity_sp_c(adj, all_pairs_shortest_paths_c(adj));
}

Rcpp::NumericVector dissimilarity_subsets_sp_c(const AdjacencyArray& adj, const
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
      NeighborhoodList nbrs = adj[i];
      NeighborhoodList::iterator nitr;
      for(nitr = nbrs.begin(); nitr != nbrs.end(); nitr++)
        tripletList.push_back(T(i,*nitr,val));
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
  double mspi, mspj;
  int degi, degj;
  for(int i = 0; i < (ns-1); i++)
  {
    int ti = s[i];
    degi = adj.degree(ti);
    NeighborhoodList nbrsi = adj[ti];
    for(int j = i+1; j < ns; j++)
    {
      int tj = s[j];
      degj = adj.degree(tj);
      NeighborhoodList nbrsj = adj[tj];
      mspi = mspj = 0;
      for(int n = 0; n < degi; n++)
        mspi += x(nbrsi[n],j);
      mspi /= degi;
      for(int n = 0; n < degj; n++)
        mspj += x(nbrsj[n],i);
      mspj /= degj;
      D(idx) = (mspi < mspj) ? mspi : mspj;
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
  AdjacencyArray adj(nv, el);
  return dissimilarity_subsets_sp_c(adj, s, sp);
}

Rcpp::NumericVector dissimilarity_subsets_c(int nv, const
  Rcpp::IntegerMatrix& el, const Rcpp::IntegerVector& s)
{
  AdjacencyArray adj(nv, el);
  return dissimilarity_subsets_sp_c(adj, s, subsets_shortest_paths_c(adj, s));
}