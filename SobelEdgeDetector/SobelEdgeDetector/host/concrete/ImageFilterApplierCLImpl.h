//
//  ImageFilterApplierCLImpl.h
//  SobelEdgeDetector
//
//  Created by wws2003 on 4/29/18.
//  Copyright © 2018 hpg. All rights reserved.
//

#ifndef ImageFilterApplierCLImpl_h
#define ImageFilterApplierCLImpl_h

#include "IImageFilterApplier.h"
#include "ConvolutionProgramModel.h"
#include <OpenCL/OpenCL.h>
#include <string>

class ImageFilterApplierCLImpl: public IImageFilterApplier {
public:
    ImageFilterApplierCLImpl(cl_context context, cl_command_queue commandQueue, cl_kernel kernel);
    virtual ~ImageFilterApplierCLImpl(){};
    
    //@Override
    virtual void filter(const BmpImageModel& sourceImage, const filter_t& filter, BmpImageModel& outImage);
    
private:
    
    /**
     * Initialize image output model instance from source images (effectively setup metadata: width, height...)
     * @param sourceImage
     * @param outImage
     */
    void initImageOutput(const BmpImageModel& sourceImage, BmpImageModel& outImage);
    
    /**
     * Initialize and return the convolution program structure to be ready for executing on device
     * @param sourceImage
     * @param filter
     * @param outImage
     */
    ConvolutionProgramModel createConvolutionProgramModel(const BmpImageModel& sourceImage, const filter_t& filter, BmpImageModel& outImage);
    
    /**
     * Send initial data to device
     * @param programModel The program model to be executed
     * @param sourceImage
     * @param filter
     */
    void sendDataToDevice(const ConvolutionProgramModel& programModel, const BmpImageModel& sourceImage, const filter_t& filter);
    
    /**
     * Execute convolution program
     * @param programModel The program model to be executed
     */
    void executeConvolutionProgramModel(const ConvolutionProgramModel& programModel);
    
    /**
     * Read result from executed programModel
     * @param programModel The program model to be executed
     * @param outImage
     */
    void readResult(const ConvolutionProgramModel& programModel, BmpImageModel& outImage);
    
    /**
     * Release program model
     * @param programModel
     */
    void releaseProgramModel(ConvolutionProgramModel& programModel);
    
    /**
     * Create instance of image memory space for image 2D given the image model (memory space allows only write from host to device)
     * @param imageModel Host-used model instance based on which image 2D memory object is created
     */
    cl_mem createImage2DWriteOnlyMemSpace(const BmpImageModel& imageModel);
    
    /**
     * Create instance of image memory space for image 2D given the image model (memory space allows only read from device to host)
     * @param imageModel Host-used model instance based on which image 2D memory object is created
     */
    cl_mem createImage2DReadOnlyMemSpace(const BmpImageModel& imageModel);
 
    /**
     * Create instance of image memory space for image 2D given the image model and memory flags
     * @param imageModel Host-used model instance based on which image 2D memory object is created
     * @param memFlags Memory flags
     */
    cl_mem createImage2DMemSpace(const BmpImageModel& imageModel, cl_mem_flags imageMemFlags);
    
    /**
     * Create buffer space
     * @param bufferSize
     */
    template<typename T>
    cl_mem createBufferMemSpace(size_t bufferSize);
    
    // TODO
    void writeImage2DMem(cl_mem image2DMem, const BmpImageModel& imageModel);
    
    /**
     * Read data from image 2D memory object to host-used model instance
     * @param image2DMem Image 2D memory object
     * @param imageModel Host-used model instance to which image data readed
     */
    void readImage2DMem(cl_mem image2DMem, BmpImageModel& imageModel);
    
    /**
     * Check if status for given command is success. If not, throw exception
     * @param status Result of CL function
     * @param command Name of the CL command executed
     */
    void clChk(int status, const std::string& command);
    
    // Context
    cl_context m_context = 0;
    
    // Command queue
    cl_command_queue m_commandQueue = 0;
    
    // Kernel
    cl_kernel m_kernel;
};

#endif /* ImageFilterApplierCLImpl_h */
