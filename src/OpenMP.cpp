#include <Rcpp.h>
#include <cmath>

#include "OpenMP.h"
using namespace Rcpp;

// [[Rcpp::export(rng = false)]]
void check_openmp()
{
#ifdef OMP_V4
  Rcout << "OpenMP v4 ENABLED\n";
  Rcout << "Max Threads: " << omp_get_max_threads() << "\n";
#endif
}

// [[Rcpp::export(rng = false)]]
double dostuff_seq(const NumericVector& x)
{
  const unsigned int s = x.size();
  double* res = (double*)aligned_alloc(16, s * sizeof(double));
  for(unsigned int i = 0; i < s; i++)
  {
    double val = x[i];
    res[i] = std::sqrt(val) + std::cosh(val) / std::pow(val, 1.2352);
  }
  double resd = res[s-1];
  free(res);
  return resd;
}

// [[Rcpp::export(rng = false)]]
double dostuff_par(const NumericVector& x)
{
  const unsigned int s = x.size();
  double* res = (double*)aligned_alloc(16, s * sizeof(double));
  #pragma omp parallel for shared(x)
  for(unsigned int i = 0; i < s; i++)
  {
    double val = x[i];
    res[i] = std::sqrt(val) + std::cosh(val) / std::pow(val, 1.2352);
  }
  double resd = res[s-1];
  free(res);
  return resd;
}