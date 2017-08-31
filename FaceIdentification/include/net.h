#ifndef NET_H_
#define NET_H_

#include "viplnet.h"
#include "blob.h"
#include "log.h"
#include "hyper_param.h"

#include <vector>

class Net {
 public:
  Net();
  virtual ~Net();
  // initialize the networks from a binary file
  virtual void SetUp();
  // execute the networks
  virtual void Execute() = 0;

  // check input blobs
  virtual void CheckInput();

  // check output blobs
  virtual void CheckOutput();

  virtual void Release() {
    for (int i = 0; i < output_blobs_.size(); ++i)
      output_blobs_[i].Release();
  }

  Net* const father() {
    return father_;
  }
  void SetFather(Net* father) {
    father_ = father;
  }
  std::vector<std::shared_ptr<Net> >& nets() {
    return nets_;
  }
  std::shared_ptr<Net> nets(int i) {
    return nets_[i];
  }
  std::vector<Blob>& input_blobs() {
    return input_blobs_;
  }
  Blob* input_blobs(int i) {
    return &(input_blobs_[i]);
  }
  std::vector<Blob>& output_blobs() {
    return output_blobs_;
  }
  Blob* output_blobs(int i) {
    return &(output_blobs_[i]);
  }
  std::vector<std::vector<Blob*> >& output_plugs() {
    return output_plugs_;
  }
  std::vector<Blob*>& output_plugs(int i) {
    return output_plugs_[i];
  }
  std::vector<std::vector<Blob*> >& input_plugs() {
    return input_plugs_;
  }
  std::vector<Blob*>& input_plugs(int i) {
    return input_plugs_[i];
  }
  HyperParam* hyper_param() {
    return &hyper_params_;
  }
  std::vector<Blob>& params() {
    return params_;
  }
  Blob* params(int i) {
    return &(params_[i]);
  }
  // count the number of unreleased output blobs
  inline int num_output() {
    int count = 0;
    for (int i = 0; i < output_blobs_.size(); ++ i)
      if (output_plugs_[i].size() == 0) count += 1;
    return count;
  }
 protected:
  // father net
  Net* father_;
  // the limit of net name size
  static const int MAX_NET_NAME_SIZE = 50;
  // net name
  std::string name_;

  // input and output blobs
  std::vector<Blob> input_blobs_;
  std::vector<Blob> output_blobs_;

  // subnet of the networks
  std::vector<std::shared_ptr<Net> > nets_;

  // plugs
  std::vector<std::vector<Blob*> > output_plugs_;
  std::vector<std::vector<Blob*> > input_plugs_;

  // params in the networks
  HyperParam hyper_params_;
  std::vector<Blob> params_;
};

#endif //NET_H_
