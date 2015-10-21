#ifndef GCLUST_DISSIMILARITY_H
#define GCLUST_DISSIMILARITY_H

#include <RcppEigen.h>
class AdjacencyList;

Rcpp::NumericVector dissimilarity_sp_c(const AdjacencyList& adj,
  const Eigen::MatrixXd& sp);

// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dissimilarity_sp_c(long nv, const Rcpp::IntegerMatrix& el,
  const Eigen::MatrixXd& sp);

// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dissimilarity_c(long nv, const Rcpp::IntegerMatrix& el);

#endif //GCLUST_DISSIMILARITY_H