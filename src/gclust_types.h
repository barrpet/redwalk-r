#ifndef GCLUST_BASE_H
#define GCLUST_BASE_H

#include <limits>
#include <RcppEigen.h>

#ifndef GCLUST_FLOAT_TYPE
#define GCLUST_FLOAT_TYPE double
#endif

#ifndef GCLUST_SIZE_TYPE
#define GCLUST_SIZE_TYPE long
#endif

#ifndef GCLUST_VID_TYPE
#define GCLUST_VID_TYPE long
#endif

namespace gclust
{
  typedef GCLUST_FLOAT_TYPE float_t;
  typedef GCLUST_SIZE_TYPE index_t;
  typedef GCLUST_SIZE_TYPE degree_t;
  typedef GCLUST_VID_TYPE vertex_id_t;

  using std::numeric_limits;
  static constexpr vertex_id_t none = numeric_limits<vertex_id_t>::max();
  static constexpr float_t inf = numeric_limits<float_t>::infinity();
  static constexpr float_t nan = numeric_limits<float_t>::quiet_NaN();
  static constexpr float_t zero = float_t(0);
  static constexpr float_t one = float_t(1);
  static constexpr float_t neg_one = float_t(-1);
  static constexpr float_t half = float_t(0.5);
};

#endif //GCLUST_BASE_H