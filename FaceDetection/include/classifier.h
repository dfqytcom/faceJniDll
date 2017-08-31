#ifndef SEETA_FD_CLASSIFIER_H_
#define SEETA_FD_CLASSIFIER_H_

#include "common.h"
#include "feature_map.h"

namespace seeta {
namespace fd {

enum ClassifierType {
    LAB_Boosted_Classifier,
    SURF_MLP
};

class Classifier {
 public:
  Classifier() {}
  virtual ~Classifier() {}

  virtual void SetFeatureMap(seeta::fd::FeatureMap* feat_map) = 0;
  virtual bool Classify(float* score = nullptr, float* outputs = nullptr) = 0;
  
  virtual seeta::fd::ClassifierType type() = 0;

  DISABLE_COPY_AND_ASSIGN(Classifier);
};

}  // namespace fd
}  // namespace seeta

#endif  // SEETA_FD_CLASSIFIER_H_
