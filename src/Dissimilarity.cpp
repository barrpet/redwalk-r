#include "Dissimilarity.h"
#include "Paths.h"
#include "AdjacencyArray.h"

Rcpp::NumericVector dissimilarity_sp_c(const AdjacencyArray& adj,
  const gclust::MatrixUS& sp)
{
  //Get number of vertices
  const gclust::idx_t nv = adj.vcount();

  //Allocate D2 - what will become a "dist"
  //number of entries in contiguous symmetric matrix with no diagonal
  Rcpp::NumericVector D((nv * nv - nv) >> 1);

  //Do avg shortest paths of neighbors
  //TODO: optimize
  gclust::idx_t idx = 0;
  gclust::flt_t mspi, mspj;
  for(gclust::idx_t i = 0; i < (nv-1); i++)
  {
    NeighborhoodList nbrsi = adj[i];
    gclust::idx_t degi = nbrsi.size();
    for(gclust::idx_t j = (i+1); j < nv; j++)
    {
      mspi = 0; mspj = 0;
      NeighborhoodList nbrsj = adj[j];
      gclust::idx_t degj = nbrsj.size();
      for(gclust::idx_t n = 0; n < degi; n++)
        mspi += sp(nbrsi[n],j);
      mspi /= degi;
      for(gclust::idx_t n = 0; n < degj; n++)
        mspj += sp(nbrsj[n],i);
      mspj /= degj;
      D[idx] = (mspi < mspj) ? mspi : mspj;
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

Rcpp::NumericVector dissimilarity_sp_c(const gclust::idx_t nv,
  const Rcpp::IntegerMatrix& el, const gclust::MatrixUS& sp)
{
  //Call the main function
  return dissimilarity_sp_c(AdjacencyArray(nv, el), sp);
}

Rcpp::NumericVector dissimilarity_c(const gclust::idx_t nv,
  const Rcpp::IntegerMatrix& el)
{
  //Set up adjacency array
  AdjacencyArray adj(nv, el);

  //Call main function
  return dissimilarity_sp_c(adj, shortest_path_lengths_c(adj));
}

Rcpp::NumericVector dissimilarity_subsets_sp_c(const AdjacencyArray& adj,
  const Rcpp::IntegerVector& s, const gclust::MatrixUS& sp)
{
  const gclust::idx_t nv = adj.vcount();
  const gclust::idx_t ns = s.size();

  //to check if a vertex is a target
  std::vector<bool> is_targ(nv, false);
  for(gclust::idx_t i = 0; i < ns; i++)
    is_targ[s[i]] = true;

  //Set up triplet list for sparse matrix
  typedef Eigen::Triplet<gclust::flt_t> Triplet;
  std::vector<Triplet> tripletList;
  tripletList.reserve(nv + 2 * adj.ecount()); //TODO: reserve exact or close to

  //Build the tripletlist
  for(gclust::idx_t i = 0; i < nv; i++)
  {
    tripletList.push_back(Triplet(i, i, gclust::ONE_F));
    if(!is_targ[i])
    {
      gclust::flt_t val = gclust::NEG_ONE_F / (gclust::flt_t)adj.degree(i);
      NeighborhoodList nbrs = adj[i];
      NeighborhoodList::iterator nitr;
      for(nitr = nbrs.begin(); nitr != nbrs.end(); nitr++)
        tripletList.push_back(Triplet(i,*nitr,val));
    }
  }

  //set up the random walk normalized laplacian
  Eigen::SparseMatrix<gclust::flt_t> L(nv, nv);
  L.setFromTriplets(tripletList.begin(), tripletList.end());
  L.makeCompressed();

  //free the triplets
  std::vector<Triplet>().swap(tripletList);

  //factor the RWNL
  Eigen::SparseLU<Eigen::SparseMatrix<gclust::flt_t, Eigen::ColMajor>,
                  Eigen::COLAMDOrdering<int> > solver;
  solver.analyzePattern(L);
  solver.factorize(L);

  //setup b
  gclust::MatrixF b(nv, ns);
  b.fill(0);
  for(gclust::idx_t i = 0; i < ns; i++)
    b.row(s[i]) = sp.col(i).cast<gclust::flt_t>();

  //Solve
  const gclust::MatrixF x = solver.solve(b);

  //Free b
  b.resize(0,0);

  //Free L
  Eigen::SparseMatrix<gclust::flt_t, Eigen::ColMajor>().swap(L);

  //Allocate D - what will become a "dist"
  //number of entries in contiguous symmetric matrix with no diagonal
  Rcpp::NumericVector D((ns * ns - ns) >> 1);

  gclust::idx_t idx = 0;
  gclust::flt_t mspi, mspj;
  for(gclust::idx_t i = 0; i < (ns-1); i++)
  {
    gclust::vid_t ti = s[i];
    gclust::idx_t degi = adj.degree(ti);
    NeighborhoodList nbrsi = adj[ti];
    for(gclust::idx_t j = i+1; j < ns; j++)
    {
      gclust::vid_t tj = s[j];
      gclust::idx_t degj = adj.degree(tj);
      NeighborhoodList nbrsj = adj[tj];
      mspi = 0; mspj = 0;
      for(gclust::idx_t n = 0; n < degi; n++)
        mspi += x(nbrsi[n],j);
      mspi /= degi;
      for(gclust::idx_t n = 0; n < degj; n++)
        mspj += x(nbrsj[n],i);
      mspj /= degj;
      D[idx] = (mspi < mspj) ? mspi : mspj;
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

Rcpp::NumericVector dissimilarity_subsets_sp_c(const gclust::idx_t nv, const
  Rcpp::IntegerMatrix& el, const Rcpp::IntegerVector& s, const gclust::MatrixUS&
  sp)
{
  AdjacencyArray adj(nv, el);
  return dissimilarity_subsets_sp_c(adj, s, sp);
}

Rcpp::NumericVector dissimilarity_subsets_c(const gclust::idx_t nv, const
  Rcpp::IntegerMatrix& el, const Rcpp::IntegerVector& s)
{
  AdjacencyArray adj(nv, el);
  return dissimilarity_subsets_sp_c(adj, s, shortest_path_lengths_subsets_c(adj, s));
}