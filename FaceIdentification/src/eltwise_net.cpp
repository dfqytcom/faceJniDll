#include "eltwise_net.h"

void EltwiseNet::SetUp() {
  op_ = *(std::string*)(this->hyper_param()->param("eltwise_op"));
  if (op_ == "SCALE") {
    scale_ = *(float*)(this->hyper_param()->param("scale"));
    this->nets().resize(0);
    this->params().resize(0);
    this->input_blobs().resize(1);
    this->output_blobs().resize(1);
    this->input_plugs().resize(1);
    this->output_plugs().resize(1);
  }
  else if (op_ == "BAIS_ADDER") {
    this->nets().resize(0);
    this->params().resize(1);
    this->input_blobs().resize(1);
    this->output_blobs().resize(1);
    this->input_plugs().resize(1);
    this->output_plugs().resize(1);
  }
  else if (op_ == "CLOSE") {
    lower_ = *(float*)(this->hyper_param()->param("lower"));
    upper_ = *(float*)(this->hyper_param()->param("upper"));
    this->nets().resize(0);
    this->params().resize(0);
    this->input_blobs().resize(1);
    this->output_blobs().resize(1);
    this->input_plugs().resize(1);
    this->output_plugs().resize(1);
  }
  // SUM
  // PROD
  // MAX
}

void EltwiseNet::Execute() {
  CheckInput();
  if (op_ == "BAIS_ADDER") {
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

    int bn = (bias->num() != 1);
    int bc = (bias->channels() != 1);
    int bh = (bias->height() != 1);
    int bw = (bias->width() != 1);
    for (int n = 0, offset = 0; n < num; ++n) {
      for (int c = 0; c < channels; ++ c) {
        for (int h = 0; h < height; ++ h) {
          for (int w = 0; w < width; ++ w, ++ offset) {
            dst_head[offset] = (*input)[offset] 
              + (*bias)[bias->offset(n*bn, c*bc, h*bh, w*bw)];
          }
        }
      }
    }

    output->CopyData(num, channels, height, width, dst_head);    
	delete[] dst_head;
  }
  else if (op_ == "SCALE") {
    const Blob* const input = this->input_blobs(0);
	LOG(DEBUG) << "input blob: (" << input->num() << ","
		<< input->channels() << ","
		<< input->height() << ","
		<< input->width() << ")";
    int count = input->count();
    Blob* const output = this->output_blobs(0);
    float* const dst_head = new float[count];
    for (int i = 0; i < count; ++ i)
      dst_head[i] = (*input)[i] * scale_;
    output->CopyData(input->num(), input->channels(), input->height(), 
                    input->width(), dst_head);
	delete[] dst_head;
  }
  else if (op_ == "CLOSE") {
    const Blob* const input = this->input_blobs(0);
	LOG(DEBUG) << "input blob: (" << input->num() << ","
		<< input->channels() << ","
		<< input->height() << ","
		<< input->width() << ")";
    int count = input->count();
    Blob* const output = this->output_blobs(0);
    float* const dst_head = new float[count];
    for (int i = 0; i < count; ++ i) {
      dst_head[i] = (*input)[i];
      dst_head[i] = std::min(dst_head[i], upper_);
      dst_head[i] = std::max(dst_head[i], lower_);
    }
    output->CopyData(input->num(), input->channels(), input->height(), 
                    input->width(), dst_head);
	delete[] dst_head;
  }
  // SUM
  // PROD
  // MAX
  CheckOutput();
}

REGISTER_NET_CLASS(Eltwise);
