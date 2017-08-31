#ifndef ALIGNER_H_
#define ALIGNER_H_

#include "common.h"
#include "common_net.h"
#include "net.h"

namespace seeta {
class Aligner {
 public:
  Aligner();
  Aligner(int crop_height, int crop_width, std::string type = "bicubic");
  ~Aligner();
  // Alignment and return to a ImageData
  void Alignment(const ImageData &src_img, 
      const float* const llpoint, 
      const ImageData &dst_img); 
  // Alignment and return to a Blob
  void Alignment(const ImageData &src_img, 
      const float* const llpoint, 
      Blob* const dst_blob); 

  void set_height(int height) { crop_height_ = height; }
  void set_width(int width) {crop_width_ = width; }

  int crop_height() { return crop_height_; }
  int crop_width() { return crop_width_; }
 private:
  int crop_height_;
  int crop_width_;
  std::shared_ptr<Net> net_;
};
} // namespace 
#endif // ALIGNER_H_
