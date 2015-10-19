#ifndef GCLUST_PATHS_H
#define GCLUST_PATHS_H

#include <Rcpp.h>
#include "gclust_types.h"
class AdjacencyList;

Rcpp::NumericMatrix all_pairs_shortest_paths_c(const AdjacencyList& adj);

// [[Rcpp::export(rng = false)]]
Rcpp::NumericMatrix all_pairs_shortest_paths_c(gclust::index_t nv,
  const Rcpp::IntegerMatrix& el);

Rcpp::NumericMatrix subsets_shortest_paths_c(const AdjacencyList& adj,
  const Rcpp::IntegerVector& s);

// [[Rcpp::export(rng = false)]]
Rcpp::NumericMatrix subsets_shortest_paths_c(gclust::index_t nv,
  const Rcpp::IntegerMatrix& el, const Rcpp::IntegerVector& s);

#endif //GCLUST_PATHS_H


