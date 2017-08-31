#ifndef CONV_NET_H_
#define CONV_NET_H_

#include "net.h"
#include "net_factory.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

class ConvNet: public Net {
 public:
  ConvNet(): Net() {}
  virtual ~ConvNet() {}
  virtual void SetUp();
  virtual void Execute();
 
 protected:
  int stride_h_;
  int stride_w_;
};

#endif //CONV_NET_H_

