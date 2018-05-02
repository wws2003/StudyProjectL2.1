//
//  main.cpp
//  SobelEdgeDetector
//
//  Created by wws2003 on 4/21/18.
//  Copyright © 2018 hpg. All rights reserved.
//

#include "ImageFilterApplierCLImpl.h"
#include "ImageUtil.h"
#include <iostream>
#include <OpenCL/opencl.h>

/**
 * Create instance (pointer) of ImageFilterApplier
 */
ImageFilterApplierPtr createImageFilterApplier();

/*-------------------------------------Main function------------------------------*/

int main(int argc, const char * argv[]) {
    const char* sourceImagePath = "input.bmp";
    const char* outImagePath = "output.bmp";
    
    // Initialize filter process arguments
    BmpImageModel sourceImageModel;
    BmpImageModel outImageModel;
    // TODO Initialize filter
    filter_t filter;
    
    ImageUtil::readImage(sourceImagePath, sourceImageModel);
    
    // Apply filter
    ImageFilterApplierPtr pImageFilterApplier = createImageFilterApplier();
    pImageFilterApplier->filter(sourceImageModel, filter, outImageModel);
    
    // Output
    ImageUtil::writeImage(outImagePath, outImageModel);
    
    // Release
    delete pImageFilterApplier;
    if (sourceImageModel.m_data.m_buffer != NULL) {
        delete[] sourceImageModel.m_data.m_buffer;
    }
    if (outImageModel.m_data.m_buffer != NULL) {
        delete[] outImageModel.m_data.m_buffer;
    }
    
    return 0;
}

/*-------------------------------------Function implementation------------------------------*/

ImageFilterApplierPtr createImageFilterApplier() {
    // TODO Implement
    cl_context context;
    cl_kernel kernel;
    cl_command_queue commandQueue;
    return ImageFilterApplierPtr(new ImageFilterApplierCLImpl(context, commandQueue, kernel));
}