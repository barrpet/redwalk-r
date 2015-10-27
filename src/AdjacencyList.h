#ifndef GCLUST_ADJACENCY_LIST_H
#define GCLUST_ADJACENCY_LIST_H

#include <algorithm>
#include "gclust_types.h"
#include "Alloc.h"

class AdjacencyList
{
public:
  typedef gclust::index_t index_t;
  typedef gclust::vertex_id_t vertex_id_t;

  class NeighborhoodList
  {
    friend class AdjacencyList;

  public:
    typedef AdjacencyList::index_t index_t;
    typedef AdjacencyList::vertex_id_t vertex_id_t;
    typedef vertex_id_t* pointer;
    typedef pointer iterator;
    typedef iterator const const_iterator;

  private:
    pointer nbrs_;
    index_t deg_;

  public:
    NeighborhoodList() : deg_(0), nbrs_(0) { }
    NeighborhoodList(index_t deg) : deg_(deg) { this->init(deg_); }
    ~NeighborhoodList() { this->destroy(); }

    NeighborhoodList(const NeighborhoodList& cpy)
    {
      deg_ = cpy.deg_;
      nbrs_ = gclust_alloc<vertex_id_t, index_t>(deg_);
      std::copy(cpy.begin(), cpy.end(), this->begin());
    }

    NeighborhoodList& operator=(const NeighborhoodList& cpy)
    {
      this->destroy();
      this->init(cpy.deg_);
      deg_ = cpy.deg_;
      std::copy(cpy.begin(), cpy.end(), this->begin());
      return *this;
    }

    inline void sort() { std::sort(this->begin(), this->end()); }
    inline void stable_sort() { std::stable_sort(this->begin(), this->end()); }

    inline index_t size()   const { return deg_; }
    inline index_t degree() const { return deg_; }

    inline const_iterator begin()  const { return &nbrs_[0];    }
    inline const_iterator end()    const { return &nbrs_[deg_]; }
    inline const_iterator cbegin() const { return &nbrs_[0];    }
    inline const_iterator cend()   const { return &nbrs_[deg_]; }

    inline const vertex_id_t& operator[](index_t n) const { return nbrs_[n]; }
    inline const vertex_id_t& operator()(index_t n) const { return nbrs_[n]; }

  private:
    inline vertex_id_t& operator[](index_t n) { return nbrs_[n]; }
    inline vertex_id_t& operator()(index_t n) { return nbrs_[n]; }

    void init(index_t deg)
    {
      if(deg == 0) { return; }
      this->destroy();
      deg_ = deg;
      nbrs_ = gclust_alloc<vertex_id_t, index_t>(deg_);
      std::fill(this->begin(), this->end(), gclust::none);
    }

    void destroy()
    {
      if(nbrs_) { delete [] nbrs_; }
      nbrs_ = 0;
      deg_ = 0;
    }
  };

  typedef NeighborhoodList* iterator;
  typedef NeighborhoodList* const const_iterator;

private:
  NeighborhoodList* adj_;
  index_t* deg_;
  index_t nv_;
  index_t ne_;

  AdjacencyList() : nv_(0), ne_(0), adj_(0), deg_(0) { }

public:
  AdjacencyList(const AdjacencyList& cpy) = default;
  AdjacencyList& operator=(const AdjacencyList&) = default;

  AdjacencyList(index_t nv, const Rcpp::IntegerMatrix& el) : nv_(nv)
  {
    ne_ = el.rows();
    deg_ = gclust_alloc<index_t, index_t>(nv_);
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
      adj_[v1][vidx[v1]++] = v2;
      adj_[v2][vidx[v2]++] = v1;
      //vidx[v1]++;
      //vidx[v2]++;
    }
    delete [] vidx;
  }

  ~AdjacencyList()
  {
    for(iterator itr = this->begin(); itr != this->end(); itr++)
      itr->destroy();
    if(deg_) { delete [] deg_; }
    if(adj_) { delete [] adj_; }
  }

  inline index_t vcount() const { return nv_; }
  inline index_t ecount() const { return ne_; }
  inline index_t degree(index_t v_idx) const { return (*this)[v_idx].size(); }

  inline const NeighborhoodList& operator[](vertex_id_t v) const { return adj_[v]; }
  inline const vertex_id_t& operator()(vertex_id_t v, vertex_id_t n) const { return adj_[v][n]; }

  inline const_iterator begin() const { return &adj_[0]; }
  inline const_iterator end() const { return &adj_[nv_]; }

  void sort()
  {
    for(iterator itr = this->begin(); itr != this->end(); itr++)
      itr->sort();
  }

  void stable_sort()
  {
    for(iterator itr = this->begin(); itr != this->end(); itr++)
      itr->stable_sort();
  }

private:
  inline iterator begin() { return &adj_[0]; }
  inline iterator end() { return &adj_[nv_]; }
};

#endif //GCLUST_ADJACENCY_LIST_H