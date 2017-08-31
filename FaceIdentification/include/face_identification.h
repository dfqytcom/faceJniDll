#ifndef SEETA_FACE_IDENTIFICATION_H_
#define SEETA_FACE_IDENTIFICATION_H_


#include "common.h"

typedef float* FaceFeatures;

namespace seeta {
class FaceIdentification {
public:
  // For cropping face only, you don't need a identification model, modelPath  
  // can be set NULL.
  // For identification, you need to create a 'FaceIdentification' object with  
  // the path of identification model, or call function 'LoadModel' after create   
  // an object.
  SEETA_API FaceIdentification(const char* model_path = nullptr);

  SEETA_API ~FaceIdentification();

  SEETA_API void initWithModel(const char* model_path);

  // If you create an 'FaceIdentification' object without identification model 
  //, you need to call LoadModel for recognition use.
  SEETA_API uint32_t LoadModel(const char* model_path);

  // Get feature's dimension.
  SEETA_API uint32_t feature_size();

  // Get cropping face width.
  SEETA_API uint32_t crop_width();

  // Get cropping face height.
  SEETA_API uint32_t crop_height();

  // Get cropping face channel.
  SEETA_API uint32_t crop_channels();

  // Crop face with 3-channels image and 5 located landmark points.
  // 'dst_image' can be initialized as a cv::Mat which cols equal to          
  //crop_width(), rows equal to crop_height() and channels equal to              
  //crop_channels().
  SEETA_API uint8_t CropFace(const ImageData &src_image,
      const FacialLandmark *llpoint,
      const ImageData &dst_image);

  // Extract feature with a cropping face.
  // 'feats' must be initialized with size of feature_size().
  SEETA_API uint8_t ExtractFeature(const ImageData &crop_image,
      FaceFeatures const feats);

  // Extract feature for face in a 3-channels image given 5 located landmark   \
  points.
  // 'feats' must be initialized with size of GetFeatureSize().
  SEETA_API uint8_t ExtractFeatureWithCrop(const ImageData &src_image,
      const FacialLandmark *llpoint,
      FaceFeatures const feats);

  // Calculate similarity of face features fc1 and fc2.
  // dim = -1 default feature size
  SEETA_API float CalcSimilarity(FaceFeatures const fc1,
      FaceFeatures const fc2, 
      long dim = -1);
  
  SEETA_API float getSimDot(FaceFeatures const fc1,
	  FaceFeatures const fc2, 
	  long dim = -1);

private:
  class Recognizer;
  Recognizer* recognizer;
};
}
#endif // SEETA_FACE_IDENTIFICATION_H_
