#ifndef SEETA_FACE_ALIGNMENT_H_
#define SEETA_FACE_ALIGNMENT_H_

#include <cstdlib>
#include "common.h"
class CCFAN;

namespace seeta {
class FaceAlignment{
 public:
  /** A constructor with an optional argument specifying path of the model file.
  *  If called with no argument, the model file is assumed to be stored in the
  *  the working directory as "seeta_fa_v1.1.bin".
  *
  *  @param model_path Path of the model file, either absolute or relative to
  *  the working directory.
  */
    FaceAlignment(const char* model_path = NULL);

  /** A Destructor which should never be called explicitly.
  *  Release all dynamically allocated resources.
  */
    ~FaceAlignment();
    void initWithModel(const char * model_path);

  /** Detect five facial landmarks, i.e., two eye centers, nose tip and two mouth corners.
  *  @param gray_im A grayscale image
  *  @param face_info The face bounding box
  *  @param[out] points The locations of detected facial points
  */
    bool PointDetectLandmarks(ImageData gray_im, FaceInfo face_info, FacialLandmark *points);

 private:
  CCFAN *facial_detector;
};
}  // namespace seeta

#endif
