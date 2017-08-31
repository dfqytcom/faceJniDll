#ifndef PAD_NET_H_
#define PAD_NET_H_

#include "net.h"
#include "net_factory.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

class PadNet: public Net {
 public:
  PadNet(): Net() {}
  virtual ~PadNet() {}
  virtual void SetUp();
  virtual void Execute();
 
 protected:
  int left_, right_, bottom_, top_;
};

#endif //PAD_NET_H_
