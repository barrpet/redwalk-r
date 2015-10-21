#ifndef GCLUST_SUBSETS_H
#define GCLUST_SUBSETS_H

#include <RcppEigen.h>
class AdjacencyList;

Rcpp::NumericVector dissimilarity_subsets_sp_c(const AdjacencyList& adj, const
  Rcpp::IntegerVector& s, const Eigen::MatrixXd& sp);

// [[Rcpp::export(rng=false)]]
Rcpp::NumericVector dissimilarity_subsets_sp_c(int nv, const
  Rcpp::IntegerMatrix& el, const Rcpp::IntegerVector& s, const Eigen::MatrixXd&
  sp);

#endif //GCLUST_SUBSETS_H