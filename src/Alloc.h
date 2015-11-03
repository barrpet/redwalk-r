#ifndef GCLUST_ALLOC_H
#define GCLUST_ALLOC_H

#include "gclust_types.h"

//TODO: aligned
//TODO: make class

//Allocate memory
template <typename T, typename size_type>
T* gclust_alloc(size_type n)
{
  T* ret = 0;
  try
  {
    ret = new T[n];
  }
  catch(std::exception& e)
  {
    Rcpp::stop(e.what());
  }
  return ret;
}

namespace gclust
{
enum MEMTYPE { SINGLE, ARRAY };
}

//Free memory
template <gclust::MEMTYPE MTYPE>
void gclust_free(void* ptr)
{
  if(ptr != GC_NULLPTR)
  {
    if(MTYPE == gclust::ARRAY) {
      delete [] ptr;
    } else {
      delete ptr;
    }
  }
  return;
}

#endif //GCLUST_ALLOC_H
