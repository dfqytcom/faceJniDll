#ifndef SEETA_FACE_DETECTION_H_
#define SEETA_FACE_DETECTION_H_

#include <cstdint>
#include <vector>

#include "common.h"

namespace seeta {

class FaceDetection {
 public:
    explicit FaceDetection(const char* model_path);
    ~FaceDetection();

  /**
   * @brief Detect faces on input image.
   *
   * (1) The input image should be gray-scale, i.e. `num_channels` set to 1.
   * (2) Currently this function does not give the Euler angles, which are
   *     left with invalid values.
   */
    std::vector<seeta::FaceInfo> Detect(const seeta::ImageData & img);

    void initWithModel(const char* model_path);
  
  /**
   * @brief Set the minimum size of faces to detect.
   *
   * The minimum size is constrained as no smaller than 20. Invalid values will
   * be ignored.
   */
    void SetMinFaceSize(int32_t size);

  /** 
   * @brief Set the maximum size of faces to detect.
   * 
   * The maximum face size actually used is computed as the minimum among: user
   * specified size, image width, image height.
   */
    void SetMaxFaceSize(int32_t size);

  /**
   * @brief Set the factor between adjacent scales of image pyramid.
   *
   * The value of the factor lies in (0, 1). For example, when it is set as 0.5,
   * an input image of size w x h will be resized to 0.5w x 0.5h, 0.25w x 0.25h,
   * 0.125w x 0.125h, etc. Invalid values will be ignored.
   */
    void SetImagePyramidScaleFactor(float factor);

  /**
   * @brief Set the sliding window step in horizontal and vertical directions.
   * 
   * The steps should take positive values, and invalid ones will be ignored.
   * Usually a step of 4 is a reasonable choice.
   */
    void SetWindowStep(int32_t step_x, int32_t step_y);

  /**
   * @brief Set the score thresh of detected faces.
   *
   * Detections with scores smaller than the threshold will not be returned.
   * Typical threshold values include 0.95, 2.8, 4.5. One can adjust the
   * threshold based on his or her own test set.
   */
  void SetScoreThresh(float thresh);

  DISABLE_COPY_AND_ASSIGN(FaceDetection);

 private:
  class Impl;
  Impl* impl_;
};

}  // namespace seeta

#endif  // SEETA_FACE_DETECTION_H_
