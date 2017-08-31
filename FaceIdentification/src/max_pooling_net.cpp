#include "max_pooling_net.h"
#include <cfloat>
#include <math.h>

#include <algorithm>

void MaxPoolingNet::SetUp() {
  kernel_h_ = kernel_w_ = 
      *(int*)(this->hyper_param()->param("kernel_size"));
  stride_h_ = stride_w_ = 
      *(int*)(this->hyper_param()->param("stride"));

  // check input and output blob size
  this->input_blobs().resize(1);
  this->output_blobs().resize(1);
  this->input_plugs().resize(1);
  this->output_plugs().resize(1);
}

void MaxPoolingNet::Execute() {
  // *** Argument *** //
  const float MIN_THRESHOLD = 0.0f;
  // *** //
  
  CheckInput();
  const Blob* const input = this->input_blobs(0);
  Blob* const output = this->output_blobs(0);
  
  int src_h = input->height();
  int src_w = input->width();
  int num = input->num();
  int channels = input->channels();

  int dst_h = static_cast<int>(ceil(static_cast<float>(
    src_h - kernel_h_) / stride_h_)) + 1;
  int dst_w = static_cast<int>(ceil(static_cast<float>(
    src_w - kernel_w_) / stride_w_)) + 1;

  int dst_count = num * channels * dst_h * dst_w;
  float* const dst_head = new float[dst_count];
  const float* src_data = input->data().get();
  float* dst_data = dst_head;
  int src_channel_off = src_h * src_w;
  int dst_channel_off = dst_h * dst_w;
  for (int n = 0; n < num; ++n) {
    for (int c = 0; c < channels; ++c) {
      for (int dh = 0, hstart = 0; dh < dst_h;
            ++dh, hstart += stride_h_) {
        int hend = std::min(hstart + kernel_h_, src_h);
        for (int dw = 0, wstart = 0; dw < dst_w;
            ++dw, wstart += stride_w_) {
          int wend = std::min(wstart + kernel_w_, src_w);
          int didx = dh * dst_w + dw;

          float max_val = MIN_THRESHOLD;
          for (int sh = hstart; sh < hend; ++sh) {
            for (int sw = wstart; sw < wend; ++sw) {
              int sidx = sh * src_w + sw;
              if (src_data[sidx] > max_val) {
                max_val = src_data[sidx];
              }
            } // for sw
          } // for sh
          dst_data[didx] = max_val;

        } // for dw
      } // for dh

      src_data += src_channel_off;
      dst_data += dst_channel_off;
    } // for c
  } // for n

  output->CopyData(num, channels, dst_h, dst_w, dst_head);
  delete[] dst_head;
  CheckOutput();
}

REGISTER_NET_CLASS(MaxPooling);
