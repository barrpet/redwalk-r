#ifndef GCLUST_ADJACENCY_ARRAY_H
#define GCLUST_ADJACENCY_ARRAY_H

#include "gclust_types.h"
#include "Alloc.h"

class NeighborhoodList
{
public:
  typedef gclust::vid_t vid_t;
  typedef gclust::idx_t idx_t;
  typedef vid_t* iterator;
  typedef const iterator const_iterator;

private:
  iterator beg_;
  iterator end_;

public:
  NeighborhoodList() : beg_(GC_NULLPTR), end_(GC_NULLPTR) { }
  NeighborhoodList(iterator beg, iterator end) : beg_(beg), end_(end) { }
  NeighborhoodList(const NeighborhoodList& cpy) {
    beg_ = cpy.beg_;
    end_ = cpy.end_;
  }
  NeighborhoodList& operator=(const NeighborhoodList& cpy) {
    beg_ = cpy.beg_;
    end_ = cpy.end_;
    return *this;
  }
  ~NeighborhoodList() { beg_ = GC_NULLPTR; end_ = GC_NULLPTR; }

  const_iterator begin()  const { return beg_; }
  const_iterator end()    const { return end_; }
  const_iterator cbegin() const { return beg_; }
  const_iterator cend()   const { return end_; }

  inline idx_t size()   const { return end_ - beg_; }
  inline idx_t degree() const { return end_ - beg_; }
  inline vid_t operator[](int i) const { return *(beg_ + i); }
};

class AdjacencyArray
{
public:
  typedef gclust::vid_t vid_t;
  typedef gclust::idx_t idx_t;

private:
  idx_t nv_;
  idx_t ne_;
  vid_t* adj_;
  vid_t** vtx_;

public:
  //Edgelist as 1D array
  template <typename ForwardIterator>
  AdjacencyArray(idx_t nv, ForwardIterator edge_beg, ForwardIterator
    edge_end, bool col_mjr)
  {
    nv_  = nv;
    ne_  = (edge_end - edge_beg) >> 1;
    //adj_ = gclust_alloc<vid_t, idx_t>(2*ne_);
    adj_ = new vid_t[2*ne_];
    //vtx_ = gclust_alloc<vid_t*, idx_t>(nv_+1);
    vtx_ = new vid_t*[nv_+1];

    //Set up degrees
    //idx_t* deg = gclust_alloc<idx_t, idx_t>(nv_);
    idx_t* deg = new idx_t[nv_];

    //Fill deg_ with 0
    for(idx_t i = 0; i < nv_; i++)
      deg[i] = 0;

    //Get degree of each vertex
    for(ForwardIterator el_itr = edge_beg; el_itr != edge_end; el_itr++)
      deg[(*el_itr)]++;

    //Set the heads
    vtx_[0] = &adj_[0];
    vtx_[nv_] = &adj_[2*ne_];
    for(idx_t v = 1; v < nv_; v++)
      vtx_[v] = vtx_[v-1] + deg[v-1];

    //Clear degree
    for(idx_t i = 0; i < nv; i++)
      deg[i] = 0;

    //Set the adjacency array
    ForwardIterator e0 = edge_beg;
    ForwardIterator e1 = (col_mjr) ? (e0 + ne_) : (e0 + 1);
    const idx_t inc = (col_mjr) ? 1 : 2;
    while((e1 != edge_end) && (e0 != edge_end))
    {
      vid_t v0 = *e0;
      vid_t v1 = *e1;
      *(vtx_[v0] + deg[v0]) = v1;
      *(vtx_[v1] + deg[v1]) = v0;
      deg[v0]++; deg[v1]++;
      e0 += inc; e1 += inc;
    }

    //Delete degrees
    delete [] deg;
  }

  template <typename MatrixType>
  AdjacencyArray(const idx_t nv, const MatrixType& el)
  {
    nv_ = nv;
    ne_ = el.rows();
    //adj_ = gclust_alloc<vid_t, idx_t>(2*ne_);
    adj_ = new vid_t[2*ne_];
    //vtx_ = gclust_alloc<vid_t*, idx_t>(nv_+1);
    vtx_ = new vid_t*[nv_+1];

    //Set up degrees
    //idx_t* deg = gclust_alloc<idx_t, idx_t>(nv_);
    idx_t* deg = new idx_t[nv_];

    //Fill deg_ with 0
    for(idx_t i = 0; i < nv_; i++)
      deg[i] = 0;

    //Get degree of each vertex
    typedef typename MatrixType::const_iterator citr;
    citr el_itr;
    for(el_itr = el.begin(); el_itr != el.end(); ++el_itr)
      deg[(*el_itr)]++;

    //Set the heads
    vtx_[0] = &adj_[0];
    vtx_[nv_] = &adj_[2*ne_];
    for(idx_t v = 1; v < nv_; v++)
      vtx_[v] = vtx_[v-1] + deg[v-1];

    //Clear degree
    for(idx_t i = 0; i < nv; i++)
      deg[i] = 0;

    //Set the adjacency array
    for(idx_t e = 0; e < ne_; e++)
    {
      vid_t v0 = el(e, 0);
      vid_t v1 = el(e, 1);
      *(vtx_[v0] + deg[v0]) = v1;
      *(vtx_[v1] + deg[v1]) = v0;
      deg[v0]++; deg[v1]++;
    }

    //Delete degrees
    delete [] deg;
  }

  ~AdjacencyArray()
  {
    nv_ = 0; ne_ = 0;
    if(adj_) { delete [] adj_; }
    if(vtx_) { delete [] vtx_; }
  }

  AdjacencyArray(const AdjacencyArray& cpy)
  {
    nv_ = cpy.nv_;
    ne_ = cpy.ne_;
    const idx_t sum_deg = ne_ << 1;
    //adj_ = gclust_alloc<vid_t, idx_t>(2*ne_);
    adj_ = new vid_t[sum_deg];
    //vtx_ = gclust_alloc<vid_t*, idx_t>(nv_+1);
    vtx_ = new vid_t*[nv_+1];
    for(idx_t i = 0; i < sum_deg; i++)
      adj_[i] = cpy.adj_[i];
    vtx_[0] = &adj_[0];
    vtx_[nv_] = &adj_[sum_deg];
    for(idx_t v = 1; v < nv_; v++)
      vtx_[v] = vtx_[v-1] + cpy.degree(v-1);
  }

  AdjacencyArray& operator=(const AdjacencyArray& cpy)
  {
    if(this == &cpy) { return *this; }
    if(adj_) { delete [] adj_; }
    if(vtx_) { delete [] vtx_; }
    nv_ = cpy.nv_;
    ne_ = cpy.ne_;
    const idx_t sum_deg = ne_ << 1;
    //adj_ = gclust_alloc<vid_t, idx_t>(2*ne_);
    adj_ = new vid_t[sum_deg];
    //vtx_ = gclust_alloc<vid_t*, idx_t>(nv_+1);
    vtx_ = new vid_t*[nv_+1];
    for(idx_t i = 0; i < sum_deg; i++)
      adj_[i] = cpy.adj_[i];
    vtx_[0] = &adj_[0];
    vtx_[nv_] = &adj_[sum_deg];
    for(idx_t v = 1; v < nv_; v++)
      vtx_[v] = vtx_[v-1] + cpy.degree(v-1);
    return *this;
  }

  #ifdef _GC_CXX11 //C++11 Move Semantics
  AdjacencyArray(AdjacencyArray&& mov)
  {
    nv_ = mov.nv_; mov.nv_ = 0;
    ne_ = mov.ne_; mov.ne_ = 0;
    adj_ = mov.adj_; mov.adj_ = GC_NULLPTR;
    vtx_ = mov.vtx_; mov.vtx_ = GC_NULLPTR;
  }

  AdjacencyArray& operator=(AdjacencyArray&& mov)
  {
    if(adj_) { delete [] adj_; }
    if(vtx_) { delete [] vtx_; }
    nv_ = mov.nv_; mov.nv_ = 0;
    ne_ = mov.ne_; mov.ne_ = 0;
    adj_ = mov.adj_; mov.adj_ = GC_NULLPTR;
    vtx_ = mov.vtx_; mov.vtx_ = GC_NULLPTR;
    return (*this);
  }
  #endif//_GC_CXX11 (C++11 Move Semantics)

  inline NeighborhoodList operator[](vid_t v) const
  {
    return NeighborhoodList(vtx_[v], vtx_[v+1]);
  }

  inline idx_t degree(vid_t v) const { return vtx_[v+1] - vtx_[v]; }
  inline idx_t vcount() const { return nv_; }
  inline idx_t ecount() const { return ne_; }
};

#endif //GCLUST_ADJACENCY_ARRAY_H
