#ifndef _ELTWISE_NET_H
#define _ELTWISE_NET_H

#include "net.h"
#include "net_factory.h"
#include <string>

class EltwiseNet: public Net {
 public:
  EltwiseNet(): Net() {}
  virtual ~EltwiseNet() {}
  virtual void SetUp();
  virtual void Execute();
 protected:
  std::string op_;
  float scale_;
  float upper_;
  float lower_;
};

#endif // _ELTWISE_NET_H
