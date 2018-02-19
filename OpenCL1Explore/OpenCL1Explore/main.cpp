//
//  main.cpp
//  OpenCL1Explore
//
//  Created by wws2003 on 1/10/16.
//  Copyright © 2016 apecsa. All rights reserved.
//

#include <iostream>
#include <OpenCL/opencl.h>

#define MAX_DEVICES_ENTRY 100

void invesigateDevices();

cl_int detectPlatforms(cl_platform_id** ppPlatformIds, cl_uint* pNbPlatforms);

void printPlatformInfo(cl_platform_id id);

cl_int detectDevices(cl_platform_id platformId,
                     cl_device_id* pDevicesIds,
                     cl_uint maxNbDevices,
                     cl_uint* pNbDevices,
                     cl_device_type deviceType);

void printDeviceInfo(cl_device_id deviceId);

void displayPlatformInfo(cl_platform_id id,
                         cl_platform_info name,
                         std::string str);

void printWorkItemSizeInDimensions(cl_device_id deviceId, cl_uint maxWorkItemDimensions);

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    invesigateDevices();
    return 0;
}

void invesigateDevices() {
    
    cl_int err;
    cl_uint nbPlatforms;
    cl_platform_id* pPlatformIds = NULL;
    
    const cl_uint kNbDeviceTypes = 3;
    const cl_device_type kDeviceTypes[] = {CL_DEVICE_TYPE_GPU, CL_DEVICE_TYPE_CPU, CL_DEVICE_TYPE_ACCELERATOR};
    const char* kDeviceTypeNames[] = {"CL_DEVICE_TYPE_GPU", "CL_DEVICE_TYPE_CPU", "CL_DEVICE_TYPE_ACCELERATOR"};
    
    err = detectPlatforms(&pPlatformIds, &nbPlatforms);
    
    if (err == CL_SUCCESS) {
        printf("Number of platforms = %d\n", nbPlatforms);
        for (cl_uint i = 0; i < nbPlatforms; i++) {
            printPlatformInfo(pPlatformIds[i]);
            
            for (cl_uint j = 0; j < kNbDeviceTypes; j++) {
                cl_int deviceErr;
                cl_uint nbDevices;
                cl_device_id deviceIds[MAX_DEVICES_ENTRY];
                
                deviceErr = detectDevices(pPlatformIds[i], deviceIds, MAX_DEVICES_ENTRY, &nbDevices, kDeviceTypes[j]);
                
                if (deviceErr == CL_SUCCESS) {
                    printf("Number of devices type %s for platform[%d] = %d\n", kDeviceTypeNames[j], i, nbDevices);
                    
                    for (cl_uint k = 0; k < nbDevices; k++) {
                        printDeviceInfo(deviceIds[k]);
                    }
                }
            }
        }
        
        free(pPlatformIds);
    }
}

cl_int detectPlatforms(cl_platform_id** ppPlatformIds, cl_uint* pNbPlatforms) {
    cl_int err;
    cl_uint nbPlatforms;
    cl_platform_id* pPlatformIds;
    
    err = clGetPlatformIDs(0, NULL, &nbPlatforms);
    
    if (err != CL_SUCCESS) {
        return err;
    }
    
    if (nbPlatforms > 0) {
        //Alloc memory for platform_id instances
        pPlatformIds = (cl_platform_id *)malloc(sizeof(cl_platform_id) * nbPlatforms);
        
        //Re-get info
        err = clGetPlatformIDs(nbPlatforms, pPlatformIds, &nbPlatforms);
    }
    
    *ppPlatformIds = pPlatformIds;
    *pNbPlatforms = nbPlatforms;
    return err;
}

void printPlatformInfo(cl_platform_id id) {
    displayPlatformInfo(id, CL_PLATFORM_NAME, "CL_PLATFORM_NAME");
    displayPlatformInfo(id, CL_PLATFORM_PROFILE, "CL_PLATFORM_PROFILE");
    displayPlatformInfo(id, CL_PLATFORM_VERSION, "CL_PLATFORM_VERSION");
    displayPlatformInfo(id, CL_PLATFORM_VENDOR, "CL_PLATFORM_VENDOR");
}

cl_int detectDevices(cl_platform_id platformId,
                     cl_device_id* pDevicesIds,
                     cl_uint maxNbDevices,
                     cl_uint* pNbDevices,
                     cl_device_type deviceType) {
    cl_int err;
    cl_uint nbDevices;
    
    err = clGetDeviceIDs(platformId, deviceType, 0, NULL, &nbDevices);
    
    if (err != CL_SUCCESS) {
        return err;
    }
    
    if(nbDevices > 0) {
        //What is the meaning of 1 for num_entries paramter
        err = clGetDeviceIDs(platformId, deviceType, maxNbDevices, pDevicesIds, &nbDevices);
    }
    
    *pNbDevices = nbDevices;
    return err;
}

void printDeviceInfo(cl_device_id deviceId) {
    //Number of compute units
    cl_int err;
    size_t size;
    cl_uint maxComputeUnits;
    
    err = clGetDeviceInfo(deviceId,
                          CL_DEVICE_MAX_COMPUTE_UNITS,
                          sizeof(cl_uint),
                          &maxComputeUnits,
                          &size);
    
    if (err == CL_SUCCESS) {
        std::cout << "--Device has max compute units: " << maxComputeUnits << std::endl;
    }
    else {
        std::cerr << "Can not detect max compute units of device " << deviceId << std::endl;
    }
    
    //Dimension of work items
    cl_uint maxWorkItemDimensions;
    err = clGetDeviceInfo(deviceId,
                          CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS,
                          sizeof(cl_uint),
                          &maxWorkItemDimensions,
                          &size);
    
    if (err == CL_SUCCESS) {
        std::cout << "--Device has max work items dimension: " << maxWorkItemDimensions << std::endl;
        //Number of work items on each work item dimension
        printWorkItemSizeInDimensions(deviceId, maxWorkItemDimensions);
    }
    else {
        std::cerr << "Can not detect max work items dimensions of device " << deviceId << std::endl;
    }
    
    //Max work group size
    size_t maxWorkGroupSize;
    err = clGetDeviceInfo(deviceId,
                          CL_DEVICE_MAX_WORK_GROUP_SIZE,
                          sizeof(size_t),
                          &maxWorkGroupSize,
                          &size);
    if (err == CL_SUCCESS) {
        std::cout << "--Device has max work group size = " << maxWorkGroupSize << std::endl;
    }
    else {
        std::cerr << "Can not detect max work group size of device " << deviceId << " due to error " << err << std::endl;
    }
}

void displayPlatformInfo(cl_platform_id id,
                         cl_platform_info name,
                         std::string str) {
    cl_int errNum;
    std::size_t paramValueSize;
    errNum = clGetPlatformInfo(id,
                               name,
                               0,
                               NULL,
                               &paramValueSize);
    if (errNum != CL_SUCCESS) {
        std::cerr << "Failed to find OpenCL platform " << str << "." << std::endl;
        return;
    }
    
    char * info = (char *)alloca(sizeof(char) * paramValueSize);
    errNum = clGetPlatformInfo(id,
                               name,
                               paramValueSize,
                               info,
                               NULL);
    
    if (errNum != CL_SUCCESS) {
        std::cerr << "Failed to find OpenCL platform " << str << "." << std::endl;
        return;
    }
    std::cout << "\t" << str << ":\t" << info << std::endl;
}

void printWorkItemSizeInDimensions(cl_device_id deviceId, cl_uint maxWorkItemDimensions) {
    size_t* maxWorkItemSizes = (size_t*)alloca(maxWorkItemDimensions * sizeof(size_t));
    size_t size;
    
    cl_uint err = clGetDeviceInfo(deviceId,
                          CL_DEVICE_MAX_WORK_ITEM_SIZES,
                          maxWorkItemDimensions * sizeof(size_t),
                          maxWorkItemSizes,
                          &size);
    
    if (err == CL_SUCCESS) {
        for (cl_uint i = 0; i < maxWorkItemDimensions; i++) {
            std::cout << "---In one work group, dimension " << i << " has max work items size = " << maxWorkItemSizes[i] << std::endl;
        }
    }
    else {
        std::cerr << "Can not detect max work items sizes of device " << deviceId << std::endl;
    }
}

