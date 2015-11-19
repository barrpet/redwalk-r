#ifndef REDWALK_PATHS_H
#define REDWALK_PATHS_H

#include "redwalk_types.h"

class AdjacencyArray;

redwalk::MatrixUS shortest_path_lengths_c(const AdjacencyArray& adj);

// [[Rcpp::export(rng = false)]]
redwalk::MatrixUS shortest_path_lengths_c(redwalk::index_t nv,
  const Rcpp::IntegerMatrix& el);

redwalk::MatrixUS shortest_path_lengths_subsets_c(const AdjacencyArray& adj,
  const Rcpp::IntegerVector& s);

// [[Rcpp::export(rng = false)]]
redwalk::MatrixUS shortest_path_lengths_subsets_c(redwalk::index_t nv,
  const Rcpp::IntegerMatrix& el, const Rcpp::IntegerVector& s);

#endif //REDWALK_PATHS_H


