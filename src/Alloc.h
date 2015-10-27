#ifndef GCLUST_ALLOC_H
#define GCLUST_ALLOC_H

#include "gclust_types.h"

//TODO: aligned

template <typename T, typename size_type>
T* gclust_alloc(size_type n)
{
  T* ret = 0;
  try
  {
    #ifdef _GC_CXX11
    ret = (T*)aligned_alloc(32, n*sizeof(*ret));
    #else
    ret = new T[n];
    #endif
  }
  catch(std::exception& e)
  {
    Rcpp::stop(e.what());
  }
  return ret;
}

#endif //GCLUST_ALLOC_H
