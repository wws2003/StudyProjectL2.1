//
//  ConvolutionProgramModel.h
//  SobelEdgeDetector
//
//  Created by wws2003 on 4/30/18.
//  Copyright © 2018 hpg. All rights reserved.
//

#ifndef ConvolutionProgramModel_h
#define ConvolutionProgramModel_h

#include "Common.h"
#include <OpenCL/OpenCL.h>

/**
 * Model to execute convolution program
 */
struct ConvolutionProgramModel {
    ConvolutionProgramModel(cl_mem sourceImageMem,
                            cl_mem outImageMem,
                            image_size_t rows,
                            image_size_t cols,
                            cl_mem filter,
                            int filterWidth,
                            cl_sampler sampler) :
    m_sourceImageMem(sourceImageMem),
    m_outImageMem(outImageMem),
    m_rows(rows),
    m_cols(cols),
    m_filter(filter),
    m_filterWidth(filterWidth),
    m_sampler(sampler)
    {
    };
    
    // Memory object for source image
    cl_mem m_sourceImageMem;
    // Memory object for out image
    cl_mem m_outImageMem;
    // Number of image row
    const image_size_t m_rows;
    // Number of image column
    const image_size_t m_cols;
    // Filter data
    cl_mem m_filter;
    // Filter size
    const int m_filterWidth;
    // Sampler
    cl_sampler m_sampler;
};

#endif /* ConvolutionProgramModel_h */
