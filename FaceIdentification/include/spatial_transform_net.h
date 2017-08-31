#ifndef SPATIAL_TRANSFORM_NET_H_
#define SPATIAL_TRANSFORM_NET_H_

#include "net.h"
#include "net_factory.h"

class SpatialTransformNet : public Net {
 public:
  SpatialTransformNet() : Net() {}
  virtual ~SpatialTransformNet() {}
  virtual void SetUp();
  virtual void Execute();
 
 protected:
  // sampling for common blob data
  virtual double Sampling(const float* const feat_map, int H, int W, double x, 
      double y, double scale = 1.0);

  // sampling for cv::Mat::data
  virtual double Sampling(const unsigned char* const feat_map, int c, int H, 
      int W, int C, double x, double y, double scale = 1.0);

  virtual double Cubic(double x);

  inline void Norm(std::vector<double>& weights) {
    float sum = 0;
    for (int i = 0; i < weights.size(); ++ i)
      sum += weights[i];
    for (int i = 0; i < weights.size(); ++ i)
      weights[i] /= sum;
  }

  // transformation type: linear or bicubic
  std::string type_;
  // whether input with cv::Mat::data
  int is_mat_data_;
  // output feature map height and width
  int new_height_, new_width_;
};

#endif //SPATIAL_TRANSFORM_NET_H_
