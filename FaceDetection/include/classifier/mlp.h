#ifndef SEETA_FD_CLASSIFIER_MLP_H_
#define SEETA_FD_CLASSIFIER_MLP_H_

#include <cmath>
#include <cstdint>
#include <memory>
#include <algorithm>
#include <vector>

#include "util/math_func.h"

namespace seeta {
namespace fd {

class MLPLayer {
 public:
  explicit MLPLayer(int32_t act_func_type = 1)
      : input_dim_(0), output_dim_(0), act_func_type_(act_func_type) {}
  ~MLPLayer() {}

  void Compute(const float* input, float* output);

  inline int32_t GetInputDim() const { return input_dim_; }
  inline int32_t GetOutputDim() const { return output_dim_; }

  inline void SetSize(int32_t inputDim, int32_t outputDim) {
    if (inputDim <= 0 || outputDim <= 0) {
      return;  // @todo handle the errors!!!
    }
    input_dim_ = inputDim;
    output_dim_ = outputDim;
    weights_.resize(inputDim * outputDim);
    bias_.resize(outputDim);
  }

  inline void SetWeights(const float* weights, int32_t len) {
    if (weights == nullptr || len != input_dim_ * output_dim_) {
      return;  // @todo handle the errors!!!
    }
    std::copy(weights, weights + input_dim_ * output_dim_, weights_.begin());
  }

  inline void SetBias(const float* bias, int32_t len) {
    if (bias == nullptr || len != output_dim_) {
      return;  // @todo handle the errors!!!
    }
    std::copy(bias, bias + output_dim_, bias_.begin());
  }

 private:
  inline float Sigmoid(float x) {
    return 1.0f / (1.0f + std::exp(x));
  }

  inline float ReLU(float x) {
    return (x > 0.0f ? x : 0.0f);
  }

 private:
  int32_t act_func_type_;
  int32_t input_dim_;
  int32_t output_dim_;
  std::vector<float> weights_;
  std::vector<float> bias_;
};


class MLP {
 public:
  MLP() {}
  ~MLP() {}

  void Compute(const float* input, float* output);

  inline int32_t GetInputDim() const {
    return layers_[0]->GetInputDim();
  }

  inline int32_t GetOutputDim() const {
    return layers_.back()->GetOutputDim();
  }

  inline int32_t GetLayerNum() const {
    return static_cast<int32_t>(layers_.size());
  }

  void AddLayer(int32_t inputDim, int32_t outputDim, const float* weights,
      const float* bias, bool is_output = false);

 private:
  std::vector<std::shared_ptr<seeta::fd::MLPLayer> > layers_;
  std::vector<float> layer_buf_[2];
};

}  // namespace fd
}  // namespace seeta

#endif  // SEETA_FD_CLASSIFIER_MLP_H_
