#ifndef SEETA_FD_MODEL_READER_H_
#define SEETA_FD_MODEL_READER_H_

#include <iosfwd>

#include "classifier.h"

namespace seeta {
namespace fd {

class ModelReader {
 public:
  ModelReader() {}
  virtual ~ModelReader() {}

  virtual bool Read(std::istream* input, seeta::fd::Classifier* model) = 0;

  DISABLE_COPY_AND_ASSIGN(ModelReader);
};

}  // namespace fd
}  // namespace seeta

#endif  // SEETA_FD_MODEL_READER_H_
