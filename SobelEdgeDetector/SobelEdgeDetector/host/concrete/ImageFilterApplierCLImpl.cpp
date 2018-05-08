//
//  ImageFilterApplierCLImpl.cpp
//  SobelEdgeDetector
//
//  Created by wws2003 on 4/29/18.
//  Copyright © 2018 hpg. All rights reserved.
//

#include "ImageFilterApplierCLImpl.h"
#include <stdexcept>
#include <sstream>
#include <iostream>

ImageFilterApplierCLImpl::ImageFilterApplierCLImpl(cl_context context, cl_command_queue commandQueue, cl_kernel kernel) : m_context(context), m_commandQueue(commandQueue), m_kernel(kernel) {
}

void ImageFilterApplierCLImpl::filter(const BmpImageModel& sourceImage, const filter_t& filter, BmpImageModel& outImage) {
    // Prepare out image size first
    initImageOutput(sourceImage, outImage);
    
    // Initialize program model
    ConvolutionProgramModel program = createConvolutionProgramModel(sourceImage, filter, outImage);
    
    // Send data to device
    sendDataToDevice(program, sourceImage, filter);
    
    // Execute program
    executeConvolutionProgramModel(program);
    
    // Read back the result
    readResult(program, outImage);
    
    // Clear up
    releaseProgramModel(program);
}

/*---------------------------MARK: Private methods---------------------------*/

void ImageFilterApplierCLImpl::initImageOutput(const BmpImageModel& sourceImage, BmpImageModel& outImage) const {
    // Size
    outImage.m_width = sourceImage.m_width;
    outImage.m_height = sourceImage.m_height;
    
    // Data
    outImage.m_data.m_size = sourceImage.m_data.m_size;
    
    // Meta data
    outImage.m_fileMeta.m_offset = sourceImage.m_fileMeta.m_offset;
    
    // Data will be prepared when reading from cl_mem
    outImage.m_data.m_buffer = NULL;
    
    // Meta data
    outImage.m_fileMeta.m_offset = sourceImage.m_fileMeta.m_offset;
    outImage.m_fileMeta.m_headerRawData = new char[outImage.m_fileMeta.m_offset];
    std::copy(sourceImage.m_fileMeta.m_headerRawData,
              sourceImage.m_fileMeta.m_headerRawData + sourceImage.m_fileMeta.m_offset,
              outImage.m_fileMeta.m_headerRawData);
}

ConvolutionProgramModel ImageFilterApplierCLImpl::createConvolutionProgramModel(const BmpImageModel& sourceImage,
                                                           const filter_t& filter,
                                                           BmpImageModel& outImage) {
    // Memory space object for source image
    cl_mem sourceImageMem = createImage2DWriteOnlyMemSpace(sourceImage);
    // Memory space object for out image
    cl_mem outImageMem = createImage2DReadOnlyMemSpace(outImage);
    
    // Number of image row
    image_size_t rows = sourceImage.m_height;
    // Number of image column
    image_size_t cols = sourceImage.m_width;
    // Filter data
    cl_mem filterMem = createBufferMemSpace<float>(filter.size());
    // Filter size
    int filterWidth = (int)filter.size();
    // Sampler
    int ret;
    cl_sampler sampler = clCreateSampler(m_context, CL_FALSE, CL_ADDRESS_CLAMP_TO_EDGE, CL_FILTER_NEAREST, &ret);
    clChk(ret, "clCreateSampler");
    
    ConvolutionProgramModel programModel(sourceImageMem, outImageMem, rows, cols, filterMem, filterWidth, sampler);
    return programModel;
}

void ImageFilterApplierCLImpl::sendDataToDevice(const ConvolutionProgramModel& programModel, const BmpImageModel& sourceImage, const filter_t& filter) {
    // Blocking write
    cl_bool blockingWrite = CL_TRUE;
    // Offset: Start from zero
    size_t origin[3] = {0, 0, 0};
    // Element per dimension (2D image)
    size_t region[3] = {sourceImage.m_width, sourceImage.m_height, 1};
    
    // Source image
    cl_int ret = clEnqueueWriteImage(m_commandQueue,
                        programModel.m_sourceImageMem,
                        blockingWrite,
                        origin,
                        region,
                        0,
                        0,
                        sourceImage.m_data.m_buffer,
                        0,
                        NULL,
                        NULL);
    clChk(ret, "clEnqueueWriteImage");
    
    // Out image: No things need to be done yet
    
    // Filter
    ret = clEnqueueWriteBuffer(m_commandQueue,
                         programModel.m_filter,
                         blockingWrite,
                         0,
                         filter.size() * sizeof(filter_ele_t),
                         filter.data(),
                         0,
                         NULL,
                         NULL);
    clChk(ret, "clEnqueueWriteBuffer");
}

void ImageFilterApplierCLImpl::executeConvolutionProgramModel(const ConvolutionProgramModel& programModel) {
    // Set kernel arguments
    int ret = CL_SUCCESS;
    ret |= clSetKernelArg(m_kernel, 0, sizeof(cl_mem), &programModel.m_sourceImageMem);
    ret |= clSetKernelArg(m_kernel, 1, sizeof(cl_mem), &programModel.m_outImageMem);
    ret |= clSetKernelArg(m_kernel, 2, sizeof(image_size_t), &programModel.m_rows);
    ret |= clSetKernelArg(m_kernel, 3, sizeof(image_size_t), &programModel.m_cols);
    ret |= clSetKernelArg(m_kernel, 4, sizeof(cl_mem), &programModel.m_filter);
    ret |= clSetKernelArg(m_kernel, 5, sizeof(image_ele_t), &programModel.m_filterWidth);
    ret |= clSetKernelArg(m_kernel, 6, sizeof(cl_sampler), &programModel.m_sampler);
    
    clChk(ret, "clSetKernelArg");
    
    // Enqueue the kernel execution (suppose that image size fit device max support size)
    size_t globalSize[2] = {programModel.m_cols, programModel.m_rows};
    ret = clEnqueueNDRangeKernel(m_commandQueue, m_kernel, 2, NULL, globalSize, NULL, 0, NULL, NULL);
    clChk(ret, "clEnqueueNDRangeKernel");
}

void ImageFilterApplierCLImpl::readResult(const ConvolutionProgramModel& programModel, BmpImageModel& outImage) {
    // Host memory
    outImage.m_data.m_buffer = new image_ele_t[outImage.m_data.m_size];
    // Blocking read
    cl_bool blockingRead = CL_TRUE;
    // Offset: Start from zero
    size_t origin[3] = {0, 0, 0};
    // Element per dimension (2D image)
    size_t region[3] = {outImage.m_width, outImage.m_height, 1};
    
    // Enqueue read image (waiting for result ready)
    cl_int ret = clEnqueueReadImage(m_commandQueue, programModel.m_outImageMem, blockingRead, origin, region, 0, 0, outImage.m_data.m_buffer, 0, NULL, NULL);
    
    clChk(ret, "clEnqueueReadImage");
}

void ImageFilterApplierCLImpl::releaseProgramModel(ConvolutionProgramModel& programModel) {
    clReleaseMemObject(programModel.m_sourceImageMem);
    clReleaseMemObject(programModel.m_outImageMem);
    clReleaseMemObject(programModel.m_filter);
}

cl_mem ImageFilterApplierCLImpl::createImage2DWriteOnlyMemSpace(const BmpImageModel& imageModel) {
    return createImage2DMemSpace(imageModel, CL_MEM_HOST_WRITE_ONLY);
}

cl_mem ImageFilterApplierCLImpl::createImage2DReadOnlyMemSpace(const BmpImageModel& imageModel) {
    return createImage2DMemSpace(imageModel, CL_MEM_HOST_READ_ONLY);
}

cl_mem ImageFilterApplierCLImpl::createImage2DMemSpace(const BmpImageModel& imageModel, cl_mem_flags imageMemFlags) {
    cl_image_format imageFormat;
    imageFormat.image_channel_order = CL_R; // Only get first channel ?
    imageFormat.image_channel_data_type = CL_FLOAT; // float data type
    
    cl_image_desc imageDesc;
    std::memset(&imageDesc, 0, sizeof(cl_image_desc));
    imageDesc.image_type = CL_MEM_OBJECT_IMAGE2D;
    imageDesc.image_width = imageModel.m_width;
    imageDesc.image_height = imageModel.m_height;
    imageDesc.image_row_pitch = 0;
    imageDesc.image_slice_pitch = 0;
    imageDesc.num_mip_levels = 0;
    imageDesc.num_samples = 0;
    imageDesc.buffer = NULL;
    
    // Create image buffer (host ptr is set to NULL to allow data to be sent later)
    cl_int ret;
    cl_mem result = clCreateImage(m_context, imageMemFlags, &imageFormat, &imageDesc, NULL, &ret);
    
    clChk(ret, "clCreateImage");
    
    return result;
}

template<typename T>
cl_mem ImageFilterApplierCLImpl::createBufferMemSpace(size_t bufferSize) {
    int ret;
    // Create buffer (host ptr is set to NULL to allow data to be sent later)
    cl_mem buffer = clCreateBuffer(m_context, 0, bufferSize * sizeof(T), NULL, &ret);
    clChk(ret, "clCreateBuffer");
    return buffer;
}

void ImageFilterApplierCLImpl::readImage2DMem(cl_mem image2DMem, BmpImageModel& imageModel) {
    // Blocking read
    cl_bool blockingRead = CL_TRUE;
    // Offset: Start from zero
    size_t origin[3] = {0, 0, 0};
    // Element per dimension (2D image)
    size_t region[3] = {imageModel.m_width, imageModel.m_height, 1};
    
    // Enqueue read image (waiting for result ready)
    cl_int ret = clEnqueueReadImage(m_commandQueue, image2DMem, blockingRead, origin, region, 0, 0, imageModel.m_data.m_buffer, 0, NULL, NULL);
    
    clChk(ret, "clEnqueueReadImage");
}

void ImageFilterApplierCLImpl::clChk(int errorCode, const std::string& command) {
    if (errorCode != CL_SUCCESS) {
        std::stringstream ms;
        ms << "OpenCL runtime error for command: " << command << " with error code = " << errorCode;
        throw std::runtime_error(ms.str());
    }
}