#ifndef GCLUST_BASE_H
#define GCLUST_BASE_H

#include <cstddef>
#include <limits>
#include <RcppEigen.h>
#include "OpenMP.h"

#if __cplusplus >= 201103L
#define _GC_CXX11 1
#endif

#ifdef _GC_CXX11
#  define GC_NULLPTR nullptr
#  define GC_CONSTEXPR constexpr
#  define GC_CONSTEXPR_OR_CONST constexpr
#else
#  ifdef NULL
#    define GC_NULLPTR NULL
#  else
#    define GC_NULLPTR 0
#  endif
#  define GC_CONSTEXPR
#  define GC_CONSTEXPR_OR_CONST const
#endif

namespace gclust
{
  typedef double float_t;
  typedef unsigned long int index_t;
  typedef unsigned int vertex_id_t;
  typedef unsigned short ushort_t;
  typedef unsigned char uchar_t;

  typedef float_t flt_t;
  typedef index_t idx_t;
  typedef vertex_id_t vid_t;

  typedef Eigen::Matrix<float_t, Eigen::Dynamic, Eigen::Dynamic> MatrixF;
  typedef Eigen::Matrix<ushort_t, Eigen::Dynamic, Eigen::Dynamic> MatrixUS;

  static GC_CONSTEXPR_OR_CONST vid_t NONE = std::numeric_limits<vid_t>::max();
  static GC_CONSTEXPR_OR_CONST ushort_t INF_US = std::numeric_limits<ushort_t>::max();

  static GC_CONSTEXPR_OR_CONST flt_t INF_F = std::numeric_limits<flt_t>::infinity();
  static GC_CONSTEXPR_OR_CONST flt_t NAN_F = std::numeric_limits<flt_t>::quiet_NaN();
  static GC_CONSTEXPR_OR_CONST flt_t ZERO_F = flt_t(0);
  static GC_CONSTEXPR_OR_CONST flt_t ONE_F = flt_t(1);
  static GC_CONSTEXPR_OR_CONST flt_t NEG_ONE_F = flt_t(-1);
  static GC_CONSTEXPR_OR_CONST flt_t HALF_F = flt_t(0.5);
};

#endif //GCLUST_BASE_H