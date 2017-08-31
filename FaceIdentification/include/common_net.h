#ifndef COMMON_NET_H_
#define COMMON_NET_H_

#include "blob.h"
#include "log.h"
#include "net_factory.h"
#include "net.h"

#include <vector>

class CommonNet : public Net {
 public:
  CommonNet();
  ~CommonNet();
  // load model
  static std::shared_ptr<Net> Load(FILE* file);
  // initialize the networks from a binary file
  virtual void SetUp();
  // execute the networks
  virtual void Execute();
};

#endif // COMMON_NET_H_
