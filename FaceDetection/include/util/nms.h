#ifndef SEETA_FD_UTIL_NMS_H_
#define SEETA_FD_UTIL_NMS_H_

#include <vector>

#include "common.h"

namespace seeta {
namespace fd {

void NonMaximumSuppression(std::vector<seeta::FaceInfo>* bboxes,
  std::vector<seeta::FaceInfo>* bboxes_nms, float iou_thresh = 0.8f);

}  // namespace fd
}  // namespace seeta

#endif  // SEETA_FD_UTIL_NMS_H_
