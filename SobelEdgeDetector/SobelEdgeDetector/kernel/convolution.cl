
__kernel void convolution(__read_only image2d_t sourceImage,
                          __write_only image2d_t outputImage,
                          const int rows,
                          const int cols,
                          const float* filter,
                          const int filterWidth,
                          sampler_t sampler) {
    
}