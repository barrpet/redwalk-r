#ifndef GCLUST_BASE_H
#define GCLUST_BASE_H

#include <limits>
#include <RcppEigen.h>

namespace gclust
{
  typedef float float_t;
  typedef R_xlen_t index_t;
  typedef int vertex_id_t;
  typedef unsigned short ushort_t;

  typedef Eigen::Matrix<float_t, Eigen::Dynamic, Eigen::Dynamic> MatrixF;
  typedef Eigen::Matrix<ushort_t, Eigen::Dynamic, Eigen::Dynamic> MatrixUS;

  using std::numeric_limits;
  static const vertex_id_t none = numeric_limits<vertex_id_t>::max();
  static const float_t inf = numeric_limits<float_t>::infinity();
  static const float_t nan = numeric_limits<float_t>::quiet_NaN();
  static const float_t zero = float_t(0);
  static const float_t one = float_t(1);
  static const float_t neg_one = float_t(-1);
  static const float_t half = float_t(0.5);
};

#endif //GCLUST_BASE_H