#ifndef GCLUST_ADJACENCY_ARRAY_H
#define GCLUST_ADJACENCY_ARRAY_H

class NeighborhoodList
{
public:
  typedef int* iterator;
  typedef const iterator const_iterator;

private:
  iterator beg_;
  iterator end_;

  NeighborhoodList() : beg_(0), end_(0) { }

public:
  NeighborhoodList(iterator beg, iterator end) : beg_(beg), end_(end) { }
  ~NeighborhoodList() { beg_ = 0; end_ = 0; }

  const_iterator begin()  const { return beg_; }
  const_iterator end()    const { return end_; }
  const_iterator cbegin() const { return beg_; }
  const_iterator cend()   const { return end_; }
};

class AdjacencyArray
{
private:
  int nv_;
  int ne_;
  int* adj_;
  int** vtx_;

public:
  //Edgelist as 1D array
  template <typename ForwardIterator>
  AdjacencyArray(int nv, ForwardIterator edge_beg, ForwardIterator edge_end, bool col_mjr)
  {
    nv_  = nv;
    ne_  = (edge_end - edge_beg) / 2;
    adj_ = new int[2*ne_];
    vtx_ = new int*[nv_+1];

    //Set up degrees
    int* deg = new int[nv_];

    //Fill deg_ with 0
    for(int i = 0; i < nv_; i++)
      deg[i] = 0;

    //Get degree of each vertex
    for(ForwardIterator el_itr = edge_beg; el_itr != edge_end; el_itr++)
      deg[(*el_itr)]++;

    //Set the heads
    vtx_[0] = &adj_[0];
    vtx_[nv_] = &adj_[2*ne_];
    for(int v = 1; v < nv_; v++)
      vtx_[v] = vtx_[v-1] + deg[v-1];

    //Clear degree
    for(int i = 0; i < nv; i++)
      deg[i] = 0;

    //Set the adjacency array
    ForwardIterator e0 = edge_beg;
    ForwardIterator e1 = (col_mjr) ? (e0 + ne_) : (e0 + 1);
    const int inc = (col_mjr) ? 1 : 2;
    while((e1 != edge_end) && (e0 != edge_end))
    {
      int v0 = *e0;
      int v1 = *e1;
      *(vtx_[v0] + deg[v0]) = v1;
      *(vtx_[v1] + deg[v1]) = v0;
      deg[v0]++;
      deg[v1]++;
      e0 += inc;
      e1 += inc;
    }

    //Delete degrees
    delete [] deg;
  }

  template <typename MatrixType>
  AdjacencyArray(int nv, const MatrixType& el)
  {
    nv_ = nv;
    ne_ = el.rows();
    adj_ = new int[2*ne_];
    vtx_ = new int*[nv_+1];

    //Set up degrees
    int* deg = new int[nv_];

    //Fill deg_ with 0
    for(int i = 0; i < nv_; i++)
      deg[i] = 0;

    //Get degree of each vertex
    typedef typename MatrixType::const_iterator citr;
    citr el_itr;
    for(el_itr = el.begin(); el_itr != el.end(); el_itr++)
      deg[(*el_itr)]++;

    //Set the heads
    vtx_[0] = &adj_[0];
    vtx_[nv_] = &adj_[2*ne_];
    for(int v = 1; v < nv_; v++)
      vtx_[v] = vtx_[v-1] + deg[v-1];

    //Clear degree
    for(int i = 0; i < nv; i++)
      deg[i] = 0;

    //Set the adjacency array
    for(int e = 0; e < ne_; e++)
    {
      int v0 = el(e, 0);
      int v1 = el(e, 1);
      *(vtx_[v0] + deg[v0]) = v1;
      *(vtx_[v1] + deg[v1]) = v0;
      deg[v0]++;
      deg[v1]++;
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

  NeighborhoodList operator[](int v) const { return NeighborhoodList(vtx_[v], vtx_[v+1]); }
  inline int degree(int v) const { return vtx_[v+1] - vtx_[v]; }
  inline int vcount() const { return nv_; }
  inline int ecount() const { return ne_; }

};

#endif //GCLUST_ADJACENCY_ARRAY_H
