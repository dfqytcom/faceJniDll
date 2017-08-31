#ifndef SEETA_FD_DETECTOR_H_
#define SEETA_FD_DETECTOR_H_

#include <cstdint>
#include <string>
#include <vector>

#include "common.h"
#include "util/image_pyramid.h"

namespace seeta {
namespace fd {

class Detector {
 public:
  Detector() {}
  virtual ~Detector() {}

  virtual bool LoadModel(const std::string & model_path) = 0;
  virtual std::vector<seeta::FaceInfo> Detect(seeta::fd::ImagePyramid* img_pyramid) = 0;

  virtual void SetWindowSize(int32_t size) {}
  virtual void SetSlideWindowStep(int32_t step_x, int32_t step_y) {}

  DISABLE_COPY_AND_ASSIGN(Detector);
};

}  // namespace fd
}  // namespace seeta

#endif  // SEETA_FD_DETECTOR_H_
