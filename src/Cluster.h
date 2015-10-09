#ifndef GCLUST_CLUSTER_H
#define GCLUST_CLUSTER_H

#include <Rcpp.h>
class AdjacencyList;

Rcpp::NumericVector d2_berenhaut_sp_c(const AdjacencyList& adj,
  const Rcpp::NumericMatrix& sp);

// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector d2_berenhaut_sp_c(long nv, const Rcpp::IntegerMatrix& el,
  const Rcpp::NumericMatrix& sp);

// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector d2_berenhaut_c(long nv, const Rcpp::IntegerMatrix& el);

#endif //GCLUST_CLUSTER_H