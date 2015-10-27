#ifndef GCLUST_PATHS_H
#define GCLUST_PATHS_H

#include "gclust_types.h"

class AdjacencyArray;

gclust::MatrixUS shortest_path_lengths_c(const AdjacencyArray& adj);

// [[Rcpp::export(rng = false)]]
gclust::MatrixUS shortest_path_lengths_c(gclust::index_t nv,
  const Rcpp::IntegerMatrix& el);

gclust::MatrixUS shortest_path_lengths_subsets_c(const AdjacencyArray& adj,
  const Rcpp::IntegerVector& s);

// [[Rcpp::export(rng = false)]]
gclust::MatrixUS shortest_path_lengths_subsets_c(gclust::index_t nv,
  const Rcpp::IntegerMatrix& el, const Rcpp::IntegerVector& s);

#endif //GCLUST_PATHS_H


