#ifndef GCLUST_DISSIMILARITY_H
#define GCLUST_DISSIMILARITY_H

#include <RcppEigen.h>
class AdjacencyArray;

Rcpp::NumericVector dissimilarity_sp_c(const AdjacencyArray& adj,
  const Eigen::MatrixXd& sp);

// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dissimilarity_sp_c(long nv, const Rcpp::IntegerMatrix& el,
  const Eigen::MatrixXd& sp);

// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dissimilarity_c(long nv, const Rcpp::IntegerMatrix& el);

Rcpp::NumericVector dissimilarity_subsets_sp_c(const AdjacencyArray& adj, const
  Rcpp::IntegerVector& s, const Eigen::MatrixXd& sp);

// [[Rcpp::export(rng=false)]]
Rcpp::NumericVector dissimilarity_subsets_sp_c(int nv, const
  Rcpp::IntegerMatrix& el, const Rcpp::IntegerVector& s, const Eigen::MatrixXd&
  sp);

// [[Rcpp::export(rng=false)]]
Rcpp::NumericVector dissimilarity_subsets_c(int nv, const
  Rcpp::IntegerMatrix& el, const Rcpp::IntegerVector& s);

#endif //GCLUST_DISSIMILARITY_H