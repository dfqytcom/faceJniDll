#include "net.h"

Net::Net() {
  nets_.clear();
  
  input_blobs_.clear();
  output_blobs_.clear();

  input_plugs_.clear();
  output_plugs_.clear();

  father_ = nullptr;
}

Net::~Net() {
  nets_.clear();
  
  input_blobs_.clear();
  output_blobs_.clear();

  input_plugs_.clear();
  output_plugs_.clear();

  params_.clear();
}

void Net::SetUp() {
  input_blobs_.clear();
  output_blobs_.clear();

  input_plugs_.clear();
  output_plugs_.clear();
  
  nets_.clear();

  params_.clear();
}

void Net::Execute() {
  // 1. check input blobs
  CheckInput();
  // 2. execute
  for (int i = 0; i < nets_.size(); ++ i) {
    nets_[i]->Execute();
  }
  // 3. check output blobs
  CheckOutput();
}

void Net::CheckInput() {
  for (int i = 0; i < input_blobs_.size(); ++ i) {
    if (input_blobs_[i].data() == nullptr) {
      LOG(INFO) << "Net input haven't been initialized completely!";
      exit(0);
    }
  }
}

void Net::CheckOutput() {
  for (int i = 0; i < input_blobs_.size(); ++ i) {
    input_blobs_[i].Release();
  }
  for (int i = 0; i < output_blobs_.size(); ++ i) {
    // connecting output plugs
    for (std::vector<Blob*>::iterator blob = output_plugs_[i].begin();
        blob != output_plugs_[i].end(); ++ blob) {
      (*blob)->SetData(output_blobs_[i]);
    }
    // release output blobs
    if (output_plugs_[i].size() != 0) {
      output_blobs_[i].Release();
    }
  } 
}

