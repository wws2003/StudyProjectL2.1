//
//  SimpleCLProgramFactoryImpl.cpp
//  OpenCLHelloWorld
//
//  Created by wws2003 on 4/3/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#include "SimpleCLProgramFactoryImpl.h"
#include "SimpleCLProgramImpl.h"
#include "KernelArgumentContainerFactoryImpl.h"
#include <sstream>
#include <string>
#include <iostream>
#include <exception>
#include <fstream>

SimpleCLProgramFactoryImpl::SimpleCLProgramFactoryImpl() {
    m_pKernelArgumentContainerFactory = KernelArgumentContainerFactoryPtr(new KernelArgumentContainerFactoryImpl());
}

SimpleCLProgramFactoryImpl::~SimpleCLProgramFactoryImpl() {
    delete m_pKernelArgumentContainerFactory;
}

SimpleCLProgramPtr SimpleCLProgramFactoryImpl::getSimpleCLProgram(cl_context context,
                                                                  cl_device_id deviceToBuildOn,
                                                                  const char* clSourceFilePath,
                                                                  const ProgramPrototype& programPrototype) {
    
    std::string fileContent = getProgramSource(clSourceFilePath);
    const char* programSrc = fileContent.data();
    
     //Create program
    cl_program program = clCreateProgramWithSource(context, 1, &programSrc, NULL, NULL);
    if (program == NULL) {
        std::cerr << "Failed to create program from source file" << std::endl;
        throw std::runtime_error("Failed to create program from source file");
    }
    
    //Build program
    cl_device_id devices[] = {deviceToBuildOn};
    cl_int errNum = clBuildProgram(program, 1, devices, NULL, NULL, NULL);
    if (errNum != CL_SUCCESS) {
        std::cerr << "Can't build program from source file " << clSourceFilePath << std::endl;
        
        // Also determine the reason for the error
        char buildLog[16384];
        clGetProgramBuildInfo(program, deviceToBuildOn, CL_PROGRAM_BUILD_LOG, sizeof(buildLog), buildLog, NULL);
        std::cerr << "Error in kernel: " << std::endl;
        std::cerr << buildLog;
        clReleaseProgram(program);
        throw std::runtime_error("Can't build program from source file");
    }
    
    return SimpleCLProgramPtr(new SimpleCLProgramImpl(context, program, m_pKernelArgumentContainerFactory, programPrototype));
}

std::string SimpleCLProgramFactoryImpl::getProgramSource(const char* clSourceFilePath) {
    std::ifstream kernelFile(clSourceFilePath, std::ios::in);
    if (!kernelFile.is_open()) {
        std::cerr << "Failed to open CL source file for reading: " << clSourceFilePath << std::endl;
        throw std::runtime_error("Failed to open CL source file for reading");
    }
    
    std::ostringstream oss;
    oss << kernelFile.rdbuf();
    std::string srcStdStr = oss.str();
    
    kernelFile.close();
    
    return srcStdStr;
}