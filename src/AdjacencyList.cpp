#include "AdjacencyList.h"
#include "Alloc.h"
#include <algorithm>

AdjacencyList::NeighborhoodList::NeighborhoodList(const NeighborhoodList& cpy)
{
  deg_ = cpy.deg_;
  nbrs_ = gclust_alloc<vertex_id_t, degree_t>(deg_);
  std::copy(cpy.begin(), cpy.end(), this->begin());
}

AdjacencyList::NeighborhoodList::NeighborhoodList(NeighborhoodList&& mov)
{
  deg_ = mov.deg_;
  nbrs_ = mov.nbrs_;
  mov.deg_ = 0;
  mov.nbrs_ = nullptr;
}

AdjacencyList::NeighborhoodList&
  AdjacencyList::NeighborhoodList::operator=(const NeighborhoodList& cpy)
{
  this->destroy();
  this->init(cpy.deg_);
  deg_ = cpy.deg_;
  std::copy(cpy.begin(), cpy.end(), this->begin());
  return *this;
}

AdjacencyList::NeighborhoodList&
  AdjacencyList::NeighborhoodList::operator=(NeighborhoodList&& mov)
{
  this->destroy();
  deg_ = mov.deg_;
  nbrs_ = mov.nbrs_;
  return *this;
}

void AdjacencyList::NeighborhoodList::init(degree_t deg)
{
  if(deg == 0) { return; }
  this->destroy();
  deg_ = deg;
  nbrs_ = gclust_alloc<vertex_id_t, index_t>(deg_);
  std::fill(this->begin(), this->end(), gclust::none);
}

AdjacencyList::AdjacencyList(index_t nv, const Rcpp::IntegerMatrix& el)
  : nv_(nv)
{
  ne_ = el.rows();
  deg_ = gclust_alloc<degree_t, index_t>(nv_);
  for(long i = 0; i < nv; i++)
    deg_[i] = 0;
  for(index_t e = 0; e < ne_; e++)
  {
    deg_[el(e,0)]++;
    deg_[el(e,1)]++;
  }

  adj_ = gclust_alloc<NeighborhoodList, index_t>(nv_);
  for(index_t v = 0; v < nv_; v++)
    adj_[v].init(deg_[v]);

  index_t* vidx = gclust_alloc<index_t, index_t>(nv_);
  std::fill(vidx, vidx + nv_, 0);
  for(index_t e = 0; e < ne_; e++)
  {
    vertex_id_t v1 = el(e,0);
    vertex_id_t v2 = el(e,1);
    adj_[v1][vidx[v1]] = v2;
    adj_[v2][vidx[v2]] = v1;
    vidx[v1]++;
    vidx[v2]++;
  }
  delete [] vidx;

  for(NeighborhoodList& nl : (*this))
    std::sort(nl.begin(), nl.end());
}
