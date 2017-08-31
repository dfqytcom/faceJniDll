#ifndef INNER_PRODUCT_NET_H_
#define INNER_PRODUCT_NET_H_

#include "net.h"
#include "net_factory.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

class InnerProductNet: public Net {
 public:
  InnerProductNet(): Net() {}
  virtual ~InnerProductNet() {}
  virtual void SetUp();
  virtual void Execute();
 
 protected:
};

#endif //INNER_PRODUCT_NET_H_

