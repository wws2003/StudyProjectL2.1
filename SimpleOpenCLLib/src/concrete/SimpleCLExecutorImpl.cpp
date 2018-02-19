//
//  SimpleCLExecutorImpl.cpp
//  OpenCLHelloWorld
//
//  Created by wws2003 on 3/29/16.
//  Copyright © 2016 tbg. All rights reserved.
//

#include "SimpleCLExecutorImpl.h"
#include "GenericBuffer.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include "Util.h"

SimpleCLExecutorImpl::SimpleCLExecutorImpl(cl_context context, SimpleCLProgramFactoryPtr pSimpleCLProgramFactory) : m_context(context), m_pSimpleCLProgramFactory(pSimpleCLProgramFactory) {
}

SimpleCLExecutorImpl::~SimpleCLExecutorImpl() {
    releaseCurrentProgram();
    releaseCommandQueue();
    releaseContext();
}

void SimpleCLExecutorImpl::setup() {
    initCommandQueueOnFirstDevice();
}

//Implement
void SimpleCLExecutorImpl::loadProgram(const char* clSourceFilePath, const ProgramPrototype& programPrototype) {
    releaseCurrentProgram();

    //Get ready program from source code
    m_pCurrentCLProgram = m_pSimpleCLProgramFactory->getSimpleCLProgram(m_context,
                                                                        m_deviceIdInUsed, 
                                                                        clSourceFilePath,
                                                                        programPrototype);
}

//Implement
submission_t SimpleCLExecutorImpl::submitOnce(const char* kernelName,
                const WorkDims& workDims,
                const HostInputParams& inputParams) {
    
    EventPtr pCommandEvent = new cl_event[1];
    submission_t submission = m_commandEventPool.submitCommandEvent(pCommandEvent);
    
    cl_kernel kernel;
    kernel = m_pCurrentCLProgram->getKernelExecutable(submission, kernelName, inputParams);
    
    cl_uint nbDimensions = workDims.getNbDimensions();
    
    cl_int errNum = clEnqueueNDRangeKernel(m_commandQueue,
                                           kernel,
                                           nbDimensions,
                                           workDims.globalOffsets(),
                                           workDims.globalSizes(),
                                           workDims.localSizes(),
                                           0,
                                           NULL,
                                           pCommandEvent);
    
    if (errNum != CL_SUCCESS) {
        logAndThrow("Couldn't submit kernel");
    }
    
    return submission;
}

//Implement
void SimpleCLExecutorImpl::getResult(const submission_t& submission,
                                     HostOutputParams& outputs,
                                     TimeSpec* pTimeSpec) {
    
    //Wait for the event corresponding to the submission
    EventPtr pCommandEvent = m_commandEventPool.getCommandEventForSubmission(submission);
    if (pCommandEvent == NULL) {
        logAndThrow("Couldn't find submission");
    }
    computeRunTime(pCommandEvent, pTimeSpec);
    
    //It is the program that has to detect from kernel name and prototype
    m_pCurrentCLProgram->getKernelExecutableOutputBufferParams(submission,
                                                               m_commandQueue,
                                                               outputs);
    
    //Release submission and bound resources like event
    m_commandEventPool.releaseSubmission(submission);
    
    //Release kernel from current program
    m_pCurrentCLProgram->releaseKernelExecutable(submission);
}

//--------------------MARK: Below are private methods--------------------//

void SimpleCLExecutorImpl::initCommandQueueOnFirstDevice() {
    //First get the size of device buffer from the context
    cl_uint errNum;
    cl_device_id* pDeviceIds;
    size_t deviceBufferSize;
    cl_command_queue commandQueue = NULL;
    
    errNum = clGetContextInfo(m_context, CL_CONTEXT_DEVICES, 0, NULL, &deviceBufferSize);
    
    if (errNum != CL_SUCCESS || deviceBufferSize <= 0) {
        logAndThrow("Can not get devices buffer");
    }
    
    //Allocate memory for devices buffer
    pDeviceIds = new cl_device_id[deviceBufferSize / sizeof(cl_device_id)];
    
    //Get device ids for the context
    errNum = clGetContextInfo(m_context, CL_CONTEXT_DEVICES, deviceBufferSize, pDeviceIds, NULL);
    if (errNum != CL_SUCCESS) {
        logAndThrow("Failed to get device ids");
    }
    
    //Create command queue on the first retrieved device
    cl_command_queue_properties queueProperties = 0;
    queueProperties |= CL_QUEUE_PROFILING_ENABLE;
    commandQueue = clCreateCommandQueue(m_context, pDeviceIds[0], queueProperties, NULL);
    if (commandQueue == NULL) {
        logAndThrow("Failed to create command queue");
    }
    
    m_commandQueue = commandQueue;
    m_deviceIdInUsed = pDeviceIds[0];
    
    delete[] pDeviceIds;
}

void SimpleCLExecutorImpl::computeRunTime(EventPtr pEvent, TimeSpec* pTimeSpec) {
    clWaitForEvents(1, pEvent);
    if (pTimeSpec != NULL) {
        cl_ulong startTime = (cl_ulong)0;
        cl_ulong endTime = (cl_ulong)0;
        size_t returnBytes;
        cl_int errNum = clGetEventProfilingInfo(*pEvent,
                                                CL_PROFILING_COMMAND_START,
                                                sizeof(cl_ulong),
                                                &startTime,
                                                &returnBytes);
        errNum = clGetEventProfilingInfo(*pEvent,
                                         CL_PROFILING_COMMAND_END,
                                         sizeof(cl_ulong),
                                         &endTime,
                                         &returnBytes);
        double runTime = (double)(endTime - startTime);
        pTimeSpec->tv_sec = runTime * 1e-9;
        pTimeSpec->tv_nsec = (endTime - startTime) - pTimeSpec->tv_sec * 1e9;
    }
}

void SimpleCLExecutorImpl::releaseCommandQueue() {
    clReleaseCommandQueue(m_commandQueue);
}

void SimpleCLExecutorImpl::releaseCurrentProgram() {
    delete m_pCurrentCLProgram;
}

void SimpleCLExecutorImpl::releaseContext() {
    clReleaseContext(m_context);
}

//--------------------MARK: Below are inner class 's methods--------------------//

SimpleCLExecutorImpl::CommandEventPool::~CommandEventPool() {
    for (EventMap::iterator eIter = m_submissionEventMap.begin(); eIter != m_submissionEventMap.end(); ) {
        EventPtr pEvent = eIter->second;
        clReleaseEvent(*pEvent);
        delete pEvent;
        eIter = m_submissionEventMap.erase(eIter);
    }
}

submission_t SimpleCLExecutorImpl::CommandEventPool::submitCommandEvent(EventPtr pEvent) {
    submission_t maxSubmission = m_submissionEventMap.empty() ? 0: m_submissionEventMap.rbegin()->first;
    submission_t nextSubmission = maxSubmission + 1;
    m_submissionEventMap[nextSubmission] = pEvent;
    return nextSubmission;
}

SimpleCLExecutorImpl::EventPtr SimpleCLExecutorImpl::CommandEventPool::getCommandEventForSubmission(submission_t submission) {
    EventMap::const_iterator eIter = m_submissionEventMap.find(submission);
    if (eIter != m_submissionEventMap.end()) {
        return eIter->second;
    }
    return NULL;
}

void SimpleCLExecutorImpl::CommandEventPool::releaseSubmission(submission_t submission) {
    EventMap::const_iterator eIter = m_submissionEventMap.find(submission);
    if (m_submissionEventMap.find(submission) != m_submissionEventMap.end()) {
        EventPtr pEvent = m_submissionEventMap[submission];
        clReleaseEvent(*pEvent);
        delete pEvent;
        m_submissionEventMap.erase(eIter);
    }
}
