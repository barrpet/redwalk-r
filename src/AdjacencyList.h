#ifndef GCLUST_ADJACENCY_LIST_H
#define GCLUST_ADJACENCY_LIST_H

#include "gclust_types.h"

class AdjacencyList
{
public:
  typedef gclust::index_t index_t;
  typedef gclust::degree_t degree_t;
  typedef gclust::vertex_id_t vertex_id_t;

  class NeighborhoodList
  {
    friend class AdjacencyList;

  public:
    typedef AdjacencyList::index_t index_t;
    typedef AdjacencyList::degree_t degree_t;
    typedef AdjacencyList::vertex_id_t vertex_id_t;
    typedef vertex_id_t* pointer;
    typedef pointer iterator;
    typedef iterator const const_iterator;

  private:
    degree_t deg_ = 0;
    pointer nbrs_ = nullptr;

  public:
    NeighborhoodList(degree_t deg = 0) : deg_(deg) { this->init(deg_); }
    ~NeighborhoodList() { this->destroy(); }

    NeighborhoodList(const NeighborhoodList& cpy);
    NeighborhoodList(NeighborhoodList&& mov);
    NeighborhoodList& operator=(const NeighborhoodList& cpy);
    NeighborhoodList& operator=(NeighborhoodList&& mov);

    inline degree_t size() const { return deg_; }
    inline degree_t degree() const { return deg_; }

    inline const_iterator begin() const { return &nbrs_[0]; }
    inline const_iterator end() const { return &nbrs_[deg_]; }

    inline const_iterator cbegin() const { return &nbrs_[0]; }
    inline const_iterator cend() const { return &nbrs_[deg_]; }

    inline const vertex_id_t& operator[](index_t n) const { return nbrs_[n]; }
    inline const vertex_id_t& operator()(index_t n) const { return nbrs_[n]; }

  private:
    inline iterator begin() { return &nbrs_[0]; }
    inline iterator end() { return &nbrs_[deg_]; }

    inline vertex_id_t& operator[](index_t n) { return nbrs_[n]; }
    inline vertex_id_t& operator()(index_t n) { return nbrs_[n]; }

    void init(degree_t deg);
    inline void destroy() { deg_ = 0; delete [] nbrs_; nbrs_ = nullptr; }
  };

  typedef NeighborhoodList* iterator;
  typedef NeighborhoodList* const const_iterator;
  typedef degree_t* degree_iterator;
  typedef degree_t* const const_degree_iterator;
  typedef NeighborhoodList::iterator neighborhood_iterator;
  typedef NeighborhoodList::const_iterator const_neighbor_iterator;

private:
  index_t nv_ = 0;
  index_t ne_ = 0;
  NeighborhoodList* adj_ = nullptr;
  degree_t* deg_ = nullptr;

public:
  AdjacencyList() = delete;
  AdjacencyList(const AdjacencyList&) = default;
  AdjacencyList(AdjacencyList&&) = default;
  AdjacencyList& operator=(const AdjacencyList&) = default;
  AdjacencyList& operator=(AdjacencyList&&) = default;

  AdjacencyList(index_t nv, const Rcpp::IntegerMatrix& el);
  ~AdjacencyList() { delete [] deg_; delete [] adj_; }

  inline index_t vcount() const { return nv_; }
  inline index_t ecount() const { return ne_; }

  inline degree_t degree(index_t v_idx) const { return (*this)[v_idx].size(); }

  inline const NeighborhoodList& operator[](long v) const { return adj_[v]; }
  inline const NeighborhoodList::vertex_id_t& operator()(long v, long n) { return adj_[v][n]; }

  inline const_iterator begin() const { return &adj_[0]; }
  inline const_iterator end() const { return &adj_[nv_]; }

  inline const_degree_iterator degree_begin() const { return &deg_[0]; }
  inline const_degree_iterator degree_end() const { return &deg_[nv_]; }

private:
  inline iterator begin() { return &adj_[0]; }
  inline iterator end() { return &adj_[nv_]; }

  inline degree_iterator degree_begin() { return &deg_[0]; }
  inline degree_iterator degree_end() { return &deg_[nv_]; }
};

#endif //GCLUST_ADJACENCY_LIST_H