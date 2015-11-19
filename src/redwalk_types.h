#ifndef REDWALK_BASE_H
#define REDWALK_BASE_H

#include <cstddef>
#include <limits>
#include <RcppEigen.h>
#include "OpenMP.h"

#if __cplusplus >= 201103L
#define _RW_CXX11 1
#endif

#ifdef _RW_CXX11
#  define RW_NULLPTR nullptr
#  define RW_CONSTEXPR constexpr
#  define RW_CONSTEXPR_OR_CONST constexpr
#else
#  ifdef NULL
#    define RW_NULLPTR NULL
#  else
#    define RW_NULLPTR 0
#  endif
#  define RW_CONSTEXPR
#  define RW_CONSTEXPR_OR_CONST const
#endif

namespace redwalk
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

  static RW_CONSTEXPR_OR_CONST vid_t NONE = std::numeric_limits<vid_t>::max();
  static RW_CONSTEXPR_OR_CONST ushort_t INF_US = std::numeric_limits<ushort_t>::max();

  static RW_CONSTEXPR_OR_CONST flt_t INF_F = std::numeric_limits<flt_t>::infinity();
  static RW_CONSTEXPR_OR_CONST flt_t NAN_F = std::numeric_limits<flt_t>::quiet_NaN();
  static RW_CONSTEXPR_OR_CONST flt_t ZERO_F = flt_t(0);
  static RW_CONSTEXPR_OR_CONST flt_t ONE_F = flt_t(1);
  static RW_CONSTEXPR_OR_CONST flt_t NEG_ONE_F = flt_t(-1);
  static RW_CONSTEXPR_OR_CONST flt_t HALF_F = flt_t(0.5);
};

#endif //REDWALK_BASE_H