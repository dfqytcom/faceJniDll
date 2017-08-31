#include "inner_product_net.h"
#include "math_functions.h"

void InnerProductNet::SetUp() {
  // check input and output blob size
  this->input_blobs().resize(1);
  this->output_blobs().resize(1);
  this->input_plugs().resize(1);
  this->output_plugs().resize(1);
  this->params().resize(1);
}

void InnerProductNet::Execute() {
  CheckInput();
  const Blob* const input = this->input_blobs(0); // src_num * vec_len
  const Blob* const weight = this->params(0);  // dst_channels * vec_len
  Blob* const output = this->output_blobs(0); // src_num * dst_channels
  
  int src_num = input->num();
  int src_channels = input->channels();
  int src_h = input->height();
  int src_w = input->width();
  int dst_channels = weight->num();
  
  LOG(DEBUG) << "input blob: (" <<src_num << "," << src_channels << "," << src_h 
    << "," << src_w << ")";

  const int vec_len = src_channels * src_h * src_w;
  float* const dst_head = new float[src_num * dst_channels];
  const float* src_data = input->data().get();
  for (int sn = 0, didx = 0; sn < src_num; ++sn) {
    const float* weight_data = weight->data().get();
    for (int dc = 0; dc < dst_channels; ++dc) {
      dst_head[didx++] = simd_dot(src_data, weight_data, vec_len);
      weight_data += vec_len;
    } // for dc
    
    src_data += vec_len;
  } // for sn
  
  output->CopyData(src_num, dst_channels, 1, 1, dst_head);
  delete[] dst_head;
  LOG(DEBUG) << "output blob: (" << output->num() << "," << output->channels() 
    << "," << output->height() << "," << output->width() << ")";
  CheckOutput();
}

REGISTER_NET_CLASS(InnerProduct);
