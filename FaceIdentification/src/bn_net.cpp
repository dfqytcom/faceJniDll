#include "bn_net.h"
#include "math.h"

void BnNet::SetUp() {
  epsilon_ = *(float*)(this->hyper_param()->param("epsilon"));
 
 //check input and output blob size
  this->input_blobs().resize(1);
  this->output_blobs().resize(1);
  this->input_plugs().resize(1);
  this->output_plugs().resize(1);
  this->params().resize(3);
}

void BnNet::Execute() {
  CheckInput();
 
  const Blob* const input = this->input_blobs(0);
  const Blob* const para_mean = this->params(0);
  const Blob* const para_var = this->params(1);
  const Blob* const para_scale = this->params(2);
  Blob* const output = this->output_blobs(0);
  
  int channels = input->channels();
  CHECK_EQ(channels, para_mean->channels());
  CHECK_EQ(channels, para_var->channels());

  int height = input->height();
  int width = input->width();
  int num = input->num();
  
  float* const dst_head = new float[num*channels*height*width];

  float scale = (*para_scale)[0];
  if (scale > 0){
	scale = 1.0f / scale;
  } else if (scale < 0){
	scale = 1.0f;
  } else {  // scale and epsilon should not both be equal to 0.
	if (epsilon_ < 1e-5){
		epsilon_ = 1e-5;
	}
  }

  int size = height * width;
  for (int n = 0, offset = 0; n < num; ++n) {
    for (int ichannel = 0; ichannel < channels; ++ichannel) {
      float mean = (*para_mean)[ichannel] * scale;
      float var = sqrt((*para_var)[ichannel] * scale + epsilon_);
      for (int i = 0; i < size; ++i, ++offset) {
        dst_head[offset] = ((*input)[offset] - mean) / var;
      }
	}
  }
  output->CopyData(num, channels, height, width, dst_head); 
  delete[] dst_head;
  CheckOutput(); 
}

REGISTER_NET_CLASS(Bn);
