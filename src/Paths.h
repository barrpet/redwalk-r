#ifndef GCLUST_PATHS_H
#define GCLUST_PATHS_H

#include <RcppEigen.h>
#include "gclust_types.h"
class AdjacencyList;

Eigen::MatrixXd all_pairs_shortest_paths_c(const AdjacencyList& adj);

// [[Rcpp::export(rng = false)]]
Eigen::MatrixXd all_pairs_shortest_paths_c(gclust::index_t nv,
  const Rcpp::IntegerMatrix& el);

Eigen::MatrixXd subsets_shortest_paths_c(const AdjacencyList& adj,
  const Rcpp::IntegerVector& s);

// [[Rcpp::export(rng = false)]]
Eigen::MatrixXd subsets_shortest_paths_c(gclust::index_t nv,
  const Rcpp::IntegerMatrix& el, const Rcpp::IntegerVector& s);

#endif //GCLUST_PATHS_H


