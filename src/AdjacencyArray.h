#ifndef GCLUST_ADJACENCY_ARRAY_H
#define GCLUST_ADJACENCY_ARRAY_H

#include "gclust_types.h"

class NeighborhoodList
{
public:
  typedef gclust::vertex_id_t vertex_id_t;
  typedef vertex_id_t vid_t;
  typedef gclust::index_t index_t;
  typedef index_t idx_t;
  typedef vid_t* iterator;
  typedef const iterator const_iterator;

private:
  iterator beg_;
  iterator end_;

public:
  NeighborhoodList() : beg_(GC_NULLPTR), end_(GC_NULLPTR) { }
  NeighborhoodList(iterator beg, iterator end) : beg_(beg), end_(end) { }
  ~NeighborhoodList() { beg_ = GC_NULLPTR; end_ = GC_NULLPTR; }

  const_iterator begin()  const { return beg_; }
  const_iterator end()    const { return end_; }
  const_iterator cbegin() const { return beg_; }
  const_iterator cend()   const { return end_; }

  inline idx_t size()   const { return end_ - beg_; }
  inline idx_t degree() const { return end_ - beg_; }
  inline vid_t operator[](int i) { return *(beg_ + i); }
};

class AdjacencyArray
{
public:
  typedef gclust::vertex_id_t vertex_id_t;
  typedef vertex_id_t vid_t;
  typedef gclust::index_t index_t;
  typedef index_t idx_t;

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
    ne_  = (edge_end - edge_beg) / 2;
    adj_ = new vid_t[2*ne_];
    vtx_ = new vid_t*[nv_+1];

    //Set up degrees
    vid_t* deg = new vid_t[nv_];

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
      int v0 = *e0;
      int v1 = *e1;
      *(vtx_[v0] + deg[v0]) = v1;
      *(vtx_[v1] + deg[v1]) = v0;
      deg[v0]++; deg[v1]++;
      e0 += inc; e1 += inc;
    }

    //Delete degrees
    delete [] deg;
  }

  template <typename MatrixType>
  AdjacencyArray(idx_t nv, const MatrixType& el)
  {
    nv_ = nv;
    ne_ = el.rows();
    adj_ = new vid_t[2*ne_];
    vtx_ = new vid_t*[nv_+1];

    //Set up degrees
    idx_t* deg = new idx_t[nv_];

    //Fill deg_ with 0
    for(idx_t i = 0; i < nv_; i++)
      deg[i] = 0;

    //Get degree of each vertex
    typedef typename MatrixType::const_iterator citr;
    citr el_itr;
    for(el_itr = el.begin(); el_itr != el.end(); el_itr++)
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

  #ifdef _GC_CXX11
  AdjacencyArray(AdjacencyArray&& mov)
  {
    nv_ = mov.nv_; mov.nv_ = 0;
    ne_ = mov.ne_; mov.ne_ = 0;
    adj_ = mov.adj_; mov.adj_ = GC_NULLPTR;
    vtx_ = mov.vtx_; mov.vtx_ = GC_NULLPTR;
  }

  AdjacencyArray& operator=(AdjacencyArray& mov)
  {
    if(adj_) { delete [] adj_; }
    if(vtx_) { delete [] vtx_; }
    nv_ = mov.nv_; mov.nv_ = 0;
    ne_ = mov.ne_; mov.ne_ = 0;
    adj_ = mov.adj_; mov.adj_ = GC_NULLPTR;
    vtx_ = mov.vtx_; mov.vtx_ = GC_NULLPTR;
    return (*this);
  }
  #endif

  inline NeighborhoodList operator[](vid_t v) const
  {
    return NeighborhoodList(vtx_[v], vtx_[v+1]);
  }

  inline idx_t degree(vid_t v) const { return vtx_[v+1] - vtx_[v]; }
  inline idx_t vcount() const { return nv_; }
  inline idx_t ecount() const { return ne_; }
};

#endif //GCLUST_ADJACENCY_ARRAY_H
