#ifndef SEETA_FD_CLASSIFIER_SURF_MLP_H_
#define SEETA_FD_CLASSIFIER_SURF_MLP_H_

#include <memory>
#include <vector>

#include "classifier.h"
#include "classifier/mlp.h"
#include "feat/surf_feature_map.h"

namespace seeta {
namespace fd {

class SURFMLP : public Classifier {
 public:
  SURFMLP() : Classifier(), model_(new seeta::fd::MLP()) {}
  virtual ~SURFMLP() {}

  virtual bool Classify(float* score = nullptr, float* outputs = nullptr);

  inline virtual void SetFeatureMap(seeta::fd::FeatureMap* feat_map) {
    feat_map_ = dynamic_cast<seeta::fd::SURFFeatureMap*>(feat_map);
  }

  inline virtual seeta::fd::ClassifierType type() {
    return seeta::fd::ClassifierType::SURF_MLP;
  }

  void AddFeatureByID(int32_t feat_id);
  void AddLayer(int32_t input_dim, int32_t output_dim, const float* weights,
    const float* bias, bool is_output = false);

  inline void SetThreshold(float thresh) { thresh_ = thresh; }

 private:
  std::vector<int32_t> feat_id_;
  std::vector<float> input_buf_;
  std::vector<float> output_buf_;

  std::shared_ptr<seeta::fd::MLP> model_;
  float thresh_;
  seeta::fd::SURFFeatureMap* feat_map_;
};

}  // namespace fd
}  // namespace seeta

#endif  // SEETA_FD_CLASSIFIER_SURF_MLP_H_
