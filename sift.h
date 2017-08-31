#pragma once

#include "stdio.h"
#include <string>
#include <cmath>

typedef unsigned char BYTE;

class SIFT{
 public:
  SIFT();
  ~SIFT();

  /** Initialize the SIFT extractor.
	  *  @param im_width The width of the input image
	  *  @param im_height The height of the input image
	  *  @param patch_size The size of one patch for extracting SIFT
	  *  @param grid_spacing The stride for extracting SIFT
	  */
  void InitSIFT(int im_width, int im_height, int patch_size, int grid_spacing);

  /** Compute SIFT feature
  *  @param gray_im A grayscale image
  *  @param[out] sift_feature The output SIFT feature
  */
  void CalcSIFT(BYTE* gray_im, double* sift_feature);

 private:
  /** Implement convolutional function "filter2" same in Matlab.
  *  @param gray_im A grayscale image
  *  @param kernel A convolutional kernel
  *  @param kernel_size The size of convolutional kernel
  *  @param[out] filter_im The output image map after convolution
  */
  void filter2(double* gray_im, double* kernel, int kernel_size, double* filter_im);

  /** Sparse convolution for speed-up
  *  @param gray_im A grayscale image
  *  @param kernel A convolutional kernel
  *  @param kernel_size The size of convolutional kernel
  *  @param[out] filter_im The output image map after sparse convolution
  */
  void SparseFilter2(double* gray_im, double* kernel, int kernel_size, double* filter_im);

  /** Calculate image orientation
  *  @param gray_im A grayscale image
  *  @param[out] image_orientation The output image orientation
  */
  void ImageOrientation(double* gray_im, double* image_orientation);

  /** Calculate image orientation
  *  @param image_orientation A image orientation map
  *  @param[out] conv_im The output convolutional image
  */
  void ConvImage(double* image_orientation, double* conv_im);

  private:
  struct SIFTParam
  {
	  int image_width;
	  int image_height;
	  int patch_size;
	  int grid_spacing;
	  int angle_nums;
	  int bin_nums;

	  int image_pixel;
	  int sample_nums;
	  int sample_pixel;
	  int patch_cnt_width;
	  int patch_cnt_height;
	  int patch_dims;
	  int image_dims;

	  int filter_size;
	  double sigma;
	  double alpha;
		
  };

  SIFTParam param;

  static double delta_gauss_x[25];
  static double delta_gauss_y[25];

};

