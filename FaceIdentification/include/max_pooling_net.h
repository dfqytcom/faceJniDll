#ifndef MAX_POOLING_NET_H_
#define MAX_POOLING_NET_H_

#include "net.h"
#include "net_factory.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

class MaxPoolingNet: public Net {
 public:
  MaxPoolingNet(): Net() {}
  virtual ~MaxPoolingNet() {}
  virtual void SetUp();
  virtual void Execute();
 
 protected:
  int kernel_h_;
  int kernel_w_;
  int stride_h_;
  int stride_w_;
};

#endif //MAX_POOLING_NET_H_

