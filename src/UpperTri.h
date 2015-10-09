#ifndef GCLUST_LOWER_TRI_H
#define GCLUST_LOWER_TRI_H

#include "gclust_types.h"

#ifdef DEBUG
#include <exception>
#endif

class UpperTri
{
public:
  typedef gclust::vertex_id_t vertex_id_t;
  typedef vertex_id_t int_t;

private:
  vertex_id_t  n_     = 0;
  int_t        cap_   = 0;
  int_t*       data_  = nullptr;

  void init(vertex_id_t n, int_t value = 0);
  void destroy();

public:
  UpperTri() = default;
  UpperTri(vertex_id_t n);
  UpperTri(vertex_id_t n, int_t value);
  virtual ~UpperTri();

  inline vertex_id_t size() const { return n_; }
  inline int_t elements() const { return cap_; }
  inline void clear() { destroy(); }
  inline void resize_and_clear(vertex_id_t n) { destroy(); init(n); }

  inline int_t& operator()(vertex_id_t v1, vertex_id_t v2)
  {
    #ifdef DEBUG
    if(v1 >= v2) { throw std::out_of_range(); }
    #endif

  }

  inline int_t operator()(vertex_id_t v1, vertex_id_t v2) const;
};

#endif
