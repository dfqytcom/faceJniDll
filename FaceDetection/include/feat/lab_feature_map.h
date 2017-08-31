#ifndef SEETA_FD_FEAT_LAB_FEATURE_MAP_H_
#define SEETA_FD_FEAT_LAB_FEATURE_MAP_H_

#include <vector>

#include "feature_map.h"

namespace seeta {
namespace fd {

/** @struct LABFeature
 *  @brief Locally Assembled Binary (LAB) feature.
 *
 *  It is parameterized by the coordinates of top left corner.
 */
typedef struct LABFeature {
  int32_t x;
  int32_t y;
} LABFeature;

class LABFeatureMap : public seeta::fd::FeatureMap {
 public:
  LABFeatureMap() : rect_width_(3), rect_height_(3), num_rect_(3) {}
  virtual ~LABFeatureMap() {}

  virtual void Compute(const uint8_t* input, int32_t width, int32_t height);

  inline uint8_t GetFeatureVal(int32_t offset_x, int32_t offset_y) const {
    return feat_map_[(roi_.y + offset_y) * width_ + roi_.x + offset_x];
  }

  float GetStdDev() const;

 private:
  void Reshape(int32_t width, int32_t height);
  void ComputeIntegralImages(const uint8_t* input);
  void ComputeRectSum();
  void ComputeFeatureMap();

  template<typename Int32Type>
  inline void Integral(Int32Type* data) {
    const Int32Type* src = data;
    Int32Type* dest = data;
    const Int32Type* dest_above = dest;

    *dest = *(src++);
    for (int32_t c = 1; c < width_; c++, src++, dest++)
      *(dest + 1) = (*dest) + (*src);
    dest++;
    for (int32_t r = 1; r < height_; r++) {
      for (int32_t c = 0, s = 0; c < width_; c++, src++, dest++, dest_above++) {
        s += (*src);
        *dest = *dest_above + s;
      }
    }
  }

  const int32_t rect_width_;
  const int32_t rect_height_;
  const int32_t num_rect_;

  std::vector<uint8_t> feat_map_;
  std::vector<int32_t> rect_sum_;
  std::vector<int32_t> int_img_;
  std::vector<uint32_t> square_int_img_;
};

}  // namespace fd
}  // namespace seeta

#endif  // SEETA_FD_FEAT_LAB_FEATURE_MAP_H_
