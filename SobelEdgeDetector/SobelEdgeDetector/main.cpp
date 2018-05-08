//
//  main.cpp
//  SobelEdgeDetector
//
//  Created by wws2003 on 4/21/18.
//  Copyright © 2018 hpg. All rights reserved.
//

#include "ImageFilterApplierCLImpl.h"
#include "ImageUtil.h"
#include "FileUtil.h"
#include <iostream>
#include <sstream>
#include <OpenCL/opencl.h>

/**
 * Create instance (pointer) of ImageFilterApplier
 */
ImageFilterApplierPtr createImageFilterApplier();

/**
 * Release allocated memory for image data
 * @param imageModel
 */
void releaseImageData(BmpImageModel& imageModel);

/**
 * Check status. If not success, throw runtime exception
 * @param status Result of CL function
 * @param command Name of the CL command executed
 */
void clChk(cl_int status, const std::string& command);

/*-------------------------------------Main function------------------------------*/

int main(int argc, const char * argv[]) {
    const char* sourceImagePath = "/Users/wws2003/neo-c++/SobelEdgeDetector/SobelEdgeDetector/img/input.bmp";
    const char* outImagePath = "/Users/wws2003/neo-c++/SobelEdgeDetector/SobelEdgeDetector/img/output.bmp";
    
    // Initialize filter process arguments
    BmpImageModel sourceImageModel;
    BmpImageModel outImageModel;
    
    // Initialize filter
    filter_t filter = {
        -1.0, 0.0, 1.0,
        -2.0, 0.0, 2.0,
        -1.0, 0.0, 1.0
    };
    
    ImageUtil::readImage(sourceImagePath, sourceImageModel);
    
    // Apply filter
    ImageFilterApplierPtr pImageFilterApplier = createImageFilterApplier();
    pImageFilterApplier->filter(sourceImageModel, filter, outImageModel);
    
    // Output
    ImageUtil::writeImage(outImagePath, outImageModel);
    
    // Release
    //delete pImageFilterApplier;
    releaseImageData(sourceImageModel);
    releaseImageData(outImageModel);
    
    return 0;
}

/*-------------------------------------Function implementation------------------------------*/

ImageFilterApplierPtr createImageFilterApplier() {
    cl_int ret;
    
    // Detect platform
    cl_platform_id platformId;
    cl_uint platformCnt;
    ret = clGetPlatformIDs(1, &platformId, &platformCnt);
    clChk(ret, "clGetPlatformIDs");

    cl_device_type deviceType = CL_DEVICE_TYPE_GPU;
    
    // Detect device
    cl_device_id deviceId;
    cl_uint clDeviceCnt;
    ret = clGetDeviceIDs(platformId, deviceType, 1, &deviceId, &clDeviceCnt);
    clChk(ret, "clGetDeviceIDs");
    if (clDeviceCnt < 1) {
        throw std::runtime_error("Can not get device");
    }
    
    // Create context
    cl_context_properties contextProperties[] = {
        CL_CONTEXT_PLATFORM,
        (cl_context_properties)platformId,
        0
    };
    cl_context context = clCreateContext(contextProperties, 1, &deviceId, NULL, NULL, &ret);
    clChk(ret, "clCreateContextFromType");

    // Create command queue (only on one device)
    
    cl_command_queue_properties commandQueueProperties = 0;
    cl_command_queue commandQueue = clCreateCommandQueue(context, deviceId, commandQueueProperties, &ret);
    clChk(ret, "clCreateCommandQueue");
    
    // Create and build program
    const char* programFilePath = "/Users/wws2003/neo-c++/SobelEdgeDetector/SobelEdgeDetector/kernel/convolution.cl";
    std::string programSource = FileUtil::readFile(programFilePath);
    size_t programSourceSize = programSource.length();
    const char* pProgramSource = programSource.data();
    cl_program program = clCreateProgramWithSource(context, 1, &pProgramSource, &programSourceSize, &ret);
    clChk(ret, "clCreateProgramWithSource");
    
    ret = clBuildProgram(program, 1, &deviceId, NULL, NULL, NULL);
    clChk(ret, "clBuildProgram");
    
    // Create kernel
    const char* kernelName = "convolution";
    cl_kernel kernel = clCreateKernel(program, kernelName, &ret);
    clChk(ret, "clCreateKernel");
    
    return ImageFilterApplierPtr(new ImageFilterApplierCLImpl(context, commandQueue, kernel));
}

void releaseImageData(BmpImageModel& imageModel) {
    if (imageModel.m_fileMeta.m_headerRawData != NULL) {
        imageModel.m_fileMeta.m_headerRawData = NULL;
        delete [] imageModel.m_fileMeta.m_headerRawData;
    }
    if (imageModel.m_data.m_buffer != NULL) {
        imageModel.m_data.m_buffer = NULL;
        delete [] imageModel.m_data.m_buffer;
    }
}

void clChk(cl_int errorCode, const std::string& command) {
    if (errorCode != CL_SUCCESS) {
        std::stringstream ms;
        ms << "OpenCL runtime error for command: " << command << " with error code = " << errorCode;
        throw std::runtime_error(ms.str());
    }
}