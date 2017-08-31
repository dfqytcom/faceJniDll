#ifndef BN_NET_H_
#define BN_NET_H_

#include "net.h"
#include "net_factory.h"

class BnNet : public Net {
 public:
  BnNet(): Net() {}
  virtual ~BnNet() {}
  virtual void SetUp();
  virtual void Execute();

 private:
  float epsilon_;
};


# endif //BN_NET_H_
