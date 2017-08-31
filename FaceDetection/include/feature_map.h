#ifndef SEETA_FD_FEATURE_MAP_H_
#define SEETA_FD_FEATURE_MAP_H_

#include "common.h"

namespace seeta {
namespace fd {

class FeatureMap {
 public:
  FeatureMap()
      : width_(0), height_(0) {
    roi_.x = 0;
    roi_.y = 0;
    roi_.width = 0;
    roi_.height = 0;
  }

  virtual ~FeatureMap() {}

  virtual void Compute(const uint8_t* input, int32_t width, int32_t height) = 0;

  inline virtual void SetROI(const seeta::Rect & roi) {
    roi_ = roi;
  }

 protected:
  int32_t width_;
  int32_t height_;

  seeta::Rect roi_;
};

}  // namespace fd
}  // namespace seeta

#endif  // SEETA_FD_FEATURE_MAP_H_
