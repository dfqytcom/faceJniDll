#include "classifier/mlp.h"

#include "common.h"

#define SEETA_NUM_THREADS 4


namespace seeta {
namespace fd {

void MLPLayer::Compute(const float* input, float* output) {
#pragma omp parallel num_threads(SEETA_NUM_THREADS)
  {
#pragma omp for nowait
    for (int32_t i = 0; i < output_dim_; i++) {
      output[i] = seeta::fd::MathFunction::VectorInnerProduct(input,
        weights_.data() + i * input_dim_, input_dim_) + bias_[i];
      output[i] = (act_func_type_ == 1 ? ReLU(output[i]) : Sigmoid(-output[i]));
    }
  }
}

void MLP::Compute(const float* input, float* output) {
  layer_buf_[0].resize(layers_[0]->GetOutputDim());
  layers_[0]->Compute(input, layer_buf_[0].data());

  size_t i; /**< layer index */
  for (i = 1; i < layers_.size() - 1; i++) {
    layer_buf_[i % 2].resize(layers_[i]->GetOutputDim());
    layers_[i]->Compute(layer_buf_[(i + 1) % 2].data(), layer_buf_[i % 2].data());
  }
  layers_.back()->Compute(layer_buf_[(i + 1) % 2].data(), output);
}

void MLP::AddLayer(int32_t inputDim, int32_t outputDim, const float* weights,
    const float* bias, bool is_output) {
  if (layers_.size() > 0 && inputDim != layers_.back()->GetOutputDim())
    return;  // @todo handle the errors!!!

  std::shared_ptr<seeta::fd::MLPLayer> layer(new seeta::fd::MLPLayer(is_output ? 0 : 1));
  layer->SetSize(inputDim, outputDim);
  layer->SetWeights(weights, inputDim * outputDim);
  layer->SetBias(bias, outputDim);
  layers_.push_back(layer);
}

}  // namespace fd
}  // namespace seeta
