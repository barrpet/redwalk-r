#include "UpperTri.h"
#include <algorithm>

void UpperTri::init(vertex_id_t n, int_t value)
{
  if(data_ != nullptr) { return; }
  n_ = n;
  cap_ = (n * n - n) / 2;
  data_ = new int_t[cap_];
  std::fill(&data_[0], &data_[cap_], value);
}

void UpperTri::destroy()
{
  if(data_ == nullptr) { return; }
  delete [] data_;
  data_ = nullptr;
  n_ = 0;
  cap_ = 0;
}