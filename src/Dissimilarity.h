#ifndef REDWALK_DISSIMILARITY_H
#define REDWALK_DISSIMILARITY_H

#include "redwalk_types.h"

// [[Rcpp::depends(RcppEigen)]]

class AdjacencyArray;

Rcpp::NumericVector dissimilarity_sp_c(const AdjacencyArray& adj,
  const redwalk::MatrixUS& sp);

// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dissimilarity_sp_c(const redwalk::idx_t nv,
  const Rcpp::IntegerMatrix& el, const redwalk::MatrixUS& sp);

// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dissimilarity_c(const redwalk::idx_t nv,
  const Rcpp::IntegerMatrix& el);

Rcpp::NumericVector dissimilarity_subsets_sp_c(const AdjacencyArray& adj, const
  Rcpp::IntegerVector& s, const redwalk::MatrixUS& sp);

// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dissimilarity_subsets_sp_c(const redwalk::idx_t nv,
  const Rcpp::IntegerMatrix& el, const Rcpp::IntegerVector& s,
  const redwalk::MatrixUS& sp);

// [[Rcpp::export(rng = false)]]
Rcpp::NumericVector dissimilarity_subsets_c(const redwalk::idx_t nv,
  const Rcpp::IntegerMatrix& el, const Rcpp::IntegerVector& s);

#endif //REDWALK_DISSIMILARITY_H