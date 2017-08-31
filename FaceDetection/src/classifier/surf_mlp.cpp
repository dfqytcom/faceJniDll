#include "classifier/surf_mlp.h"

#include <string>

namespace seeta {
namespace fd {

bool SURFMLP::Classify(float* score, float* outputs) {
  float* dest = input_buf_.data();
  for (size_t i = 0; i < feat_id_.size(); i++) {
    feat_map_->GetFeatureVector(feat_id_[i] - 1, dest);
    dest += feat_map_->GetFeatureVectorDim(feat_id_[i]);
  }
  output_buf_.resize(model_->GetOutputDim());
  model_->Compute(input_buf_.data(), output_buf_.data());

  if (score != nullptr)
    *score = output_buf_[0];
  if (outputs != nullptr) {
    std::memcpy(outputs, output_buf_.data(),
      model_->GetOutputDim() * sizeof(float));
  }

  return (output_buf_[0] > thresh_);
}

void SURFMLP::AddFeatureByID(int32_t feat_id) {
  feat_id_.push_back(feat_id);
}

void SURFMLP::AddLayer(int32_t input_dim, int32_t output_dim,
    const float* weights, const float* bias, bool is_output) {
  if (model_->GetLayerNum() == 0)
    input_buf_.resize(input_dim);
  model_->AddLayer(input_dim, output_dim, weights, bias, is_output);
}

}  // namespace fd
}  // namespace seeta
