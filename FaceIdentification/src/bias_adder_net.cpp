#include "bias_adder_net.h"

void BiasAdderNet::SetUp() {
  this->input_blobs().resize(1);
  this->output_blobs().resize(1);
  this->input_plugs().resize(1);
  this->output_plugs().resize(1);
  this->params().resize(1);
}

void BiasAdderNet::Execute() {
  CheckInput();

  const Blob* const input = this->input_blobs(0);
  const Blob* const bias = this->params(0);
  Blob* const output = this->output_blobs(0);

  int channels = bias->channels();
  CHECK_EQ(channels, input->channels());

  int height = input->height();
  int width = input->width();
  int num = input->num();
  
  LOG(DEBUG) << "input blob: (" << num << "," << input->channels() << "," 
    << height << "," << width << ")";
  LOG(DEBUG) << "bias blob: (" << bias->num() << "," << bias->channels() 
    << "," << bias->height() << "," << bias->width() << ")";
  
  float* const dst_head = new float[num*channels*height*width];

  int size = height * width;
  for (int n = 0, offset = 0; n < num; ++n) {
    for (int ichannel = 0; ichannel < channels; ++ichannel) {
      for(int i = 0; i < size; ++i, ++offset) {
        dst_head[offset] = (*input)[offset] + (*bias)[ichannel];
      }
    }
  }

  output->CopyData(num, channels, height, width, dst_head);
  delete []dst_head;
  CheckOutput();
}

REGISTER_NET_CLASS(BiasAdder);
