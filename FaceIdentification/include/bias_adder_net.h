#ifndef BIAS_ADDER_NET_H_
#define BIAS_ADDER_NET_H_

#include "net.h"
#include "net_factory.h"

class BiasAdderNet : public Net {
  public:
    BiasAdderNet():Net() {}
    virtual ~BiasAdderNet(){}
    virtual void SetUp();
    virtual void Execute();
};
#endif // BIAS_ADDER_NET_H_
