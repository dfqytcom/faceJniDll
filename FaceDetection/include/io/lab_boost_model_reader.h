#ifndef SEETA_FD_IO_LAB_BOOST_MODEL_READER_H_
#define SEETA_FD_IO_LAB_BOOST_MODEL_READER_H_

#include <istream>
#include <vector>

#include "model_reader.h"
#include "classifier/lab_boosted_classifier.h"

namespace seeta {
namespace fd {

class LABBoostModelReader : public ModelReader {
 public:
  LABBoostModelReader() : ModelReader() {}
  virtual ~LABBoostModelReader() {}

  virtual bool Read(std::istream* input, seeta::fd::Classifier* model);

 private:
  bool ReadFeatureParam(std::istream* input,
    seeta::fd::LABBoostedClassifier* model);
  bool ReadBaseClassifierParam(std::istream* input,
    seeta::fd::LABBoostedClassifier* model);

  int32_t num_bin_;
  int32_t num_base_classifer_;
};

}  // namespace fd
}  // namespace seeta

#endif  // SEETA_FD_IO_LAB_BOOST_MODEL_READER_H_
