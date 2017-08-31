#ifndef TFORM_MAKER_NET_H_
#define TFORM_MAKER_NET_H_

#include "net.h"

// Calculate affine transformation according to feature points
class TransformationMakerNet: public Net {
 public:
  TransformationMakerNet(): Net() {}
  virtual ~TransformationMakerNet() {}
  virtual void SetUp();
  virtual void Execute();
 protected:
  // the number of feature points
  int points_num_;
};

#endif //TFORM_MAKER_NET_H_
