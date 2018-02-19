//
//  main.cpp
//  BitonicSortOpenCL
//
//  Created by wws2003 on 2/11/18.
//  Copyright © 2018 tbg. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <random>
#include <ctime>
#include <vector>
#include <algorithm>
#include <functional>
#include <cassert>
#include "IntBitonicSorter.h"
#include "IntSortingNetworkCPUImpl.h"
#include "IntSortingNetworkGPUImpl.h"
#include "SimpleCLProgramFactoryImpl.h"
#include "SimpleCLExecutorImpl.h"
#include "SimpleCLExecutorFactoryImpl.h"
#include "Util.h"

using namespace std;

#define ARRAY_SIZE (262144) // 512 * 512 = 262144
#define GLOBAL_SIZE_0 (512)
#define GLOBAL_SIZE_1 (512)
#define LOCAL_SIZE (8)

/**
 * Initialize the test data
 * @param entries
 */
void initTestData(ElementList<int>& entries);

/**
 * Test bitonic sort on CPUs
 * @param entries
 */
void testBitonicSortCPU(const ElementList<int>& entries);

/**
 * Test bitonic sort on GPUs
 * @param entries
 */
void testBitonicSortGPU(const ElementList<int>& entries);

/**
 * Test bitonic sorting network
 * @param pSortingNetwork
 */
void testBitonicSortingNetwork(SortingNetworkPtr<int> pSortingNetwork);

/**
 * Print timespec info
 */
void printTimeSpec(std::string prefixMessage, TimeSpec& timeSpec);

int main() {
    std::srand((unsigned int)std::time(0)); // use current time as seed for random generator
    ElementList<int> entries;
    
    // CPU test
    initTestData(entries);
    testBitonicSortCPU(entries);
    
    // GPU test
    initTestData(entries);
    testBitonicSortGPU(entries);
    
    std::cout << "Finish testing bitonic sort\n";
    return 0;
}

void initTestData(ElementList<int>& entries) {
    entries.clear();
    for (int i = 0; i < ARRAY_SIZE; i++) {
        entries.push_back(i);
    }
    std::random_device rd;
    std::mt19937 g(rd());
    
    std::shuffle(entries.begin(), entries.end(), g);
}

void testBitonicSortCPU(const ElementList<int>& entries) {
    // Create CPU sorting network
    SortingNetworkPtr<int> pSortingNetwork = SortingNetworkPtr<int>(new IntSortingNetworkCPUImpl(entries));
    
    // Test the sorting network in the sort function
    TimeSpec timeSpec;
    {
        ScopeTimer scopeTimer(&timeSpec);
        testBitonicSortingNetwork(pSortingNetwork);
    }
    printTimeSpec("CPU Bitonic sort time", timeSpec);
    
    // Release
    freePtr(pSortingNetwork);
}

void testBitonicSortGPU(const ElementList<int>& entries) {
    // Create GPU sorting network
    SimpleCLProgramFactoryPtr pProgramFactory = SimpleCLProgramFactoryPtr(new SimpleCLProgramFactoryImpl());
    SimpleCLExecutorFactoryPtr pSimpleExecutorFactory = SimpleCLExecutorFactoryPtr(new SimpleCLExecutorFactoryImpl(pProgramFactory));
    
    WorkDims workDims({GLOBAL_SIZE_0, GLOBAL_SIZE_1}, {LOCAL_SIZE, LOCAL_SIZE});
    cl_device_type deviceType = CL_DEVICE_TYPE_GPU;
    SortingNetworkPtr<int> pSortingNetwork = SortingNetworkPtr<int>(new IntSortingNetworkGPUImpl(entries, pSimpleExecutorFactory, workDims, deviceType));

    // Test the sorting network in the sort function
    TimeSpec timeSpec;
    {
        ScopeTimer scopeTimer(&timeSpec);
        testBitonicSortingNetwork(pSortingNetwork);
    }
    printTimeSpec("GPU Bitonic sort time", timeSpec);

    // Release
    freePtr(pSortingNetwork);
    freePtr(pSimpleExecutorFactory);
    freePtr(pProgramFactory);
}

void testBitonicSortingNetwork(SortingNetworkPtr<int> pSortingNetwork) {
    BitonicSorter<int> bitonicSorter;
    bitonicSorter.sort(pSortingNetwork, SortOrder::ASC);
    ElementList<int> sortedEntries;
    pSortingNetwork->collect(sortedEntries);
    
    size_t elementCnt = sortedEntries.size();
    for(unsigned int i = 0; i < elementCnt - 1; i++) {
        assert(sortedEntries[i] <= sortedEntries[i + 1]);
    }
}

void printTimeSpec(std::string prefixMessage, TimeSpec& timeSpec) {
    std::cout << prefixMessage  << ":\n----" << timeSpec.tv_sec << " s and " << timeSpec.tv_nsec << " ns\n";
}
